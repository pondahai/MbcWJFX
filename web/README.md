# web/ — 網頁版

把 Palm OS 版的 MbcWJFX 改寫成瀏覽器版本。**單一 HTML 檔，打開就能跑，沒有 build step。**

```
web/
├── vpl.html                  ← 用瀏覽器直接開這個檔
├── README.md
├── PLAN.md                   ← 移植進度表（112 個函式逐一對照）
├── samples/
│   └── demo.pdb              ← 測試用存檔
└── tools/
    ├── extract_bitmaps.py    ← 從 Starter.prc 抽圖的一次性工具
    └── make_sample_pdb.py    ← 產生上面那個測試存檔
```

## 現況

目前可以讀 Palm 的 `.pdb` 存檔、把 block diagram 畫在 160×160 的畫布上、
點選元件、看場景結構，**能執行**（執行／單步／停止，附即時的節點狀態表和
線段動畫），有**前面板**（Block / Panel 兩個分頁，面板上的控制鈕按得動），
也**能編輯** —— 從元件面板拿元件、拖曳、接線、剪線、刪除。

選「空白（自己畫）」場景就可以從零做一支程式。

排版和行為對照的是 repo 根目錄那幾張當年的截圖：`1.png` 是方塊圖、
`mbcwjfx.jpg` 是前面板、`2.png` 是元件面板（還沒做）。

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
| `drawPanel()` | `Src/panel.c:239` `DrawPanel()` |
| `valueDisplay()` | `Src/panel.c:108` `ValueDisplay()` |
| `changeControl()` | `Src/panel.c:367` `ChangeControl()` |
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

## 手機

畫面倍率會自動配合視窗寬度：2× 以上取整數倍讓像素工整，不足 2× 時允許小數
優先填滿。375px 寬的手機會得到 2×（320px），撐滿整個寬度，比例最接近當年
握著 PDA 的樣子。倍率滑桿可以手動覆蓋，按「自動」回到自動。

本機要跑起來看的話：

```
python -m http.server 8777 --directory web
```

然後用同一個網路的手機開 `http://<電腦IP>:8777/vpl.html`。
（`.claude/launch.json` 裡有同一份設定。）

## 一個渲染上的細節

Palm 的 bitmap 是**不透明**的（白底黑點），所以畫布底色必須是白的，
否則每張圖都會拖著一塊白方框。LCD 的綠色調是用 CSS `mix-blend-mode: multiply`
疊在 canvas 上層 —— 白變綠、黑仍然是黑。

另外 `WinDrawLine` 畫的是 1 像素無反鋸齒的線，canvas 要畫出一樣的效果
得把座標對到像素中心（`+0.5`），否則線會糊成兩像素的灰。

## 元件面板

工具列 x=120 那個按鈕打開的就是 `2.png` 那個 3×3 選單。

原版的面板內容是執行時從一個叫 `func` 的 pdb 資料庫讀進來的
（`Src/functions.c:274`），**那個檔案沒有留下來**。但同一個檔案裡有一段被
註解掉的 `functxt` 字串常數，內容就是那個資料庫的原文 —— 9 個分類、每個分類
底下的元件、連 pap 和控制點都在裡面。`tools/extract_palette.py` 直接從註解裡
把它挖出來，不用手抄。

抽出來是 9 個分類、19 個元件：

| 分類 | 內容 |
| --- | --- |
| FUNCARITH 算術 | ADD, SUB, MUL, DIV, INDICATOR8BLOCK, CTRLU8BLOCK |
| FUNCLOGIC 邏輯 | AND, OR, NOT, LOGICINDICATOR, LOGICCTRL |
| FUNCSTRUCT 結構 | FORLOOP, WHILELOOP, SWITCHCASE |
| FUNCCOMP 比較 | ABOVE, LESS, EQU, ABOVEEQU, LESSEQU |
| FUNCSTR 字串 | *(空)* |
| FUNCCUST 自訂 | *(空)* |
| FUNCARRAY 陣列 | *(空)* |
| FUNCTIME 時間 | *(空)* |
| FUNCFILE 檔案 | *(空)* |

**9 個分類裡有 5 個是空的** —— 字串、自訂、陣列、時間、檔案這些圖示都畫好了、
分類也建好了，但底下一個元件都沒有。當年做到一半。

彈出視窗的幾何來自表單定義（`Src/StarterRsc.h:34`）：`FunctionsForm` 是
(50,1) 100×157、`BlockToolsForm` 是 (118,1) 40×109。格線排法照
`functions.c:333` `DrawToolIcon()`：x 依序 1/34/67，每滿三個換行、y 加 35。

挑一個元件之後由 `addToSysHook()` 實例化（`Src/functions.c:545`），
IO 節點的配置全部照原始碼，例如運算元件是兩個 8×8 的輸入疊在左半邊、
一個 8×16 的輸出佔右半邊；for 迴圈會自動長出 N 和 I 兩個預設元件，
而且它們的值是 **4 bytes 的 long**（`functions.c:843`），跟其他節點的 2 bytes 不同。

## 編輯工具

工具列最右邊那格打開工具選單，順序照 `Src/block.c:3044` 的 PageUp 循環：

| 工具 | 行為 |
| --- | --- |
| HAND 手 | 拖曳元件。y 被夾在 18 以下，拖不到工具列上面（`Src/misc.c:800`） |
| THREAD 線 | 點兩個 IO 節點接線 |
| SCISSOR 剪刀 | 點線段剪掉 |
| KILL 刪除 | 刪元件，接在它身上的線一併拆掉（`Src/block.c:1693` `BreakWireConnection`） |
| DRAG 拖拉 | 尚未實作（改變結構元件大小用的） |

線段要放進哪個 hook 是靠 `scopeFor()` 取兩端**共同的父層**決定的：迴圈裡的
兩個元件接線會落在迴圈自己的 `wires`，不是頂層。原版是靠 `SelfBlockLLHead`
判斷所屬範圍，跨層的情況另外由 `Src/block.c:1206` `CrossWire()` 處理 ——
那個還沒移植。

## 工具列

那排圖示是**畫在 160×160 螢幕裡面**的，不是外掛的介面。位置寫死在
`Src/block.c:1166`（面板版在 `Src/panel.c:340`）：

| x | 大小 | 內容 |
| --- | --- | --- |
| 40 | 16×16 | 一直跑 `RUNRUN` / `RUNRUNWAIT` |
| 60 | 16×16 | 執行 `RUN` / `PAUSE` / `RUNWAIT` |
| 80 | 16×16 | 停止 `STOP` / `STOPUNUSE` / `STOPWAIT` |
| 100 | 16×16 | 動畫開關 `LAMPLIGHT` / `LAMP` |
| 120 | 16×16 | 元件面板（用 `ADDBitmap` 當圖示） |
| 140 | 18×18 | 目前工具 `BlockPointerState`（面板版在 142） |

狀態變數的定義 `Src/mbcwjfx.c:63` 連註解都寫好了：

- `STOP`：`0=無用 1=可用 2=停止`
- `RUN`：`0=靜止 1=執行 2=暫停`

按鈕行為照 `Src/block.c:1958`：執行鈕是 靜止→執行→暫停→執行 的循環；
停止鈕**只有 `STOP==1` 時才有作用**；一直跑打開的同時就會開始執行。

兩個發現：

1. **`STOP==2` 是死狀態。** `STOPBitmap` 畫得出來，但整份原始碼裡沒有
   任何地方把 `STOP` 設成 2。
2. **`#define offset_y -16`（`Src/vpl.h:10`）就是為了這排工具列。**
   `CaculateNowXNowY()`（`Src/misc.c:798`）把筆點的 y 夾在 18 以下
   （`PointIconYExt`），所以元件不能被拖到工具列上面去。

原版還有兩個鍵盤快捷鍵：PageUp 循環工具（HAND → DRAG → THREAD → SCISSOR
→ KILL），PageDown 切換 Block / Panel 兩個 form（`Src/block.c:3044`）。
工具那些要等編輯功能做了才有意義。

## 前面板

這是 LabVIEW 的 front panel 概念：同一個元件有兩種外觀 —— `bap` 畫在方塊圖上
（「程式」那一面），`pap` 畫在前面板上（「使用者介面」那一面）。**點畫面左上角的標題頁籤就會在兩者之間切換** —— 判定區是 `(0,0)` 到
`33x15`，見 `Src/block.c:2008`（面板那邊是 `Src/panel.c:493`）。原版還可以
用 PageDown 鍵切換（`block.c:3067`）。

面板元件的樣子直接取自資源檔：`CTRLU8Bitmap`（32×16）是點陣邊框的方框加上
右側的上下箭頭，`INDICATOR8Bitmap` 是同樣的方框但沒有箭頭。值畫在
`pap.XY + (3,3)`，元件 ID 畫在 `pap.XY.y - 11`（`Src/panel.c:279`）。

按鈕的判定區來自 `Src/functions.c:641`：右邊 4px 寬的直條切成上下兩半，
`CONTROLUP` 是 `TL(28,0) EXT(4,8)`、`CONTROLDOWN` 是 `TL(28,8)`。
`SWON`/`SWOFF`（16×32 的搖頭開關）和 `LEDON`/`LEDOFF` 是靠換圖來表示狀態的。

兩個容易誤會的地方：

1. **面板座標是絕對值，不跟著巢狀位移。** `DrawPanel()` 遞迴進迴圈的時候
   並沒有加上父元件的位移（`Src/panel.c:257`），所以迴圈裡的控制鈕在面板上
   是獨立擺放的 —— 跟方塊圖那邊完全不同的規則。
2. **switch case 不上面板**（`Src/panel.c:255` 直接 break）。

還沒做的是那個數字鍵盤（`DrawDecimalIntKeyboard`，`Src/misc.c:854`），
原版可以點一下控制鈕直接打字輸入數值。

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

內建的「資料流（可執行）」場景：上半是 5 + 3 送進顯示器，下半是一個 for
迴圈，**計數上限 N 從迴圈外面餵進去、計數值 I 再送到迴圈外面的顯示器** ——
兩條線都跨越了結構邊界，由 `crossWire()` 在迴圈邊框上長出接點。
`count=3` 時 9 個 tick 跑完，改成 6 就變 15 個 tick。

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

### 「暗線段」是什麼

`Src/block.c:131` 的 `// 跳過暗線段` 常被誤會成接線提示，其實不是。

暗線段是最外層 `HOOKBLOCK` 的接腳連到內部控制鈕／顯示器的那些線段，
存檔時才產生（`save.c:600`）。它們**故意不畫** —— 因為在頂層時 `HOOKBLOCK`
那個外框本身也不顯示，這些線只有在這個檔被別的圖當成自訂元件載入時才有意義。

接線時真正的提示是另一回事：拖的過程中筆下的 IO 節點會被畫成實心方塊反白
（`block.c:2597` → `misc.c:368` `CheckOnNode(Draw=true)`）。

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
產生的，涵蓋了 HOOKBLOCK 外框、巢狀迴圈、switch case 兩頁、pap 面板外觀
和線段，載進去畫得出來也跑得動。

**但是還沒有拿真正由 Palm 寫出來的檔案驗證過** —— repo 裡沒有留下任何當年
存的 `.pdb`。如果你手上找得到，值得拿來試，特別是 FileStream 容器那一層。

## 下一步

完整的移植進度表在 **[PLAN.md](PLAN.md)** —— 112 個函式逐一標了狀態，
包含 4 個「行為跟原版不一樣」的已知缺陷。下面是摘要：

- [ ] 拿真正的 Palm `.pdb` 驗證讀檔器
- [ ] 編輯：拖曳元件、拉線（`Src/block.c` 的 `BlockpenDownProcess` / `BlockpenMoveProcess`）
- [ ] 跨層接線（`Src/block.c:1206` `CrossWire()`）
- [ ] DRAG 工具：改變結構元件的大小
- [ ] 存檔（寫出 `.pdb`，格式已經知道了）
- [ ] 自訂元件的執行（`DoRun_HOOKBLOCK`）
- [ ] 面板上的數字鍵盤（`DrawDecimalIntKeyboard`）（`Src/panel.c`、`Src/functions.c`）
