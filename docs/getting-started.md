# 環境構築ガイド

このドキュメントでは、開発環境のセットアップから初回ビルド・テスト実行までの手順を説明します。

## 必要要件

| ツール | バージョン | 用途 |
|--------|-----------|------|
| CMake | 3.14 以上 | ビルドシステム |
| Ninja | 1.10 以上 | ビルドツール (高速化) |
| GCC または Clang | C99 対応 | コンパイラ |
| GDB | 任意 | デバッグ（オプション） |
| ffmpeg | 任意 | GIF 変換（オプション） |

### バージョン確認

```bash
cmake --version   # 3.14 以上であること
gcc --version     # または clang --version
```

---

## リポジトリの取得

```bash
git clone <repository-url>
cd fundprog-video-composer
```

---

## ビルド

### CMake Presets (推奨)

CMake 3.19 以上をお使いの場合は、プリセット機能を使って簡単にビルド構成を行えます。
特に Windows 環境の場合はこのプリセットを使うと GCC が強制されて便利です。

**利用可能なプリセット一覧:**

```bash
cmake --list-presets
```

**Linux (Debug / Release):**

```bash
# Debug
cmake --preset linux-debug
cmake --build build/linux-debug

# Release
cmake --preset linux-release
cmake --build build/linux-release
```

**Windows (MinGW) (Debug / Release):**

```bash
# Debug
cmake --preset windows-debug
cmake --build build/windows-debug

# Release
cmake --preset windows-release
cmake --build build/windows-release
```

### 従来の方法

CMake のバージョンが古い場合は、プリセットを使わずにコマンドライン引数を次のように直接与えてください。

```bash
# Debug
cmake -B build/debug/build -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug/build

# Release
cmake -B build/release/build -DCMAKE_BUILD_TYPE=Release
cmake --build build/release/build
```

また、Ninja が使用できない環境では次のようなコマンドライン引数も同時に与えてください。

```bash
-G "Unix Makefiles"  # Linux
-G "MinGW Makefiles" # Windows (MinGW)
```

### ビルド成果物

ビルドが成功すると、以下のファイルが生成されます：

| ファイル | 説明 |
|---------|------|
| `build/linux-debug/bin/video_composer` | 統合アプリケーション |
| `build/linux-debug/bin/test_rgba` | RGBA ライブラリのテスト |
| `build/linux-debug/lib/lib*.a` | 各ライブラリの静的ライブラリ |
| `build/linux-debug/build/` | 中間ファイル (Ninja ビルド定義など) |

---

## 動作確認

### テスト実行

```bash
ctest --test-dir build/linux-debug/build --output-on-failure
```

成功すると以下のような出力が表示されます:

```
Test project /path/to/build/debug
    Start 1: test_rgba
1/1 Test #1: test_rgba ........................   Passed    0.00 sec

100% tests passed, 0 tests failed out of 1
```

### アプリケーション実行

```bash
./build/linux-debug/bin/video_composer
```

出力例：

```
Video Composer - 動画コンポーネント統合アプリケーション
Done.
```

---

## VS Code でのデバッグ

本プロジェクトには VS Code 用の設定ファイルが含まれています。

### 必要な拡張機能

- **C/C++** (Microsoft)

### 使い方

1. **F5** を押してデバッグ構成を選択
2. 利用可能な構成：
   - **Debug: Video Composer** - メインアプリケーションをデバッグ
   - **Debug: Test RGBA** - RGBA テストをデバッグ

### ビルドタスク

`Ctrl+Shift+B` でビルドタスクを実行できます。

---

## トラブルシューティング

### CMake が見つからない

```bash
sudo apt install cmake  # Ubuntu/Debian
brew install cmake      # macOS
```

### ビルドエラー: コンパイラが見つからない

```bash
sudo apt install build-essential  # Ubuntu/Debian
xcode-select --install            # macOS
```

### clangd の警告が消えない

ビルド後、`compile_commands.json` が生成されます。ビルド後、`.clangd` のパスを適切なフォルダを指定すると警告は解除されます。

```bash
CompileFlags:
  CompilationDatabase: build/linux-debug/build

```
