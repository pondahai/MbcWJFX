#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
產生一個測試用的 .pdb 存檔，格式完全照 Src/save.c 的 DataStruct2ASCII() 寫。

repo 裡沒有留下任何當年存的 .pdb，所以先造一個來驗證讀檔器。
內容刻意跟 web/vpl.html 的內建測試場景一樣，載進去應該畫出同一張圖
（等於一個 round-trip 測試）。

    python web/tools/make_sample_pdb.py

注意：外層的 PDB 容器是照標準 PDB 格式寫的，Palm 的 FileStream 在記錄裡
還有它自己的表頭，本工具沒有重現。讀檔器是用掃描 "STARTBLOCK" 的方式取出
內文，所以兩種都吃得下。真正由 Palm 寫出來的檔案還沒驗證過。
"""
import struct, os, time

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OUT = os.path.join(ROOT, 'web', 'samples', 'demo.pdb')

# Src/StarterRsc.h 的常數
CTRLU8BLOCK, INDICATOR8BLOCK = 3200, 3100
CTRLU8, INDICATOR8 = 1500, 1400
FORLOOP, SWITCHCASE = 4900, 5100
FORLOOPN, FORLOOPI = 7900, 8000
FUNCARITH, LAMP, LEDOFF = 6100, 4300, 4400
CUSTOMBLOCK = 3800

HOOKBLOCK, FUNCBLOCK, LOOPBLOCK, CASEBLOCK = 1, 2, 3, 4


class W:
    """照 save.c 的 AddString() 一行一行疊出內文。"""

    def __init__(self):
        self.out = []

    def line(self, s):
        self.out.append(str(s))

    def io(self, nid, typ, tl, ext, value=0, nbytes=2):
        self.line('ADDIONODE')
        self.line('ID'); self.line(nid)
        self.line('TYPE'); self.line(typ)
        self.line('BYTES'); self.line(nbytes)
        # save.c 只在 BYTES 是 2 或 4 的時候寫 VALUE；Palm 的 sizeof(int) 是 2
        self.line('VALUE'); self.line(value)
        self.line('TL'); self.line(tl[0]); self.line(tl[1])
        self.line('SIZE'); self.line(ext[0]); self.line(ext[1])

    def ctrl(self, cid, tl, ext):
        self.line('ADDCTRLNODE')
        self.line('ID'); self.line(cid)
        self.line('TL'); self.line(tl[0]); self.line(tl[1])
        self.line('SIZE'); self.line(ext[0]); self.line(ext[1])

    def node(self, nid, typ, bitmap, xy, size,
             ios=(), ctrls=(), pap=None, loop=None, cases=None, filename=None):
        self.line('NEWNODE')
        self.line('ID'); self.line(nid)
        self.line('TYPE'); self.line(typ)
        self.line('NEWBAP')
        self.line('BITMAPID'); self.line(bitmap)
        self.line('XY'); self.line(xy[0]); self.line(xy[1])
        self.line('SIZE'); self.line(size[0]); self.line(size[1])
        if ios:
            self.line('NEWIONODEP')
            for a in ios:
                self.io(*a)
        if ctrls:
            self.line('NEWCTRLNODEP')
            for c in ctrls:
                self.ctrl(*c)
        self.line('ENDBAP')
        if pap:
            self.line('NEWPAP')
            self.line('BITMAPID'); self.line(pap['bitmap'])
            self.line('XY'); self.line(pap['xy'][0]); self.line(pap['xy'][1])
            self.line('SIZE'); self.line(pap['size'][0]); self.line(pap['size'][1])
            self.line('ENDPAP')
        if loop is not None:                       # save.c:202 LOOPBLOCK 只有一個 hook
            self.line('NEWLOOPHOOK')
            self.line('NEWHOOK')
            loop(self)
        if cases is not None:                      # save.c:209 CASEBLOCK 有一串 hook
            self.line('NEWCASEHOOK')
            for page in cases:
                self.line('NEWHOOK')
                page(self)
        if filename:
            self.line('FILENAME'); self.line(filename)
        self.line('ENDNODE')

    def body(self, nodes, wires):
        """一個 hook 的內容：STARTBLOCK…ENDBLOCK + STARTWIRE…ENDWIRE"""
        self.line('STARTBLOCK')
        for n in nodes:
            n(self)
        self.line('ENDBLOCK')
        self.line('STARTWIRE')
        for sb, sn, eb, en in wires:
            self.line('STARTBID'); self.line(sb)
            self.line('STARTNID'); self.line(sn)
            self.line('ENDBID'); self.line(eb)
            self.line('ENDNID'); self.line(en)
        self.line('ENDWIRE')

    def text(self):
        return '\n'.join(self.out) + '\n'


def build():
    w = W()

    # 迴圈內部：N、I 兩個預設元件加一個算術元件，內部自己有一條線
    def loop_body(w):
        w.body([
            lambda w: w.node(8, FUNCBLOCK, FORLOOPN, (8, 13), (10, 10),
                             ios=[(0, 1, (0, 0), (10, 10), 10)]),
            lambda w: w.node(9, FUNCBLOCK, FORLOOPI, (8, 45), (10, 10),
                             ios=[(0, 0, (0, 0), (10, 10), 0)]),
            lambda w: w.node(10, FUNCBLOCK, FUNCARITH, (30, 15), (32, 32),
                             ios=[(0, 1, (0, 0), (32, 32), 0)]),
        ], [(9, 0, 10, 0)])

    # switch case 的兩頁。注意 save.c 沒有把 hook->name 寫進檔案，
    # 所以分頁名稱（原本的 "true"/"false"）在存檔裡是遺失的。
    def case_true(w):
        w.body([lambda w: w.node(11, FUNCBLOCK, LAMP, (15, 18), (16, 16),
                                 ios=[(0, 1, (0, 0), (16, 16), 1)])], [])

    def case_false(w):
        w.body([lambda w: w.node(12, FUNCBLOCK, LEDOFF, (15, 18), (16, 16),
                                 ios=[(0, 1, (0, 0), (16, 16), 0)])], [])

    # 最外層。第 1 個 block 是 HOOKBLOCK（ID 1），代表這個存檔本身當成
    # 自訂元件時的外框 —— load.c:676 的 wire 解析把 ID 1 視為它。
    w.body([
        lambda w: w.node(1, HOOKBLOCK, CUSTOMBLOCK, (0, 0), (32, 32),
                         ios=[(0, 1, (0, 0), (16, 32)), (1, 0, (16, 0), (16, 32))],
                         filename='demo'),
        lambda w: w.node(2, FUNCBLOCK, CTRLU8BLOCK, (6, 16), (32, 16),
                         ios=[(0, 0, (0, 0), (32, 16), 42)],
                         pap={'bitmap': CTRLU8, 'xy': (10, 50), 'size': (32, 16)}),
        lambda w: w.node(3, FUNCBLOCK, INDICATOR8BLOCK, (108, 16), (32, 16),
                         ios=[(0, 1, (0, 0), (32, 16), 0)],
                         pap={'bitmap': INDICATOR8, 'xy': (10, 80), 'size': (32, 16)}),
        lambda w: w.node(4, LOOPBLOCK, FORLOOP, (12, 52), (74, 62),
                         ios=[(0, 1, (0, 24), (6, 6))],
                         ctrls=[(1, (67, 55), (7, 7))],
                         loop=loop_body),
        lambda w: w.node(5, CASEBLOCK, SWITCHCASE, (100, 60), (46, 40),
                         ios=[(0, 1, (0, 20), (6, 6))],
                         ctrls=[(1, (39, 33), (7, 7)), (2, (39, 1), (6, 13)), (3, (1, 1), (6, 13))],
                         cases=[case_true, case_false]),
    ], [
        (2, 0, 3, 0),   # 控制鈕 -> 顯示器
        (2, 0, 4, 0),   # 控制鈕 -> 迴圈
        (4, 0, 5, 0),   # 迴圈 -> switch case
    ])

    return w.text()


def pdb(name, body, dbtype=b'save', creator=b'wjfx'):
    """最小的 PDB 容器：78 bytes 表頭 + 記錄表 + 一筆記錄。"""
    # Palm 的時間原點是 1904-01-01
    now = int(time.time()) + 2082844800
    nrec = 1
    hdr = (name.encode('latin1')[:31].ljust(32, b'\0')
           + struct.pack('>HH', 0, 0)                 # attributes, version
           + struct.pack('>III', now, now, 0)         # ctime, mtime, btime
           + struct.pack('>III', 0, 0, 0)             # modnum, appInfo, sortInfo
           + dbtype + creator
           + struct.pack('>II', 0, 0)                 # uniqueIDSeed, nextRecordList
           + struct.pack('>H', nrec))
    offset = len(hdr) + nrec * 8 + 2                  # +2 是記錄表後的填充
    entries = struct.pack('>IBBBB', offset, 0, 0, 0, 1)
    return hdr + entries + b'\0\0' + body.encode('latin1')


def main():
    text = build()
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    open(OUT, 'wb').write(pdb('demo', text))
    print('%s（內文 %d 行，檔案 %d bytes）' % (OUT, text.count('\n'), os.path.getsize(OUT)))


if __name__ == '__main__':
    main()
