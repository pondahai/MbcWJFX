#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
從 Src/functions.c 抽出元件面板的定義，產生 vpl.html 內嵌用的 PALETTE 表。

原版的面板是執行時從一個叫 "func" 的 pdb 資料庫讀進來的
（Src/functions.c:274），那個檔案沒有留下來。但是同一個檔案裡有一段被註解掉
的 functxt 字串常數，內容就是那個資料庫的原文 —— 9 個分類、每個分類底下的
元件、連 pap 和控制點都在裡面。所以直接從註解裡把它挖出來，不用手抄。

用法（只有在 functions.c 的面板定義有變動時才需要重跑）：
    python web/tools/extract_palette.py

格線排法取自 Src/functions.c:333 的 DrawToolIcon()：
x 依序是 1 / 34 / 67，每滿三個就換行、y 加 35，起點 (1,16)。
"""
import os, re, sys, io, json

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SRC = os.path.join(ROOT, 'Src', 'functions.c')
RSC_H = os.path.join(ROOT, 'Src', 'StarterRsc.h')
VPL = os.path.join(ROOT, 'web', 'vpl.html')

BEGIN = '/* <<PALETTE>> 由 tools/extract_palette.py 產生，請勿手改 */'
END = '/* <</PALETTE>> */'


def bitmap_names():
    """資源 ID -> 常數名，規則跟 extract_bitmaps.py 一樣（Bitmap 優先於 BitMap）。"""
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


def grab_functxt():
    """挖出被註解掉的那行 char *functxt = "…"; 並還原 \\n。"""
    src = open(SRC, encoding='utf-8').read()
    m = re.search(r'functxt\[\]\s*=\s*"(.*?)"\s*;', src, re.S)
    if not m:
        sys.exit('functions.c 裡找不到 functxt 字串')
    return m.group(1).replace('\\n', '\n').split('\n')


class P:
    """順序讀 token 的小工具。這個迷你格式是位置固定的，跟存檔格式同一路數。"""

    def __init__(self, toks):
        self.t, self.i = toks, 0

    def peek(self):
        return self.t[self.i] if self.i < len(self.t) else 'ENDEND'

    def next(self):
        v = self.peek()
        self.i += 1
        return v

    def num(self):
        return int(self.next())


def parse_node(p):
    """NEWNODE 已經被吃掉了，從這裡開始讀一個元件。"""
    node = {'TYPE': None, 'bap': {}, 'pap': None, 'items': None, 'loop': False}
    while True:
        tok = p.next()
        if tok == 'ENDNODE' or tok == 'ENDEND':
            return node
        elif tok == 'TYPE':
            node['TYPE'] = p.num()
        elif tok == 'NEWBAP':
            bap = {'ctrl': []}
            while True:
                t2 = p.next()
                if t2 == 'ENDBAP':
                    break
                elif t2 == 'BITMAPID':
                    bap['BITMAPID'] = p.num()
                elif t2 == 'X':
                    bap['x'] = p.num()
                elif t2 == 'Y':
                    bap['y'] = p.num()
                elif t2 == 'EXTX':
                    bap['w'] = p.num()
                elif t2 == 'EXTY':
                    bap['h'] = p.num()
                elif t2 == 'NEWCTRLNODE':
                    c = {}
                    while True:
                        t3 = p.next()
                        if t3 == 'ENDCTRLNODE':
                            break
                        elif t3 == 'ID':
                            c['ID'] = p.num()
                        elif t3 == 'X':
                            c['x'] = p.num()
                        elif t3 == 'Y':
                            c['y'] = p.num()
                        elif t3 == 'EXTX':
                            c['w'] = p.num()
                        elif t3 == 'EXTY':
                            c['h'] = p.num()
                    bap['ctrl'].append(c)
            node['bap'] = bap
        elif tok == 'NEWPAP':
            pap = {}
            while True:
                t2 = p.next()
                if t2 == 'ENDPAP':
                    break
                elif t2 == 'BITMAPID':
                    pap['BITMAPID'] = p.num()
                elif t2 == 'EXTX':
                    pap['w'] = p.num()
                elif t2 == 'EXTY':
                    pap['h'] = p.num()
            node['pap'] = pap
        elif tok == 'NEWLOOPHOOK':
            node['loop'] = True
        elif tok == 'NEWHOOKP':
            items = []
            while True:
                t2 = p.next()
                if t2 == 'ENDHOOKP':
                    break
                if t2 == 'NEWNODE':
                    items.append(parse_node(p))
            node['items'] = items


def main():
    p = P(grab_functxt())
    cats = []
    while p.peek() != 'ENDEND':
        if p.next() == 'NEWNODE':
            cats.append(parse_node(p))

    nm = bitmap_names()

    def js_node(n):
        b = n['bap']
        o = {'bmp': b['BITMAPID'], 'w': b.get('w', 16), 'h': b.get('h', 16),
             'name': nm.get(b['BITMAPID'], '?').replace('Bitmap', '')}
        if n['pap']:
            o['pap'] = {'bmp': n['pap']['BITMAPID'],
                        'w': n['pap'].get('w', 16), 'h': n['pap'].get('h', 16)}
        if b['ctrl']:
            o['ctrl'] = b['ctrl']
        if n['loop']:
            o['loop'] = True
        return o

    out = []
    for c in cats:
        entry = js_node(c)
        entry['items'] = [js_node(i) for i in (c['items'] or [])]
        out.append(entry)

    body = json.dumps(out, ensure_ascii=False, separators=(',', ':'))
    block = '%s\nconst PALETTE = %s;\n%s' % (BEGIN, body, END)

    html = open(VPL, encoding='utf-8').read()
    s, e = html.find(BEGIN), html.find(END)
    if s < 0 or e < 0:
        sys.exit('vpl.html 裡找不到 PALETTE 標記')
    open(VPL, 'w', encoding='utf-8', newline='\n').write(
        html[:s] + block + html[e + len(END):])

    sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8')
    print('%d 個分類、%d 個元件 -> %s'
          % (len(out), sum(len(c['items']) for c in out), VPL))
    for c in out:
        print('  %-12s %s' % (c['name'], ', '.join(i['name'] for i in c['items']) or '(空)'))


if __name__ == '__main__':
    main()
