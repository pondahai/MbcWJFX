# 移植進度表

把 Palm 版的 112 個函式逐一掃過，標出網頁版的狀態。這份表是**先掃描再實作**
的補課 —— 前面幾步是深度優先做下去的，所以漏了 panel 和工具列這種整塊的東西。

圖例：✅ 完成 ｜ ⚠️ 部分 ｜ ❌ 未做 ｜ ➖ 不適用（Palm 平台專屬）

---

## block.c（3161 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `DrawWires` | ✅ | `drawWires()` |
| `DrawRunPointOnWire` | ✅ | `stepRunPoints()` / `drawRunPoints()` |
| `DrawDiagram` | ⚠️ | `HOOKBLOCK` 那個 case 沒做 |
| `DrawCUSTOMBLOCKLattice` | ❌ | 自訂元件的格子外觀 |
| `CrossWire` | ❌ | 跨層接線 |
| `StartStopAddIntoWLL` | ⚠️ | `addWire()` 是簡化版，用共同父層決定歸屬 |
| `CheckWireLink` | ⚠️ | 擋掉了自我連線；其餘合法性檢查沒做 |
| `BreakWireConnection` | ✅ | 併在 `removeBlock()` 裡 |
| `ChangeLinkList` | ❌ | 把元件拖進／拖出結構元件 |
| `PenDownInBlockToolArea` | ✅ | `toolbarHit()` |
| `BlockpenDownProcess` | ⚠️ | HAND／THREAD／SCISSOR／KILL／DRAG 做了；PENCIL 沒做 |
| `BlockpenMoveProcess` | ✅ | 拖曳元件、拉大小、拉線 |
| `BlockDiagramFormDoCommand` | ❌ | 下拉選單 |

## panel.c（1035 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `ValueDisplay` / `ValueErase` | ✅ | |
| `DrawPanel` | ✅ | 含數字鍵盤 |
| `DrawRUNPanel` | ➖ | 原版為了省重畫而分出來的，網頁版整張重畫 |
| `ChangeControl` | ✅ | |
| `PenDownInPanelToolArea` | ✅ | |

## functions.c（1053 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `set_function_palette` / `new_node` | ✅ | `tools/extract_palette.py` 從註解裡抽出來 |
| `DrawToolIcon` / `DrawToolCatagory` | ✅ | `drawPopup()` |
| `AddToSYSHOOK` | ✅ | `addToSysHook()` |
| `FunctionsFormHandleEvent` | ✅ | |
| `FunctionspenUp/Down/MoveProcess` | ➖ | Palm 的事件分派 |
| `FindInputNode` / `FindOutputNode` | ❌ | 存成自訂元件時自動長出 IO 點 |
| `AddCUSTtoSYSHOOK` | ❌ | 把自訂元件放進圖裡 |

## run.c（1692 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `DoRun` | ✅ | |
| `DoRun_WireRun` | ✅ | |
| `DoRun_BlockRun` | ✅ | |
| `DoRun_LOOPBLOCK` | ✅ | 含 IO 點角色對調 |
| `DoItemRUN` | ⚠️ | 算術／邏輯／比較都有；`SWITCHCASE` 原版就是空的 |
| `CheckConnection` | ✅ | |
| `ResetWireStatus` | ✅ | |
| `MoveNodeData` | ✅ | |
| `FindNodeByID` | ✅ | |
| `DoRun_HOOKBLOCK` | ❌ | 自訂元件的執行 |
| `FindWireNodeByIONode` | ❌ | 只有 `DoRun_HOOKBLOCK` 在用 |
| `DoWireRun` / `DoBlockRun` | ➖ | 原始碼裡整段被註解掉，是舊版遺留 |

## misc.c（1024 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `DrawIcon` | ✅ | `icon()` |
| `CaculateNowXNowY` | ✅ | 只取了 y 夾在 18 以下這一段 |
| `GetNodeData` | ✅ | |
| `IsInArea` | ✅ | `hitTest()` |
| `CheckOnCTRLNode` | ✅ | `ctrlHit()`，面板與方塊圖都有 |
| `DrawDecimalIntKeyboard` | ✅ | `drawKeyboard()` |
| `EraseDecimalIntKeyboard` | ➖ | 網頁版整張重畫 |
| `KeyboardPendownProcess` | ✅ | `keyboardHit()` |
| `AutoFindIOnodesSetIntoICONnode` | ❌ | 自訂元件用 |
| `IsTheSameWireLLHead` | ❌ | 自訂元件用 |
| `NestReDraw` / `NestItemMoveToLastPosition` | ➖ | 為了省重畫；網頁版整張重畫 |
| `GetObjectPtr` / `MainFormInit` | ➖ | Palm 表單管線 |

## load.c（974 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `parse_loop` / `LOAD` | ✅ | `parseSave()`，而且補了原版沒做完的 `NEWCASEHOOK` |
| `read_a_str` | ✅ | |
| `FindNodeByID` / `FindIONodeByID` | ✅ | |
| `ProcessCUSTOMLoad` | ❌ | 載入自訂元件 |
| `LOADFormHandleEvent` / `ConvertFileName2RecordIndex` | ❌ | 檔案清單介面 |

## save.c（632 行）— **整個檔案都還沒做**

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `DataStruct2ASCII` | ❌ | 格式已經完全弄懂了，反過來寫就行 |
| `SAVE` | ❌ | |
| `FindInputNode` / `FindOutputNode` | ❌ | 存檔時在 HOOKBLOCK 上長出 IO 點和暗線段 |
| `CountInputNode` / `CountOutputNode` | ❌ | |
| `RemoveDarkWire` | ❌ | |
| `SAVEFormHandleEvent` | ❌ | 檔名輸入介面 |

## linklist.c（836 行）

| 函式 | 狀態 | 備註 |
| --- | --- | --- |
| `allocate_*` / `InsertInto*` / `Remove*` | ➖ | JS 用陣列和物件參照，整層不需要 |
| `set_block_tools_palette` / `set_panel_tools_palette` | ✅ | `BLOCK_TOOLS` / `PANEL_TOOLS` |
| `ItemMoveToLastPosition` | ❌ | 點選時把元件移到最上層 |
| `CountIONODEs` | ✅ | |

## 其他

| 檔案 | 狀態 | 備註 |
| --- | --- | --- |
| `testdata.c` | ✅ | 重建成內建場景 |
| `blocktools.c` / `paneltools.c` / `tools.c` | ✅ | 工具選單 |
| `db.c` | ➖ | Palm 資料庫管線 |
| `mbcwjfx.c` | ➖ | 應用程式生命週期與事件迴圈 |
| `about.c` | ❌ | 關於畫面（有 `dahaiBitmap`） |

---

## 已知缺陷（行為跟原版不一樣）

1. ~~接線可以連到同一個元件的兩個節點~~ — 已修（`block.c:1593`）
2. ~~接線的手勢不同~~ — 已改成拖曳。原版 `BlockpenMoveProcess` 裡沒有
   THREAD 的處理，所以拖的過程**本來就沒有預覽線**，這點也一致
3. ~~switch case 不能翻頁~~ — 已修。翻頁在 **DRAG 工具**底下，不是 HAND
   （`block.c:2151` 那個 case 才是，我原本分類錯了）
4. ~~執行中還可以編輯~~ — 已修（`stopForEdit()`，`block.c:2049`）

## 建議順序

1. ~~4 個已知缺陷~~ ✅
2. ~~數字鍵盤~~ ✅
3. ~~DRAG 工具（改大小 + 翻頁）~~ ✅
4. **存檔** — 做完編輯循環才閉合，格式已經完全知道了
5. `ChangeLinkList` — 把元件拖進／拖出結構元件
6. 自訂元件一整套（`HOOKBLOCK`）：`DoRun_HOOKBLOCK`、`AddCUSTtoSYSHOOK`、
   `ProcessCUSTOMLoad`、`DrawCUSTOMBLOCKLattice`、存檔時的暗線段
7. `CrossWire` 跨層接線
8. 選單、關於畫面
9. `ItemMoveToLastPosition` — 點選時把元件移到最上層
