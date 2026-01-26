# fundprog-assignment14

基礎プログラミング演習の総合課題として、**複数人で動画（GIFアニメーション）を作成するためのフレームワーク**です。

## 特徴

- **複数人での共同開発**: 各開発者が独自の「コンポーネント」を実装し、最終的にそれらを統合して1つの動画を生成します
- **コード衝突の回避**: 開発者ごとにディレクトリが分離し、Git でのマージコンフリクトを最小限に抑えています
- **再利用可能な基礎ライブラリ**: 色操作（RGB/RGBA）やレイヤー描画などの共通機能を、全員が利用できるライブラリとして提供しています

最終的な出力は PPM 連番画像として生成され、ffmpeg を使って GIF アニメーションに変換できます。

---

## クイックスタート

### 必要なもの

- CMake 3.14 以上
- GCC または Clang（C99 対応）
- GDB（デバッグ用、オプション）
- ffmpeg（GIF 変換用、オプション）

### ビルドと実行

```bash
# リポジトリをクローン
git clone https://github.com/tomolatoon/fundprog-assignment14.git
cd fundprog-assignment14

# 構成: 推奨: プリセットを使用 (CMake 3.19+)
cmake --preset linux-debug  # Linux
cmake --preset windows-debug  # Windows (MinGW)

# ビルド
cmake --build build/ilnux-debug  # Linux
cmake --build build/windows-debug  # Windows (MinGW)

# 実行
./build/debug/app/video_composer  # Linux
./build/windows-debug/app/video_composer.exe  # Windows

# テスト実行
ctest --test-dir build/debug --output-on-failure
```

詳しい環境構築手順は [docs/getting-started.md](docs/getting-started.md) を参照してください。

---

## プロジェクト構成

このプロジェクトは **3層構造** になっています：

```
┌─────────────────────────────────────────┐
│           app (video_composer)          │  ← 最終的な動画を生成
└─────────────────────┬───────────────────┘
                      │ 利用
      ┌───────────────┴───────────────┐
      ▼                               ▼
┌───────────┐                   ┌───────────┐
│ h2511186  │                   │ k2511070  │  ← 各開発者のコンポーネント
│   _lib    │                   │   _lib    │
└─────┬─────┘                   └─────┬─────┘
      │                               │
      └───────────────┬───────────────┘
                      ▼
              ┌─────────────┐
              │  libs_all   │  ← 基盤ライブラリをまとめて提供
              └──────┬──────┘
           ┌─────────┼─────────┐
           ▼         ▼         ▼
      ┌────────┐ ┌─────────┐ ┌────────┐
      │rgba_lib│ │layer_lib│ │  ...   │  ← 基盤ライブラリ
      └────────┘ └─────────┘ └────────┘
```

### ディレクトリ構造

| ディレクトリ | 説明 |
|-------------|------|
| `libs/` | 基盤ライブラリ |
| `components/` | 開発者ごとのコンポーネント |
| `app/` | 統合アプリケーション（動画を生成） |
| `cmake/` | CMake ヘルパーモジュール |
| `docs/` | ドキュメント |
| `output/` | 出力先（PPM 画像、GIF など） |

詳しいアーキテクチャは [docs/architecture.md](docs/architecture.md) を参照してください。

---

## 基盤ライブラリ

コンポーネント開発で使用できる基盤ライブラリです。

| ライブラリ | 説明 | ドキュメント |
|-----------|------|-------------|
| `rgba_lib` | RGB/RGBA カラー操作 | [docs/libs/rgba.md](docs/libs/rgba.md) |
| `layer_lib` | レイヤー（画像バッファ）操作 | [docs/libs/layer.md](docs/libs/layer.md) |

## コンポーネント開発者向け

各開発者は `components/<your-id>/` ディレクトリで作業します。ファイルを追加するだけで自動的にビルド対象になります。

### 始め方

1. 自分のディレクトリを作成（すでに存在する場合はスキップ）
2. `src/` にソースファイルを追加
3. `include/<your-id>/` にヘッダファイルを追加
4. ビルドすると自動的に検出されます

詳しい手順は [docs/components/how-to-add.md](docs/components/how-to-add.md) を参照してください。

---

## ドキュメント一覧

| ドキュメント | 内容 |
|-------------|------|
| [docs/getting-started.md](docs/getting-started.md) | 環境構築から初回ビルドまで |
| [docs/architecture.md](docs/architecture.md) | プロジェクト全体のアーキテクチャ |
| [docs/libs/rgba.md](docs/libs/rgba.md) | rgba_lib の設計と使い方 |
| [docs/libs/layer.md](docs/libs/layer.md) | layer_lib の設計と使い方 |
| [docs/components/how-to-add.md](docs/components/how-to-add.md) | コンポーネント追加方法 |
| [docs/cmake/conventions.md](docs/cmake/conventions.md) | CMake 設計規約 |

---

## テストフレームワーク

[Unity](https://github.com/ThrowTheSwitch/Unity)（v2.6.0）を使用しています。CMake の FetchContent により自動的にダウンロードされるため、事前のインストールは不要です。
