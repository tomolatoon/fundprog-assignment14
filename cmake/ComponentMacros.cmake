# ==============================================================================
# コンポーネント制作者ディレクトリ作成用マクロ
# ==============================================================================

# ------------------------------------------------------------------------------
# 動画コンポーネントライブラリを作成するマクロ
# 使用法: add_video_component(name)
# - src/*.c を自動検出してビルド対象に
# - include/ をヘッダー検索パスに追加
# - test/*.c を自動検出してテスト実行ファイルを生成
# - libs_all に自動的に依存
# ------------------------------------------------------------------------------
function(add_video_component name)
    # ソースファイルを自動検出
    file(GLOB_RECURSE COMPONENT_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    )

    # ヘッダーファイルも自動検出（IDE用）
    file(GLOB_RECURSE COMPONENT_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    )

    add_library(${name}_lib STATIC
        ${COMPONENT_SOURCES}
        ${COMPONENT_HEADERS}
    )

    add_library(${PROJECT_NAME}::${name}_lib ALIAS ${name}_lib)

    target_include_directories(${name}_lib
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )

    # 基礎ライブラリ統合ターゲットに依存
    target_link_libraries(${name}_lib
        PUBLIC
        ${PROJECT_NAME}::libs_all
    )

    set_strict_warnings(${name}_lib)

    # テストファイル (test_*.c) -> 単一のテスト実行ファイル
    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/test/test_*.c"
    )

    if(TEST_SOURCES)
        add_executable(test_${name} ${TEST_SOURCES})
        target_link_libraries(test_${name} PRIVATE
            ${name}_lib
            unity
            m
        )
        set_basic_warnings(test_${name})
        add_test(NAME test_${name} COMMAND test_${name})
    endif()

    # 例ファイル (example_*.c) -> 個別の実行ファイル
    file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/test/example_*.c"
    )

    foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
        get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
        add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
        target_link_libraries(${EXAMPLE_NAME} PRIVATE ${name}_lib m)
        set_basic_warnings(${EXAMPLE_NAME})
    endforeach()
endfunction()
