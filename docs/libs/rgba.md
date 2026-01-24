# rgba_lib - カラー操作ライブラリ

RGB/RGBA カラー構造体と、それらを操作するための関数を提供します。

## ヘッダファイル

詳細な API は以下のヘッダファイルを参照してください: 

📄 [`libs/rgba/include/rgba/rgba.h`](../../libs/rgba/include/rgba/rgba.h)

---

## 設計概要

### 値の範囲

RGB/RGBA の各成分は **0.0〜1.0** の範囲で表現します。これによって次の利点があります。

- 整数型（0〜255）よりも微妙な色の変化を保持できる
- 補間（lerp）やブレンドが自然に書ける

### RGBA のアルファ値

- `0.0` = 完全透明
- `1.0` = 完全不透明

---

## 使用例

```c
#include "rgba/rgba.h"

// 色の作成
RGB red   = rgb_new(1.0, 0.0, 0.0);
RGB green = (RGB) {0.0, 1.0, 0.0};
RGB blue  = {0.0, 0.0, 1.0};

// 半透明の緑
RGBA green = {green, 0.5};

// アルファブレンド
RGBA blended = rgba_blend(red, blue);
```

---

## 関連

- [layer_lib](layer.md) - レイヤー操作（rgba_lib を使用）
