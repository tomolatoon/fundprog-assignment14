cmake_minimum_required(VERSION 3.10)

project(fundprog-assignment14
    VERSION 1.0.0
    LANGUAGES C
)

# C99 標準を使用、コンパイラ拡張なし
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 実行ファイルを作成
add_executable(${PROJECT_NAME}
    src/main.c
)

# インクルードディレクトリ
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 安全性重視のコンパイルオプション
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall                # 基本的な警告を有効化
        -Wextra              # 追加の警告を有効化
        -Wpedantic           # ISO C標準への厳格な準拠
        -Werror              # 警告をエラーとして扱う
        -Wshadow             # 変数のシャドウイングを警告
        -Wconversion         # 暗黙の型変換を警告
        -Wsign-conversion    # 符号変換を警告
        -Wnull-dereference   # NULLポインタの参照を警告
        -Wdouble-promotion   # floatからdoubleへの暗黙の昇格を警告
        -Wformat=2           # printf/scanf形式文字列の厳格なチェック
        -Wundef              # 未定義マクロの使用を警告
        -Wstrict-prototypes  # 古いスタイルの関数宣言を警告
        -Wmissing-prototypes # プロトタイプなしの関数定義を警告
    )
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4                  # 高い警告レベル
        /WX                  # 警告をエラーとして扱う
    )
endif()
