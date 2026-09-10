# MbcWJFX

2003 年寫在 Palm OS 上的視覺化程式語言（LabVIEW 那一路的資料流語言），
現在把它改寫成瀏覽器版本。

- `Src/` —— 原始的 C 程式碼，**唯讀的規格書**，不要改它
- `web/` —— 網頁版，單一 HTML 檔
- 線上版：https://pondahai.github.io/MbcWJFX/

## 先讀這兩份

| 檔案 | 內容 |
| --- | --- |
| `web/PLAN.md` | **進度的唯一來源**。112 個函式逐一標了狀態，含已知缺陷和刻意的介面差異 |
| `web/README.md` | 跟 C 原始碼的對照表、存檔格式、執行模型、跨邊界資料流 |

要接續工作就從 `web/PLAN.md` 的「建議順序」往下做。

## 工作方式

**先掃描再實作。** 這條是用教訓換來的：前期是深度優先做下去的，結果整塊
的功能（前面板、畫面裡的工具列）都漏掉，靠使用者提醒才發現。動任何新
功能之前，先把相關的 C 函式整個讀完再開始。`web/PLAN.md` 就是補這門課
的產物。

**搜尋要搜完。** 曾經因為只掃了函式的前 9000 個字元，就斷定
`BlockpenMoveProcess` 裡沒有 THREAD 的處理、對使用者說「原版沒有接線提示」
—— 是錯的，那段在 `block.c:2597`。不確定就把整個函式印出來看。

**每個移植的東西都標出處。** JS 這邊的函式上面寫對應的 C 檔名和行號
（例如 `// run.c:1043 DoRun_LOOPBLOCK`），欄位名也沿用 C 的寫法
（`bap`、`BITMAPID`、`IONodeLLHead`、`TL`/`EXT`）。之後查證會快很多。

**分辨三種「沒做」。** `web/PLAN.md` 用 ❌ 未做、⚠️ 部分、➖ 不適用。
還有第四種要特別標出來：**原版自己就沒做完**（例如 `DoItemRUN` 裡
`SWITCHCASE` 是空的、`load.c:569` 的 `NEWCASEHOOK` 寫著「5-26 未完成」、
`AutoFindIOnodesSetIntoICONnode` 是空函式）。不要把這些當成自己的缺口。

**改完要在瀏覽器裡實測再 commit。** 起本機伺服器：

```
python -m http.server 8778
```

然後開 `http://localhost:8778/web/vpl.html`。注意 `file://` 開的話瀏覽器
不准 fetch 本機檔案，量版面寬度也會失準。

**commit 訊息寫清楚為什麼**，包含對應的 C 原始碼行號、以及任何跟原版
不一樣的地方和理由。

## `web/vpl.html` 的注意事項

單一檔案、沒有 build step、打開就能跑。裡面有兩段是工具產生的，
**不要手改**：

| 標記 | 產生工具 | 來源 |
| --- | --- | --- |
| `<<SPRITES>>` | `web/tools/extract_bitmaps.py` | `Starter.prc` 裡的 74 個 Tbmp |
| `<<PALETTE>>` | `web/tools/extract_palette.py` | `Src/functions.c` 裡被註解掉的 `functxt` 字串 |

兩個工具都是就地改寫 `vpl.html` 的那一段，所以 `vpl.html` 永遠是完整
可執行的、也完整 commit 進 repo。只有在來源變動時才需要重跑。

其他容易踩到的：

- **Palm 的 bitmap 是不透明白底黑點**，所以畫布底色必須是白的，LCD 綠色調
  用 CSS `mix-blend-mode: multiply` 疊上去
- **`WinDrawLine` 是 1 像素無反鋸齒**，canvas 要 `+0.5` 對到像素中心
- **`WinDrawRectangle` 是實心的**，`WinEraseRectangle` 才是清成白色
- **面板座標是絕對值**，不跟著巢狀位移（`panel.c:257`）；方塊圖那邊相反，
  要沿父層一路累加（`block.c:166`）
- **`INodeNUM`/`ONodeNUM` 為 0 時會被設成 -1**，代表「根本沒有那種節點」，
  跟「有但已經算完」是兩回事

## 資源檔

`Src/starter.rsrc` 是 0 bytes —— resource fork 在轉到 Windows 時掉了。
圖都從編譯好的 `Starter.prc` 抽。資源 ID 對應的名字取自 `Src/StarterRsc.h`，
注意同一個 ID 可能出現兩次：真正的圖片常數以 `Bitmap` 結尾，表單物件是
`BitMap`（大寫 M）。挑錯會讓 `B.XXXBitmap` 變成 `undefined`。

## 二進位檔

`.gitattributes` 裡的 `* text=auto` 會對二進位檔做換行正規化並弄壞它們。
`.prc` `.rsrc` `.tmp` `.psym` `.mcp` `.stg` `.tdt` `.pdb` 都已經標成
`binary`。新增別種二進位檔時記得一起加。

## Git

- 預設分支是 `master`
- `gh` 裝在 `C:\Program Files\GitHub CLI\gh.exe`（新裝的，舊終端機的 PATH
  可能還看不到）
- GitHub Pages 從 `master` 的根目錄發佈，`index.html` 會導向 `web/vpl.html`
