# web/ — 網頁版

把 Palm OS 版的 MbcWJFX 改寫成瀏覽器版本。**單一 HTML 檔，打開就能跑，沒有 build step。**

```
web/
├── vpl.html                  ← 用瀏覽器直接開這個檔
├── README.md
├── samples/
│   └── demo.pdb              ← 測試用存檔
└── tools/
    ├── extract_bitmaps.py    ← 從 Starter.prc 抽圖的一次性工具
    └── make_sample_pdb.py    ← 產生上面那個測試存檔
```

## 現況

目前是**唯讀檢視器**：可以讀 Palm 的 `.pdb` 存檔、把 block diagram 畫在
160×160 的畫布上、點選元件、看場景結構。還不能編輯、不能執行。

載入方式是選檔或把檔案拖到畫面上 —— `file://` 底下瀏覽器不准 fetch 本機檔案，
所以沒辦法自動載入。預設顯示的是內建測試場景，對應 `Src/testdata.c` 裡
（整段被註解掉的）`set_test_data()`。

## 跟 C 原始碼的對應

網頁版是**重寫**，不是移植，但刻意讓兩邊對得起來 —— C 那份等於是規格書。
JS 這邊的欄位名稱沿用 C 的寫法，函式上面標了對應的原始碼行號。

| 網頁版 | C 原始碼 |
| --- | --- |
| `block()` / `wire()` / `ioNode()` | `Src/vpl.h` 的 `BlockNode` / `wList` / `IONODE` |
| `drawDiagram()` | `Src/block.c:687` `DrawDiagram()` |
| `drawWires()` / `turnLine()` | `Src/block.c:111` `DrawWires()`、`block.c:95` `CaculateTurnLine()` |
| `absOrigin()` | `Src/block.c:166-189`，沿 `SelfBlockLLHead` 累加出絕對座標 |
| `icon()` | `Src/misc.c:84` `DrawIcon()` |
| `hitTest()` | `Src/misc.c:477` `IsInArea()`（簡化版） |
| `parseSave()` | `Src/load.c:359` `parse_loop()` |
| `pdbToText()` | `Src/load.c:733` `LOAD()` 的 `FileOpen`／`FileRead` |

C 版本裡 `Src/linklist.c` 那 836 行在手動維護 `PREV`/`NEXT` 雙向鏈結串列，
JS 直接用陣列和物件參照，所以那一整層不需要重寫。

## 圖從哪裡來

74 張 block 圖示以 base64 data URI 內嵌在 `vpl.html` 裡（約 21 KB），
所以不需要外部檔案。

來源是 **`Starter.prc`**，不是 `.rsrc`：`Src/starter.rsrc` 在轉檔到 Windows
時 resource fork 已經遺失（0 bytes），而編譯好的 PRC 裡有完整的 74 個 `Tbmp`，
格式也單純得多。全部都是未壓縮的 version 1 bitmap，多數 1bpp，
只有 `LAMP`、`LAMPLIGHT`、`STOPUNUSE` 是 2bpp 灰階。

資源 ID 對應的名字取自 `Src/StarterRsc.h`，也就是 C 程式裡 `bap->BITMAPID`
用的那些常數，所以 JS 這邊可以直接寫 `B.FORLOOPBitmap`。

只有在 `Starter.prc` 的圖有變動時才需要重跑（會就地更新 `vpl.html` 裡
`<<SPRITES>>` 標記之間的那段）：

```
python web/tools/extract_bitmaps.py
python web/tools/extract_bitmaps.py --png /tmp/out   # 另外輸出 PNG 檔來看
```

## 一個渲染上的細節

Palm 的 bitmap 是**不透明**的（白底黑點），所以畫布底色必須是白的，
否則每張圖都會拖著一塊白方框。LCD 的綠色調是用 CSS `mix-blend-mode: multiply`
疊在 canvas 上層 —— 白變綠、黑仍然是黑。

另外 `WinDrawLine` 畫的是 1 像素無反鋸齒的線，canvas 要畫出一樣的效果
得把座標對到像素中心（`+0.5`），否則線會糊成兩像素的灰。

## 存檔格式

`.pdb` 裡面是**純 ASCII**，不是二進位 struct dump —— `Src/save.c` 的
`DataStruct2ASCII()` 一行關鍵字、一行值地印出來，位置固定：

```
STARTBLOCK
  NEWNODE
    ID / <n>          TYPE / <n>
    NEWBAP
      BITMAPID / <n>  XY / <x> / <y>   SIZE / <x> / <y>
      [NEWIONODEP   (ADDIONODE   ID TYPE BYTES VALUE TL SIZE)* ]
      [NEWCTRLNODEP (ADDCTRLNODE ID TL SIZE)* ]
    ENDBAP
    [NEWPAP  … ENDPAP]
    [NEWLOOPHOOK NEWHOOK <遞迴>]        ← LOOPBLOCK，只有一個 hook
    [NEWCASEHOOK (NEWHOOK <遞迴>)*]     ← CASEBLOCK，一串 hook
    [FILENAME / <name>]
  ENDNODE
  …
ENDBLOCK
STARTWIRE
  (STARTBID / <n>  STARTNID / <n>  ENDBID / <n>  ENDNID / <n>)*
ENDWIRE
```

外層是 Palm FileStream 的 PDB 容器（type `save`、creator `wjfx`）。
FileStream 在記錄裡還有自己的表頭，格式沒有公開文件，所以讀檔器是直接
掃描 `STARTBLOCK` 取出內文 —— 不管中間夾了什麼表頭都讀得到。

### 三個要注意的地方

1. **線段的 block ID `1` 是特例。** `Src/load.c:676` 把它解讀成最外層的
   `HOOKBLOCK`，也就是這個存檔被當成自訂元件時的外框。畫面上不顯示它，
   接到它身上的線段也不畫（`Src/block.c:132` 的「跳過暗線段」）。

2. **switch case 的分頁名稱在存檔裡是遺失的。** `DataStruct2ASCII()` 寫
   case hook 的時候沒有寫 `hook->name`，所以原本的 `"true"` / `"false"`
   救不回來，只能用 `case 0` / `case 1` 代替。

3. **原版的 loader 根本沒有實作 `NEWCASEHOOK`。** `Src/load.c:569` 那裡
   留了一句「5-26 未完成」，讀到就直接跳過 —— 也就是說當年 switch case
   裡面的東西，存檔存得進去、載入卻會整個不見。寫檔那邊的格式是完整的，
   所以這個讀檔器把它補上了。

### 驗證程度

`web/samples/demo.pdb` 是用 `tools/make_sample_pdb.py` 照 `save.c` 的格式
產生的，內容跟內建測試場景一樣，載進去會畫出同一張圖（round-trip 測試）。

**但是還沒有拿真正由 Palm 寫出來的檔案驗證過** —— repo 裡沒有留下任何當年
存的 `.pdb`。如果你手上找得到，值得拿來試，特別是 FileStream 容器那一層。

## 下一步

- [ ] 拿真正的 Palm `.pdb` 驗證讀檔器
- [ ] 編輯：拖曳元件、拉線（`Src/block.c` 的 `BlockpenDownProcess` / `BlockpenMoveProcess`）
- [ ] 執行引擎（`Src/run.c`：dirty bit 傳播、`NotReady`、巢狀 forloop）
- [ ] 元件面板與工具列（`Src/panel.c`、`Src/functions.c`）
