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

目前可以讀 Palm 的 `.pdb` 存檔、把 block diagram 畫在 160×160 的畫布上、
點選元件、看場景結構，**而且能執行**（執行／單步／停止，附即時的節點狀態表）。
還不能編輯。

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
| `doRun()` | `Src/run.c:1498` `DoRun()` |
| `doRunWireRun()` | `Src/run.c:582` `DoRun_WireRun()` |
| `doRunBlockRun()` | `Src/run.c:1356` `DoRun_BlockRun()` |
| `doRunLoopBlock()` | `Src/run.c:1043` `DoRun_LOOPBLOCK()` |
| `doItemRun()` | `Src/run.c:198` `DoItemRUN()` |
| `checkConnection()` | `Src/run.c:63` `CheckConnection()` |
| `resetWireStatus()` | `Src/run.c:24` `ResetWireStatus()` |
| `tick()` 的驅動迴圈 | `Src/block.c:3096` 的 `nilEvent` 處理 |
| `stepRunPoints()` / `drawRunPoints()` | `Src/block.c:363` `DrawRunPointOnWire()` |

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

## 執行模型

這是**資料流**（dataflow），不是指令流 —— 沒有程式計數器，只有「哪個節點的
資料備妥了」。每個 IO 節點有一個 `NotReady` 旗標，每個 tick 做兩件事：

1. `doRunWireRun()` 把備妥的資料沿線段搬到對面，搬完把對面的 `NotReady`
   關掉，並把接收端元件的 `INodeNUM` 減一。
2. `doRunBlockRun()` 讓輸入到齊的元件計算。

tick 回傳 `handle`，true 代表還有事沒做完。外面的驅動迴圈一直呼叫到它回傳
false 為止 —— 原版是 Palm 的 `nilEvent`（`Src/block.c:3096`），這裡用
`setInterval`。

速度靠 `INodeNUM` / `ONodeNUM` 兩個計數器（原始碼 2003-6-3 那次改版）：
輸入全到齊 = `INodeNUM` 歸零，輸出還有沒算的 = `ONodeNUM` 不為零。
計數為 0 時會被設成 **-1**，代表「這個元件根本沒有那種節點」，
跟「有但已經算完」區分開來。

### 迴圈是怎麼做的

`doRunLoopBlock()` 有一個很漂亮的手法（`Src/run.c:1115`）：迴圈邊框上的
IO 點在進入迴圈內部之前會**整組對調角色** —— 從外面看是輸入的節點，站在
迴圈裡面看就是輸出。遞迴進去跑完再翻回來。

for 迴圈的條件判斷（`Src/run.c:1293`）比較迴圈內第一個元件（N）和第二個
元件（I）的值：不相等就 `I++`、重置內部所有元件和線段的狀態、再跑一圈；
相等就把 I 歸零、迴圈結束。所以**迴圈內部 `blocks[0]` 必須是 N、`blocks[1]`
必須是 I**，這個順序是寫死的。

內建的「資料流（可執行）」場景跑起來是這樣（8 個 tick 結束）：

```
t1  5+3 算出 8                     I=0
t2  8 送達顯示器                   I=1
t4                                 I=2
t6                                 I=3
t8  N==I，I 歸零，迴圈結束 → 完成
```

### 線段動畫（LAMP）

`stepRunPoints()` 對應 `Src/block.c:363` 的 `DrawRunPointOnWire()`。這一段
是整個執行流程最精巧的地方：**動畫和資料搬移是連動的**，不是先算完再放動畫。

執行點每個 tick 沿線走一個像素（三段式：水平→垂直→水平），走到終點就把
`RUNPOINT` 設成 `(-1,-1)`；而 `doRunWireRun()` 只搬「已抵達」的線段
（`Src/run.c:592` 的判斷）。所以資料是真的跟著那個點在跑。搬完之後
`DIRTY` 設為 true，那條線這一輪就不會再動 —— `DIRTY` 就是為了動畫而存在的。

`stepRunPoints()` 回傳「還有點在路上」時，`doRun()` 這一輪就直接結束，
不搬資料也不讓元件運算。

同一張圖開不開動畫，結果一樣，只有 tick 數差很多：

| | ticks | 結果 |
| --- | --- | --- |
| `LAMP=false` | 8 | `ind=8, I=0, 迴圈內顯示器=3` |
| `LAMP=true` | 397 | 同上 |

所以動畫開著的時候要把 tick 率拉高（介面會自動調到 120/s）才看得順。

有一個原版就有的怪癖照抄了：`not_finish` 是所有線段共用的旗標，只要**任何
一條**線的點抵達終點，這一輪就會去搬資料，還在路上的線段下一個 tick 再繼續
走（`Src/block.c:544`）。

### 還沒移植的部分

- `DoRun_HOOKBLOCK()`（`Src/run.c:746`）—— 自訂元件的執行
- switch case 的執行（原版 `DoItemRUN` 裡 `SWITCHCASEBitmap` 就是空的）

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
- [ ] 自訂元件的執行（`DoRun_HOOKBLOCK`）
- [ ] 元件面板與工具列（`Src/panel.c`、`Src/functions.c`）
