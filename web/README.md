# web/ — 網頁版

把 Palm OS 版的 MbcWJFX 改寫成瀏覽器版本。**單一 HTML 檔，打開就能跑，沒有 build step。**

```
web/
├── vpl.html                  ← 用瀏覽器直接開這個檔
├── README.md
└── tools/
    └── extract_bitmaps.py    ← 從 Starter.prc 抽圖的一次性工具
```

## 現況

目前是**唯讀檢視器**：可以把一張 block diagram 畫在 160×160 的畫布上、
點選元件、看場景結構。還不能編輯、不能執行、不能讀舊存檔。

畫面上的測試場景對應 `Src/testdata.c` 裡的 `set_test_data()`
（那個函式在原始碼裡整段被註解掉，這裡把它重建起來當第一個渲染目標，
所以還不需要先解 `.pdb` 存檔格式）。

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

## 下一步

- [ ] 讀 Palm `.pdb` 存檔（`Src/load.c` / `Src/save.c`），才能開舊作品
- [ ] 編輯：拖曳元件、拉線（`Src/block.c` 的 `BlockpenDownProcess` / `BlockpenMoveProcess`）
- [ ] 執行引擎（`Src/run.c`：dirty bit 傳播、`NotReady`、巢狀 forloop）
- [ ] 元件面板與工具列（`Src/panel.c`、`Src/functions.c`）
