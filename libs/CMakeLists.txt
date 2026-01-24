# ==============================================================================
# 基礎ライブラリ CMake設定
# ==============================================================================

# 各ライブラリを追加
add_subdirectory(rgba)
add_subdirectory(layer)

# ------------------------------------------------------------------------------
# 基礎ライブラリ統合ターゲット（INTERFACEライブラリ）
# コンポーネントはこれだけに依存すれば全基礎ライブラリが使える
# ------------------------------------------------------------------------------
add_library(libs_all INTERFACE)
add_library(${PROJECT_NAME}::libs_all ALIAS libs_all)

target_link_libraries(libs_all
    INTERFACE
    ${PROJECT_NAME}::rgba_lib
    ${PROJECT_NAME}::layer_lib
)
