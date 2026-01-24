# layer_lib - レイヤー操作ライブラリ

画像バッファ（レイヤー）を操作するためのライブラリです。`HLayer` というハンドル型を通じて操作します。

## ヘッダファイル

詳細な API は以下のヘッダファイルを参照してください: 

📄 [`libs/layer/include/layer/layer.h`](../../libs/layer/include/layer/layer.h)

---

## 設計概要

### ハンドル型パターン

`HLayer` は `Layer*` の typedef です。構造体の内部を隠蔽し、関数を通じてのみ操作させることで次の利点があります。

- **カプセル化を実現**: 内部実装を変更しても利用者のコードに影響しない
- **取り回しがよい**: ただのポインタなので値渡ししても問題なく扱いやすい

### 座標系

- 原点は **左上**
- X軸は右方向、Y軸は下方向
- ピクセル座標は **0-indexed**

### 実装状況

| 機能 | 状態 |
|------|------|
| create / destroy | ✅ 実装済み |
| get_width / get_height | ✅ 実装済み |
| get_pixel / set_pixel | ✅ 実装済み |
| fill | ✅ 実装済み |
| draw_rect / draw_circle | 🚧 スタブ |
| composite | 🚧 スタブ |
| save_ppm | 🚧 スタブ |
| rotate | 🚧 スタブ |

---

## 使用例

```c
#include "layer/layer.h"

// 640x480 のレイヤーを作成
HLayer layer = layer_create(640, 480);

// 白で塗りつぶす
layer_fill(layer, rgba_new(1.0, 1.0, 1.0, 1.0));

// 特定のピクセルを赤に設定
layer_set_pixel(layer, 100, 100, rgba_new(1.0, 0.0, 0.0, 1.0));

// 使用後は破棄（今回は動き続けるプログラムでも無いのでしなくても大した問題にならないかもしれないが）
layer_destroy(layer);
```

---

## 関連

- [rgba_lib](rgba.md) - 色操作（layer_lib が依存）
