# fundprog-assignment14

基礎プログラミング課題14のプロジェクトです。

## 必要要件

- CMake 3.10 以上
- C99対応コンパイラ（GCC, Clang, MSVC など）

## ビルド方法

```bash
# ビルドディレクトリを作成
mkdir -p build
cd build

# CMake設定を生成
cmake ..

# ビルド
cmake --build .
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

安全性重視のコンパイルオプションが有効になっています：

- `-Wall -Wextra -Wpedantic` - 厳格な警告
- `-Werror` - 警告をエラーとして扱う
- `-Wconversion -Wsign-conversion` - 暗黙の型変換を検出
- その他多数の警告オプション
