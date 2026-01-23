# fundprog-assignment14

基礎プログラミングおよび演習の第14回総合課題のプロジェクト

## 必要要件

- CMake 3.10 以上
- GCC, Clang（C99対応）

## ビルド方法

### Debugビルド（デフォルト）

デバッグ情報付き，AddressSanitizer/UndefinedBehaviorSanitizer有効

```bash
cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug && cmake --build build/debug
```

### Releaseビルド

速度最適化，LTO有効

```bash
cmake -B build/release -DCMAKE_BUILD_TYPE=Release && cmake --build build/release
```

### リビルドのみ（2回目以降）

```bash
cmake --build build/debug    # Debug
cmake --build build/release  # Release
```

## 実行方法

プロジェクトをルートとして次のフォルダに実行ファイルが生成されるのでそれを実行してください．

```bash
# Debug
/build/debug

# Release
/build/release
```

## プロジェクト構成

```
fundprog-assignment14/
├── CMakeLists.txt    # CMake設定
├── src/
│   └── main.c        # メインソース
├── include/          # ヘッダーファイル用
├── build/
│   ├── debug/        # Debugビルド成果物
│   └── release/      # Releaseビルド成果物
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
