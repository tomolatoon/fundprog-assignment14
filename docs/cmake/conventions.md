# CMake 設計規約

このドキュメントでは、プロジェクトで採用している CMake の設計規約を説明します。

## Modern CMake とは

Modern CMake（CMake 3.0 以降）では、**ターゲットベース** の設計が推奨されています。これは、グローバルな設定ではなく、個々のターゲット（ライブラリ、実行ファイル）に対して設定を行う方式です。

### 避けるべき書き方（旧式）

```cmake
# ❌ グローバルに include ディレクトリを設定
include_directories(${PROJECT_SOURCE_DIR}/include)

# ❌ グローバルにコンパイルオプションを設定
add_compile_options(-Wall -Wextra)
```

### 推奨される書き方（Modern）

```cmake
# ✅ ターゲットごとに設定
target_include_directories(my_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(my_lib PRIVATE -Wall -Wextra)
```

---

## PUBLIC / PRIVATE / INTERFACE

`target_*` コマンドには3つのキーワードがあります: 

| キーワード | 意味 |
|-----------|------|
| **PRIVATE** | このターゲットのビルドにのみ使用 |
| **PUBLIC** | このターゲットと、これに依存するターゲットの両方で使用 |
| **INTERFACE** | これに依存するターゲットのみで使用 |

### 例

```cmake
target_include_directories(rgba_lib
    PUBLIC
        # rgba_lib を使う側も、このディレクトリを参照できる
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        # rgba_lib のビルドにのみ使用
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
```

---

## 名前空間付きエイリアス

ライブラリには名前空間付きのエイリアスを作成しています: 

```cmake
add_library(rgba_lib STATIC src/rgba.c)
add_library(${PROJECT_NAME}::rgba_lib ALIAS rgba_lib)
```

### なぜ必要か

1. **明確な依存関係**: `${PROJECT_NAME}::rgba_lib` と書くことで、外部ライブラリとの区別が明確になる
2. **インストール時の互換性**: `find_package()` で取得したターゲットと同じ形式になる
3. **タイポ検出**: 存在しないエイリアスを指定するとエラーになる

---

## INTERFACE ライブラリ

`libs_all` は **INTERFACE ライブラリ** です。実体（`.a` ファイル）を持たず、他のターゲットへの依存関係をグループ化します。

```cmake
add_library(libs_all INTERFACE)
target_link_libraries(libs_all
    INTERFACE
        ${PROJECT_NAME}::rgba_lib
        ${PROJECT_NAME}::layer_lib
)
```

### 利点

- コンポーネントは `libs_all` だけに依存すればよい
- 基礎ライブラリが増えても、コンポーネントの CMakeLists.txt を変更する必要がない

---

## CONFIGURE_DEPENDS

`file(GLOB)` は通常非推奨ですが、`CONFIGURE_DEPENDS` オプションを使うことで、ファイル追加時に自動で再構成されます。

```cmake
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)
```

### 注意点

- CMake 3.12 以降で使用可能
- ビルドシステムによっては若干のオーバーヘッドがある
- 大規模プロジェクトでは明示的なファイル列挙が推奨される

このプロジェクトでは、開発者の利便性を優先して `CONFIGURE_DEPENDS` を採用しています。

---

## 関連

- [CMake Documentation](https://cmake.org/cmake/help/latest/)
- [Modern CMake (An Introduction)](https://cliutils.gitlab.io/modern-cmake/)
