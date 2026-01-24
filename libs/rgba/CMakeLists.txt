# ==============================================================================
# RGB/RGBA ライブラリ CMake設定
# ==============================================================================

# ソースファイルを自動検出
file(GLOB_RECURSE RGBA_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# ヘッダーファイルも自動検出（IDE用）
file(GLOB_RECURSE RGBA_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
)

add_library(rgba_lib STATIC
    ${RGBA_SOURCES}
    ${RGBA_HEADERS}
)

# 名前空間付きエイリアスを作成（推奨プラクティス）
add_library(${PROJECT_NAME}::rgba_lib ALIAS rgba_lib)

# ヘッダーファイルのパスを設定
target_include_directories(rgba_lib
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 厳格な警告オプションを適用
set_strict_warnings(rgba_lib)

# ------------------------------------------------------------------------------
# テスト設定
# ------------------------------------------------------------------------------
file(GLOB_RECURSE RGBA_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c"
)

if(RGBA_TEST_SOURCES)
    add_executable(test_rgba ${RGBA_TEST_SOURCES})
    target_link_libraries(test_rgba PRIVATE
        rgba_lib
        unity
        m
    )
    target_compile_definitions(test_rgba PRIVATE UNITY_INCLUDE_DOUBLE)
    set_basic_warnings(test_rgba)
    add_test(NAME test_rgba COMMAND test_rgba)
endif()
