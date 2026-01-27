# ==============================================================================
# 最終動画作成アプリケーション CMake設定
# ==============================================================================

add_executable(video_composer
    src/main.c
)

target_link_libraries(video_composer PRIVATE
    ${PROJECT_NAME}::h2511186_lib
    ${PROJECT_NAME}::k2511070_lib
    ${PROJECT_NAME}::i2511030_lib
)

set_strict_warnings(video_composer)
set_debug_options(video_composer)
set_release_options(video_composer)
