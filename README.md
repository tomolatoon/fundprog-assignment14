# fundprog-assignment14

基礎プログラミング課題14のプロジェクトです。

## 必要要件

- CMake 3.10 以上
- C99対応コンパイラ（GCC, Clang, MSVC など）

## ビルド方法

### Debugビルド（デフォルト）

デバッグ情報付き、AddressSanitizer/UndefinedBehaviorSanitizer有効

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Releaseビルド

速度最適化、LTO有効

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## 実行方法

```bash
./build/fundprog-assignment14
```

## プロジェクト構成

```
fundprog-assignment14/
├── CMakeLists.txt    # CMake設定
├── src/
│   └── main.c        # メインソース
├── include/          # ヘッダーファイル用
├── build/            # ビルド成果物（gitignore済み）
└── README.md
```

## コンパイルオプション

### 共通オプション（安全性重視）

- `-Wall -Wextra -Wpedantic -Werror` - 厳格な警告
- `-Wconversion -Wsign-conversion` - 暗黙の型変換を検出
- その他多数の警告オプション

### Debugビルド

- `-g3 -O0` - 最大限のデバッグ情報、最適化なし
- `-fno-omit-frame-pointer` - スタックトレース保持
- `-fsanitize=address,undefined` - メモリ/未定義動作検出

### Releaseビルド

- `-O3` - 最大速度最適化
- `-flto` - リンク時最適化
- `-march=native` - CPU固有最適化
- `-DNDEBUG` - assertマクロ無効化
