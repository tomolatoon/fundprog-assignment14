# ==============================================================================
# Layer ライブラリ CMake設定
# ==============================================================================

# ソースファイルを自動検出
file(GLOB_RECURSE LAYER_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# ヘッダーファイルも自動検出（IDE用）
file(GLOB_RECURSE LAYER_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

add_library(layer_lib STATIC
    ${LAYER_SOURCES}
    ${LAYER_HEADERS}
)

# 名前空間付きエイリアスを作成
add_library(${PROJECT_NAME}::layer_lib ALIAS layer_lib)

# ヘッダーファイルのパスを設定
target_include_directories(layer_lib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# rgba_lib と libm に依存
target_link_libraries(layer_lib
    PUBLIC
    ${PROJECT_NAME}::rgba_lib
    m
)

# 厳格な警告オプションを適用
set_strict_warnings(layer_lib)

# ------------------------------------------------------------------------------
# テスト設定
# ------------------------------------------------------------------------------
file(GLOB_RECURSE LAYER_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c"
)

if(LAYER_TEST_SOURCES)
    add_executable(test_layer ${LAYER_TEST_SOURCES})
    target_link_libraries(test_layer PRIVATE
        layer_lib
        unity
        m
    )
    target_compile_definitions(test_layer PRIVATE UNITY_INCLUDE_DOUBLE)
    set_basic_warnings(test_layer)
    add_test(NAME test_layer COMMAND test_layer)
endif()
