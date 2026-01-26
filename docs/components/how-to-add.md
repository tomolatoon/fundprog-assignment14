# コンポーネント追加ガイド

このドキュメントでは、新しい開発者がコンポーネントを追加する方法を説明します。

## コンポーネントとは

コンポーネントは、動画の「何を描くか」を担当するモジュールです。各開発者は自分専用のディレクトリで、独自の描画ロジックを実装します。

---

## ディレクトリ構造

```
components/
└── <your-id>/              # 例: h2511186/
    ├── CMakeLists.txt      # ビルド設定
    ├── include/<your-id>/  # ヘッダファイル
    │   └── component.h
    ├── src/                # ソースファイル
    │   └── component.c
    └── test/               # テストファイル（オプション）
        └── test_component.c
```

---

## 新しいコンポーネント制作者ディレクトリの作成

### 1. ディレクトリを作成

```bash
mkdir -p components/<your-id>/include/<your-id>
mkdir -p components/<your-id>/src
mkdir -p components/<your-id>/test
```

### 2. CMakeLists.txt を作成

`components/<your-id>/CMakeLists.txt`:

```cmake
# add_video_component マクロを使用
add_video_component(<your-id>)
```

これだけで、`src/` 内のソースファイルと `include/` 内のヘッダが自動的にビルド対象になります。

### 3. components/CMakeLists.txt に追加

`components/CMakeLists.txt` にサブディレクトリを追加：

```cmake
add_subdirectory(h2511186)
add_subdirectory(k2511070)
add_subdirectory(<your-id>)  # 追加
```

---

## ソースファイルの追加

ディレクトリが作成されたら、ソースファイルを追加するだけでビルド対象になります。

### ヘッダファイル例

`components/<your-id>/include/<your-id>/component.h`:

```c
#ifndef YOUR_ID_COMPONENT_H
#define YOUR_ID_COMPONENT_H

#include "layer/layer.h"

void your_id_component_init(void);
void your_id_component_draw(HLayer layer);

#endif
```

### インクルードガードを忘れずに

ヘッダは依存の仕方によって複数回インクルードされる可能性があるので、インクルードガードを忘れずに用意しましょう。具体的には次のようにします。

```c
#ifndef UNIQUE_NAME_H
#define UNIQUE_NAME_H

// ヘッダの中身を書く

#endif UNIQUE_NAME_H
```

### ソースファイル例

`components/<your-id>/src/component.c`:

```c
#include "<your-id>/component.h"

void your_id_component_init(void)
{
    // 初期化処理
}

void your_id_component_draw(HLayer layer)
{
    // 描画処理
    // 例: layer_fill(layer, rgba_new(1.0, 0.0, 0.0, 1.0));
}
```

---

## GLOB による自動検出

このプロジェクトでは `file(GLOB_RECURSE ... CONFIGURE_DEPENDS)` を使用しているため：

- **ファイルを追加するだけ** で自動的にビルド対象になります
- **ビルド時に自動で検出** されるため、手動で CMakeLists.txt を編集する必要はありません

---

## テストの追加

`test/` ディレクトリにテストファイルを追加すると、自動的にテスト実行ファイルが作成されます。

`components/<your-id>/test/test_component.c`:

```c
#include "<your-id>/component.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

void test_init(void)
{
    your_id_component_init();
    TEST_PASS();
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_init);
    return UNITY_END();
}
```

テストの実行：

```bash
ctest --test-dir build/linux-debug/build -R "test_<your-id>"
```

---

## 関連

- [docs/architecture.md](../architecture.md) - プロジェクト全体の構造
- [docs/libs/layer.md](../libs/layer.md) - 描画に使用するライブラリ
