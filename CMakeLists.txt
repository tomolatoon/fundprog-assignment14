# ==============================================================================
# プロジェクト設定
# ==============================================================================
cmake_minimum_required(VERSION 3.14)

project(fundprog-video-composer
    VERSION 1.0.0
    LANGUAGES C
    DESCRIPTION "動画コンポーネント作成フレームワーク"
)

# ==============================================================================
# グローバルオプション
# ==============================================================================

# C99 標準を使用、コンパイラ拡張なし
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# clangd 用に compile_commands.json を常に生成
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 出力ディレクトリの設定
# - 実行ファイル -> build/<Config>/bin
# - 静的ライブラリ -> build/<Config>/lib
# - 共有ライブラリ -> build/<Config>/lib
# NOTE: CMakePresets.json で binaryDir を build/<Config>/build に設定しているため、
#       一つ上の階層に出力することで構造を整理する
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

# デフォルトビルドタイプをDebugに設定
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# ==============================================================================
# CMakeモジュール読み込み
# ==============================================================================
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CompilerWarnings)
include(ComponentMacros)

# ==============================================================================
# 外部依存関係
# ==============================================================================

# Unity Framework（テストフレームワーク）
# - バージョンを固定してビルドの再現性を確保
include(FetchContent)
FetchContent_Declare(
    Unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.6.0
)

# Double精度を有効化（FetchContent_MakeAvailable前に設定）
add_compile_definitions(UNITY_INCLUDE_DOUBLE)

FetchContent_MakeAvailable(Unity)

# ==============================================================================
# テスト設定
# - add_subdirectory() より前に呼ぶ必要がある（ベストプラクティス）
# ==============================================================================
enable_testing()

# ==============================================================================
# サブディレクトリ
# ==============================================================================
add_subdirectory(libs)
add_subdirectory(components)
add_subdirectory(app)
