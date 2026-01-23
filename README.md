# fundprog-assignment14

基礎プログラミングおよび演習の第14回総合課題のプロジェクト

## 必要要件

- CMake 3.10 以上
- GCC または Clang（C99対応）
- GDB（デバッグ用）

## ビルド方法

まずリポジトリのルートディレクトリに移動し，その後次節の指示に従ってください．

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
`./build/debug/` 以下と，`./build/release/` 以下に実行ファイルが生成されるので，それを直接実行します

```bash
# Debug
./build/debug/fundprog-assignment14

# Release
./build/release/fundprog-assignment14
```

## VS Code でのデバッグ

本プロジェクトには VS Code 用のデバッグ設定が含まれています。

### 必要な拡張機能

- C/C++ (Microsoft)

### 使い方

1. F5 を押してデバッグ構成を選択
2. 利用可能な構成：
   - **Debug: Main** - メインプログラムをデバッグビルド＆実行
   - **Release: Main** - メインプログラムをリリースビルド＆実行
   - **Debug: Test** - テストをデバッグビルド＆実行
   - **Release: Test** - テストをリリースビルド＆実行

## プロジェクト構成

```
fundprog-assignment14/
├── CMakeLists.txt        # CMake設定
├── .clangd               # clangd設定（Intellisense用）
├── .vscode/
│   ├── tasks.json        # ビルドタスク
│   └── launch.json       # デバッグ構成
├── include/
│   └── rgba.h            # RGB/RGBA構造体・関数宣言
├── src/
│   ├── main.c            # メインソース
│   ├── rgba.c            # RGB/RGBA実装
│   └── test/
│       └── rgba.c        # Unityテスト
├── build/
│   ├── debug/            # Debugビルド成果物
│   └── release/          # Releaseビルド成果物
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

## テストフレームワーク

[Unity](https://github.com/ThrowTheSwitch/Unity)（v2.6.0）を使用しています。CMake の FetchContent により自動的にダウンロードされます。
