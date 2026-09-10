#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
從 Starter.prc 抽出全部 Tbmp 資源，產生 vpl.html 內嵌用的 SPRITES 表。

Palm 的 .rsrc 在轉檔到 Windows 時 resource fork 已經遺失
（Src/starter.rsrc 是 0 bytes），但編譯好的 Starter.prc 裡有完整的
74 個 Tbmp，而且 PRC 格式單純得多，所以直接從 PRC 抽。

用法（只有在 Starter.prc 的圖有變動時才需要重跑）：
    python web/tools/extract_bitmaps.py          # 就地更新 web/vpl.html
    python web/tools/extract_bitmaps.py --png out/   # 另外輸出 PNG 檔來看

資源 ID 對應的名字取自 Src/StarterRsc.h，也就是 C 程式裡
bap->BITMAPID 用的那些常數。
"""
import struct, zlib, base64, os, sys, re, io

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
PRC = os.path.join(ROOT, 'Starter.prc')
RSC_H = os.path.join(ROOT, 'Src', 'StarterRsc.h')
VPL = os.path.join(ROOT, 'web', 'vpl.html')

BEGIN = '/* <<SPRITES>> 由 tools/extract_bitmaps.py 產生，請勿手改 */'
END = '/* <</SPRITES>> */'

# Palm 1bpp: 0=白 1=黑；2bpp: 0=白 1=淺灰 2=深灰 3=黑
PAL = {1: [(255, 255, 255), (0, 0, 0)],
       2: [(255, 255, 255), (170, 170, 170), (85, 85, 85), (0, 0, 0)]}


def png(w, h, rows):
    """最小的 PNG 編碼器（truecolor，無濾波），省得依賴 PIL。"""
    raw = b''.join(b'\x00' + bytes(v for px in r for v in px) for r in rows)

    def chunk(tag, data):
        body = tag + data
        return struct.pack('>I', len(data)) + body + struct.pack('>I', zlib.crc32(body) & 0xffffffff)

    return (b'\x89PNG\r\n\x1a\n'
            + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0))
            + chunk(b'IDAT', zlib.compress(raw, 9))
            + chunk(b'IEND', b''))


def resources(data):
    """走 PRC 的資源表。PRC header 78 bytes，numRecords 在 0x4c。"""
    n = struct.unpack('>H', data[0x4c:0x4e])[0]
    ents = []
    for i in range(n):
        o = 0x4e + i * 10
        typ = data[o:o + 4].decode('latin1')
        rid, off = struct.unpack('>HI', data[o + 4:o + 10])
        ents.append([typ, rid, off])
    ents.sort(key=lambda e: e[2])
    for i in range(len(ents)):
        nxt = ents[i + 1][2] if i + 1 < len(ents) else len(data)
        ents[i].append(nxt - ents[i][2])
    return ents


def names():
    """StarterRsc.h 裡的 #define XxxBitmap 1234 -> {1234: 'XxxBitmap'}

    同一個資源 ID 可能出現兩次：真正的圖片常數以 "Bitmap" 結尾，
    表單物件則是 "BitMap"（大寫 M），例如 ID 1000 同時是 ADDBitmap
    和 AboutUnnamed1201BitMap。C 程式裡 bap->BITMAPID 用的是前者，
    所以 "Bitmap" 一律優先。
    """
    out, weak = {}, {}
    with open(RSC_H, encoding='utf-8') as f:
        for line in f:
            m = re.match(r'#define\s+(\w+)\s+(\d+)', line)
            if not m:
                continue
            name, rid = m.group(1), int(m.group(2))
            if name.endswith('Bitmap'):
                out.setdefault(rid, name)
            elif name.endswith('BitMap'):
                weak.setdefault(rid, name)
    for rid, name in weak.items():
        out.setdefault(rid, name)
    return out


def decode(blob):
    """Palm BitmapType -> (w, h, bpp, PNG bytes)。這批全是未壓縮 v1。"""
    w, h, row_bytes, flags = struct.unpack('>HHHH', blob[:8])
    bpp = blob[8] or 1
    if flags & 0x8000:
        raise ValueError('壓縮的 bitmap 尚未支援')
    pal, pix = PAL[bpp], blob[16:]
    rows = []
    for y in range(h):
        line = pix[y * row_bytes:(y + 1) * row_bytes]
        row = []
        for x in range(w):
            bit = x * bpp
            byte = line[bit // 8] if bit // 8 < len(line) else 0
            row.append(pal[(byte >> (8 - bpp - bit % 8)) & ((1 << bpp) - 1)])
        rows.append(row)
    return w, h, bpp, png(w, h, rows)


def main():
    data = open(PRC, 'rb').read()
    nm = names()
    sprites = []
    for typ, rid, off, size in resources(data):
        if typ != 'Tbmp':
            continue
        w, h, bpp, buf = decode(data[off:off + size])
        sprites.append((rid, nm.get(rid, 'id%d' % rid), w, h, bpp, buf))
    sprites.sort(key=lambda s: s[0])

    if '--png' in sys.argv:
        out = sys.argv[sys.argv.index('--png') + 1]
        os.makedirs(out, exist_ok=True)
        for rid, name, w, h, bpp, buf in sprites:
            open(os.path.join(out, '%d_%s.png' % (rid, name)), 'wb').write(buf)
        print('%d 個 PNG -> %s' % (len(sprites), out))

    lines = [BEGIN, 'const SPRITES = {']
    for rid, name, w, h, bpp, buf in sprites:
        lines.append('  %d: {n:"%s", w:%d, h:%d, src:"data:image/png;base64,%s"},'
                     % (rid, name, w, h, base64.b64encode(buf).decode()))
    lines.append('};')
    # 讓 C 程式裡的常數名在 JS 這邊也能直接用，例如 B.FORLOOPBitmap
    lines.append('const B = Object.fromEntries(Object.entries(SPRITES).map(([k, v]) => [v.n, +k]));')
    lines.append(END)
    block = '\n'.join(lines)

    html = open(VPL, encoding='utf-8').read()
    s, e = html.find(BEGIN), html.find(END)
    if s < 0 or e < 0:
        sys.exit('vpl.html 裡找不到 SPRITES 標記')
    html = html[:s] + block + html[e + len(END):]
    open(VPL, 'w', encoding='utf-8', newline='\n').write(html)
    print('%d 個 sprite 內嵌進 %s（%.1f KB）' % (len(sprites), VPL, len(block) / 1024))


if __name__ == '__main__':
    main()
