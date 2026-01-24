# ==============================================================================
# ソースディレクトリ CMake設定
# ==============================================================================

# ------------------------------------------------------------------------------
# RGBAライブラリ
# - 再利用可能なSTATICライブラリとして定義
# - テストからもリンク可能
# ------------------------------------------------------------------------------
add_library(rgba_lib STATIC
    rgba.c
)

target_include_directories(rgba_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# 厳格な警告オプションを適用
set_strict_warnings(rgba_lib)

# ------------------------------------------------------------------------------
# メイン実行ファイル
# ------------------------------------------------------------------------------
add_executable(${PROJECT_NAME}
    main.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    rgba_lib
)

# 警告オプションを適用
set_strict_warnings(${PROJECT_NAME})

# Debug/Releaseビルドオプションを適用
set_debug_options(${PROJECT_NAME})
set_release_options(${PROJECT_NAME})
