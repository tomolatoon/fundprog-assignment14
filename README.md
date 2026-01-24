# fundprog-video-composer

動画コンポーネント作成フレームワーク

## 必要要件

- CMake 3.14 以上
- GCC または Clang（C99対応）
- GDB（デバッグ用）
- ffmpeg（GIF変換用、オプション）

## ビルド方法

### Debugビルド（デフォルト）

```bash
cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug && cmake --build build/debug
```

### Releaseビルド

```bash
cmake -B build/release -DCMAKE_BUILD_TYPE=Release && cmake --build build/release
```

## 実行方法

```bash
# Debug
./build/debug/app/video_composer

# Release
./build/release/app/video_composer
```

## テスト実行

```bash
ctest --test-dir build/debug --output-on-failure
```

## プロジェクト構成

```
fundprog-video-composer/
├── CMakeLists.txt                      # トップレベルCMake設定
├── cmake/
│   ├── CompilerWarnings.cmake          # コンパイラ警告設定
│   └── ComponentMacros.cmake           # コンポーネント作成用マクロ
│
├── libs/                               # 基礎ライブラリ群
│   ├── CMakeLists.txt                  # libs_all 統合ターゲット含む
│   ├── rgba/                           # RGB/RGBA カラーライブラリ
│   │   ├── include/rgba/rgba.h
│   │   ├── src/rgba.c
│   │   └── test/test_rgba.c
│   └── layer/                          # Layer ライブラリ（HLayerハンドル型）
│       ├── include/layer/layer.h
│       ├── src/layer.c
│       └── test/
│
├── components/                         # 動画コンポーネント群
│   ├── h2511186/                       # 開発者 h2511186 用
│   │   ├── include/h2511186/component.h
│   │   ├── src/component.c
│   │   └── test/
│   └── k2511070/                       # 開発者 k2511070 用
│       ├── include/k2511070/component.h
│       ├── src/component.c
│       └── test/
│
├── app/                                # 統合アプリケーション
│   └── src/main.c
│
├── output/                             # GIF出力先
│   └── frames/                         # PPM連番画像
│
└── build/                              # ビルド成果物
    ├── debug/
    └── release/
```

## アーキテクチャ

```
┌─────────────────────────────────────────────────────────┐
│                        app                              │
│                   (video_composer)                      │
└────────────────────────┬────────────────────────────────┘
                         │ 依存
    ┌────────────────────┼────────────────────┐
    ▼                    ▼                    ▼
┌──────────┐       ┌──────────┐       ┌──────────┐
│ h2511186 │       │ k2511070 │       │   ...    │
│   _lib   │       │   _lib   │       │  (拡張)  │
└────┬─────┘       └────┬─────┘       └────┬─────┘
     │                  │                  │
     └──────────────────┼──────────────────┘
                        ▼
              ┌───────────────────┐
              │     libs_all      │  (INTERFACE)
              └────────┬──────────┘
         ┌─────────────┼─────────────┐
         ▼             ▼             ▼
    ┌─────────┐   ┌──────────┐   ┌─────────┐
    │rgba_lib │   │layer_lib │   │  ...    │
    └─────────┘   └──────────┘   └─────────┘
```

## コンポーネント追加方法

このプロジェクトではコンポーネントの制作者毎にディレクトリを分けて管理します。

### コンポーネントディレクトリ構造

```
components/
└── <id>/
    ├── include/
    ├── src/
    └── test/
```

### コンポーネント制作者ディレクトリ作成手順

1. `components/<id>/` ディレクトリを作成
2. `include/<id>/`, `src/`, `test/` サブディレクトリを作成
3. CMakeLists.txt に `add_video_component(<id>)` を記述

### コンポーネント作成手順

1. ソース/ヘッダファイルを追加（GLOB で自動検出されます）

## CMake構成の特徴

Modern CMake で採用される規則に基づいて構成されています。

- **ターゲットベース設計**: `target_*` コマンドのみ使用
- **名前空間付きエイリアス**: `fundprog-video-composer::rgba_lib` 形式
- **libs_all 統合ターゲット**: コンポーネントはこれだけに依存すればOK
- **CONFIGURE_DEPENDS**: ファイル追加時に自動再構成

## テストフレームワーク

[Unity](https://github.com/ThrowTheSwitch/Unity)（v2.6.0）を使用。CMake の FetchContent により自動ダウンロード。
