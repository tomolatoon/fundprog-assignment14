cmake_minimum_required(VERSION 3.10)

project(fundprog-assignment14
    VERSION 1.0.0
    LANGUAGES C
)

# C99 標準を使用、コンパイラ拡張なし
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# デフォルトビルドタイプをDebugに設定
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Unity Framework
add_compile_definitions(UNITY_INCLUDE_DOUBLE) # Double精度を有効化
include(FetchContent)
FetchContent_Declare(
    Unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.6.0
)
FetchContent_MakeAvailable(Unity)

# 実行ファイルを作成
add_executable(${PROJECT_NAME}
    src/main.c
    src/rgba.c
)

# テスト用実行ファイル
add_executable(test_rgba
    src/test/rgba.c
    src/rgba.c
)

# インクルードディレクトリ
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(test_rgba PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 数学ライブラリをリンク（test_rgba用）
# Unityの設定：Double精度を有効化
target_compile_definitions(test_rgba PRIVATE UNITY_INCLUDE_DOUBLE)
target_link_libraries(test_rgba PRIVATE m unity)

# コンパイルオプション
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    # 共通の警告オプション（安全性重視）
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
    target_compile_options(test_rgba PRIVATE
        -Wall -Wextra -Wpedantic
    )

    # Debug用オプション
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-g3>                    # 最大限のデバッグ情報
        $<$<CONFIG:Debug>:-O0>                    # 最適化なし
        $<$<CONFIG:Debug>:-fno-omit-frame-pointer> # スタックトレース保持
        $<$<CONFIG:Debug>:-fsanitize=address,undefined> # Sanitizer有効
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address,undefined>
    )

    # Release用オプション
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:-O3>          # 最大速度最適化
        $<$<CONFIG:Release>:-DNDEBUG>     # assertマクロ無効化
        $<$<CONFIG:Release>:-flto>        # リンク時最適化
        $<$<CONFIG:Release>:-march=native> # CPU固有最適化
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:-flto>
    )
endif()

