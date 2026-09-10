# 移植進度表

把 Palm 版的 112 個函式逐一掃過，標出網頁版的狀態。這份表是**先掃描再實作**
的補課 —— 前面幾步是深度優先做下去的，所以漏了 panel 和工具列這種整塊的東西。

圖例：✅ 完成 ｜ ⚠️ 部分 ｜ ❌ 未做 ｜ ➖ 不適用（Palm 平台專屬）
｜ 🚧 **原版自己就沒做完**（不是這次移植的缺口，別當成待辦）

---

## block.c（3161 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `DrawWires` | ✅ | `drawWires()` |
| `DrawRunPointOnWire` | ✅ | `stepRunPoints()` / `drawRunPoints()` |
| `DrawDiagram` | ✅ | |
| `DrawCUSTOMBLOCKLattice` | ✅ | 併在 `drawDiagram()` 的 HOOKBLOCK case |
| `CrossWire` | ✅ | `crossWire()`，結構邊界上自動長節點 |
| `StartStopAddIntoWLL` | ✅ | `addWire()`，三種情況都照 `block.c:1493` 分派 |
| `CheckWireLink` | ✅ | 擋自我連線與輸出接輸出（`block.c:1582`） |
| `BreakWireConnection` | ✅ | 併在 `removeBlock()` 裡 |
| `ChangeLinkList` | ✅ | 拖曳落在結構上就搬進去（`structAt` + `reparent`），拖到空白處回主串列 |
| `PenDownInBlockToolArea` | ✅ | `toolbarHit()`，含點標題切換 form（`block.c:2008`） |
| `BlockpenDownProcess` | ✅ 🚧 | HAND／THREAD／SCISSOR／KILL／DRAG 都做了；PENCIL 在 `block.c:2251` 和 `block.c:2858` 兩段整個被註解掉，原版按了也沒反應 |
| `BlockpenMoveProcess` | ✅ | 拖曳元件、拉大小、拉線 |
| `BlockDiagramFormDoCommand` | ✅ | 下拉選單（File / Works / Run / About），內容取自 `Starter.prc` 的 MBAR 1000 / 1100 |

## panel.c（1035 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `ValueDisplay` / `ValueErase` | ✅ | |
| `DrawPanel` | ✅ | 含數字鍵盤 |
| `DrawRUNPanel` | ➖ | 原版為了省重畫而分出來的，網頁版整張重畫 |
| `ChangeControl` | ✅ | |
| `PenDownInPanelToolArea` | ✅ | |
| `PanelpenDown/Move/UpProcess` | ✅ | 三個工具分開：HAND 搬位置、FINGER 操作控制項、PENCIL 叫鍵盤 |
| `setNewObjXY`（FrontPanelForm 那一支） | ✅ | 面板座標是絕對值，直接寫 `pap.XY`，不扣父層位移 |

## functions.c（1053 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `set_function_palette` / `new_node` | ✅ | `tools/extract_palette.py` 從註解裡抽出來 |
| `DrawToolIcon` / `DrawToolCatagory` | ✅ | `drawPopup()` |
| `AddToSYSHOOK` | ✅ | `addToSysHook()` |
| `FunctionsFormHandleEvent` | ✅ | |
| `FunctionspenUp/Down/MoveProcess` | ➖ | Palm 的事件分派 |
| `FindInputNode` / `FindOutputNode` | ✅ | 存檔時就做掉了（`buildHookNodes()`） |
| `AddCUSTtoSYSHOOK` | ✅ | `addCustomToScene()` |

## run.c（1692 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `DoRun` | ✅ | |
| `DoRun_WireRun` | ✅ | |
| `DoRun_BlockRun` | ✅ | |
| `DoRun_LOOPBLOCK` | ✅ | 含 IO 點角色對調 |
| `DoItemRUN` | ✅ 🚧 | 算術／邏輯／比較都有；`SWITCHCASE` 在 `run.c:145` 和 `run.c:421` 都是空 case |
| `CheckConnection` | ✅ | |
| `ResetWireStatus` | ✅ | |
| `MoveNodeData` | ✅ | |
| `FindNodeByID` | ✅ | |
| `DoRun_HOOKBLOCK` | ✅ | `doRunHookBlock()` |
| `FindWireNodeByIONode` | ✅ | `findWireByNode()` |
| `DoWireRun` / `DoBlockRun` | ➖ | 原始碼裡整段被註解掉，是舊版遺留 |

## misc.c（1024 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `DrawIcon` | ✅ | `icon()` |
| `CaculateNowXNowY` | ✅ | 只取了 y 夾在 18 以下這一段 |
| `GetNodeData` | ✅ | |
| `IsInArea` | ✅ | `hitTest()` |
| `CheckOnCTRLNode` | ✅ | `ctrlHit()`，面板與方塊圖都有 |
| `CheckOnNode` | ✅ | `nodeHit()` + `drawNodeHighlight()` |
| `DrawDecimalIntKeyboard` | ✅ | `drawKeyboard()` |
| `EraseDecimalIntKeyboard` | ➖ | 網頁版整張重畫 |
| `KeyboardPendownProcess` | ✅ | `keyboardHit()` |
| `AutoFindIOnodesSetIntoICONnode` | ➖ | 原始碼裡是**空函式**，從沒實作 |
| `IsTheSameWireLLHead` | ➖ | 死碼：唯一的呼叫點 `block.c:1378` 是註解掉的 |
| `NestReDraw` / `NestItemMoveToLastPosition` | ➖ | 為了省重畫；網頁版整張重畫 |
| `GetObjectPtr` / `MainFormInit` | ➖ | Palm 表單管線 |

## load.c（974 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `parse_loop` / `LOAD` | ✅ | `parseSave()`，而且補了原版沒做完的 `NEWCASEHOOK` |
| `read_a_str` | ✅ | |
| `FindNodeByID` / `FindIONodeByID` | ✅ | |
| `ProcessCUSTOMLoad` | ✅ | `makeCustomBlock()`，改用元件庫而不是檔名查找 |
| `LOADFormHandleEvent` / `BuildLOADMenu` | ✅ | LOADForm 畫在畫面裡（tFRM 1400），LOAD／DELETE／CANCEL 都有，含 Talt 1000 的刪除確認 |
| `ConvertFileName2RecordIndex` | ➖ | 原版拿檔名換 record index；網頁版直接用檔名當 key |

## save.c（632 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `DataStruct2ASCII` | ✅ | `serializeSave()`，往返測試通過 |
| `SAVE` | ✅ | `saveFile()`，下載成 `.pdb` |
| `FindInputNode` / `FindOutputNode` | ✅ | `buildHookNodes()` |
| `CountInputNode` / `CountOutputNode` | ✅ | `countIO()` |
| `AddString` | ➖ | JS 用陣列 join |
| `RemoveDarkWire` | ➖ | 暗線段是存檔時才生出來的，不留在場景裡 |
| `SAVEFormHandleEvent` | ✅ | SAVEForm 畫在畫面裡（tFRM 1600），欄位接實體鍵盤，上限 32 字照資源 |

## linklist.c（836 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `allocate_*` / `InsertInto*` / `Remove*` | ➖ | JS 用陣列和物件參照，整層不需要 |
| `set_block_tools_palette` / `set_panel_tools_palette` | ✅ | `BLOCK_TOOLS` / `PANEL_TOOLS` |
| `ItemMoveToLastPosition` | ✅ | 點選時把元件移到串列最後（最上層），巢狀連上層一起 |
| `CountIONODEs` | ✅ | |

## 其他

| 檔案 | 狀態 | 備註 |
| --- | --- | --- |
| `testdata.c` | ✅ | 重建成內建場景 |
| `blocktools.c` / `paneltools.c` / `tools.c` | ✅ | 工具選單 |
| `db.c` | ➖ | Palm 資料庫管線 |
| `mbcwjfx.c` | ➖ | 應用程式生命週期與事件迴圈 |
| `about.c` | ✅ | 關於畫面，版面照 `StarterRsc.h`、文字取自 tFRM 1200 |

---

## 已知缺陷（行為跟原版不一樣）

1. ~~接線可以連到同一個元件的兩個節點~~ — 已修（`block.c:1593`）
2. ~~接線的手勢不同~~ — 已改成拖曳，補上節點反白**和預覽線**。

   **這裡我原本判斷錯了。** 我先前寫「原版 `BlockpenMoveProcess` 裡沒有
   THREAD 的處理，拖的過程本來就沒有預覽線」—— 錯的，是我搜尋時只掃了函式
   的前半段。`block.c:2597` 有處理：拖的時候會呼叫
   `CheckOnNode(..., Draw=true)`，把筆下的 IO 節點畫成**實心方塊**反白
   （`misc.c:368`，Palm 的 `WinDrawRectangle` 是實心的）。所以原版是有
   接線提示的。

   至於橡皮筋線：現在這版的 `block.c` 裡確實沒有，但舊版的
   `Src/mbcwjfx.c.BAK`（拆檔前的整合檔）裡有寫 —— `DrawDiagram()` 開頭
   有一段從 `WireStart` 的節點畫到 `WireStop` 的節點的 `WinDrawLine`
   （1617-1638 行），而 `CheckOnNode` 會在筆移動時把 `WireStop` 設成筆下的
   節點（1347 行）。也就是說那是一條**會吸附到節點**的預覽線 —— 但整段被
   註解掉了。現在這版接回來了，筆下沒有節點時就跟著筆走。
3. ~~switch case 不能翻頁~~ — 已修。翻頁在 **DRAG 工具**底下，不是 HAND
   （`block.c:2151` 那個 case 才是，我原本分類錯了）
4. ~~執行中還可以編輯~~ — 已修（`stopForEdit()`，`block.c:2049`）

存檔那邊有一個設計差異：原版是把暗線段和 HOOKBLOCK 的 IO 點**真的加進場景裡**
（`save.c:596` 每次存檔前還要先把上一次的清掉，不然會重複），網頁版是存檔的
當下才生出來、不留在場景裡，所以不需要 `RemoveDarkWire`。

## 建議順序

1. ~~4 個已知缺陷~~ ✅
2. ~~數字鍵盤~~ ✅
3. ~~DRAG 工具（改大小 + 翻頁）~~ ✅
4. ~~存檔~~ ✅
5. ~~`ChangeLinkList` — 把元件拖進／拖出結構元件~~ ✅
6. ~~自訂元件一整套（`HOOKBLOCK`）~~ ✅
7. ~~`CrossWire` 跨層接線~~ ✅
8. ~~選單、關於畫面~~ ✅
9. ~~`ItemMoveToLastPosition` — 點選時把元件移到最上層~~ ✅

## 跨越結構邊界的資料流

結構元件（迴圈、switch case、自訂元件）是一道邊界，線不能直接穿過去。
`crossWire()`（`block.c:1206`）做的事是把比較深的那一端**往外提**一層：

1. 在那個結構的邊框上長出一個 5×5 的 IO 節點 —— 方向由哪一端比較深決定
   （資料要出來就是輸出、要進去就是輸入），邊也跟著（出去走右邊、進來走左邊）
2. 在結構自己的 hook 裡補一段線，把新節點接到內部的元件
3. 那一端就變成邊框上的節點，重複到兩端同層，最後把剩下那段接起來

執行時真正讓資料穿過邊界的，是 `DoRun_LOOPBLOCK` / `DoRun_HOOKBLOCK` 的
**IO 點角色對調** —— 同一個節點從外面看是輸入、站在裡面看就是輸出。

所以 for 迴圈的 N 從哪裡餵都可以，不必侷限在迴圈內部。實測：迴圈外的控制鈕
接到迴圈內的 N，一條線會變成邊框節點 + 外面一段 + 裡面一段，10 個 tick 跑完。

## 跟原版的介面差異（刻意的）

- **畫面倍率會自動配合視窗寬度**，手機上撐滿寬度（2× 以上取整數倍讓像素工整，
  不足 2× 時允許小數優先填滿）。原版固定 1×，因為 Palm 螢幕就是 160×160。
- 右邊那一欄（執行控制、場景、節點狀態表）是原版沒有的除錯用介面。
  工具列上的圖示和它們共用同一份狀態，兩邊按都一樣。
- 數字鍵盤旁邊會顯示正在輸入的數字，原版沒有。
- **多一道「卡住」熄火**。`DoRun_HOOKBLOCK`（`run.c:781`）和 `DoRun_LOOPBLOCK`
  在「輸入還沒到齊」時是回傳 `handle=true` 的，所以一張永遠等不到輸入的圖
  會無止盡空轉 —— 原版就是這樣，要使用者自己按 STOP。網頁版連續 64 個 tick
  整個執行狀態都沒變就停下來，並說明多半是哪裡沒接線。
- **接線的「輸出不能接輸出」判斷會換算邊框節點的角色**。結構元件邊框上的
  IO 點在 `run.c:1115` 進去之前會整批翻面，所以「迴圈裡的 `i` 接到邊框上的
  輸出點」是合法的 —— 那正是把值送出迴圈的作法。原版有同樣的判斷式，但它的
  邊框節點只能由 `CrossWire` 自動長出來、使用者碰不到，所以踩不到。
- **存檔區用 `localStorage`**。原版一個存檔就是一部機器上的一個 Palm
  database（`load.c:820`），`BuildLOADMenu` 用 `DmGetNextDatabaseByTypeCreator`
  把 creator `wjfx` 的全部列出來。瀏覽器沒有那種東西，改用 `localStorage`，
  key 是 `mbcwjfx:file:<檔名>`。匯入 `.pdb` 時也會寫進去一份，等同原版
  HotSync 把檔案放進機器裡，這樣 LOADForm 才看得到。
- 右邊那欄的「存成 .pdb」是網頁版另外加的下載出口，原版只存在機器裡。
- **方塊圖的工具列少一個 PENCIL**。原版 `linklist.c:set_block_tools_palette`
  排了六個工具，但 PENCIL 在 `block.c:2251` / `block.c:2858` 的處理整段是
  註解掉的 —— 選了不會有任何反應。與其擺一顆死按鈕，網頁版就不列它。
  （前面板的 PENCIL 是有作用的，那是叫數字鍵盤。）
- Palm 是按硬體的 Menu 鍵叫出下拉選單，網頁沒有那顆鍵，改成右邊那欄的
  「選單（Menu）」按鈕。選單本身還是畫在 160×160 螢幕裡面。
- 關於畫面的 `Last compiled:` 原版印編譯時的 `__DATE__ __TIME__`，
  網頁版改印 `vpl.html` 的 Last-Modified。
