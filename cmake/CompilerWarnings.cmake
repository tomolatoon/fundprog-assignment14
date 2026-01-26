# ==============================================================================
# コンパイラ警告設定モジュール
# ==============================================================================
#
# 使用方法:
#   include(CompilerWarnings)
#   set_strict_warnings(target_name)   # 厳格な警告（本体用）
#   set_basic_warnings(target_name)    # 基本的な警告（テスト用）
#

# ------------------------------------------------------------------------------
# 厳格な警告オプション（メインコード用）
# - 安全性重視の厳格なチェック
# - 警告をエラーとして扱う
# ------------------------------------------------------------------------------
function(set_strict_warnings target)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE
            -Wall # 基本的な警告を有効化
            -Wextra # 追加の警告を有効化
            -Wpedantic # ISO C標準への厳格な準拠
            -Werror # 警告をエラーとして扱う
            -Wshadow # 変数のシャドウイングを警告
            -Wconversion # 暗黙の型変換を警告
            -Wsign-conversion # 符号変換を警告
            -Wnull-dereference # NULLポインタの参照を警告
            -Wdouble-promotion # floatからdoubleへの暗黙の昇格を警告
            -Wformat=2 # printf/scanf形式文字列の厳格なチェック
            -Wundef # 未定義マクロの使用を警告
            -Wstrict-prototypes # 古いスタイルの関数宣言を警告
            -Wmissing-prototypes # プロトタイプなしの関数定義を警告
        )
    endif()
endfunction()

# ------------------------------------------------------------------------------
# 基本的な警告オプション（テストコード用）
# - 主要な警告のみ有効化
# - 外部ライブラリとの互換性を考慮
# ------------------------------------------------------------------------------
function(set_basic_warnings target)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
        )
    endif()
endfunction()

# ------------------------------------------------------------------------------
# Debugビルド用オプション
# - デバッグ情報の最大化
# - Sanitizerによるメモリエラー検出
# ------------------------------------------------------------------------------
function(set_debug_options target)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE
            $<$<CONFIG:Debug>:-g3> # 最大限のデバッグ情報
            $<$<CONFIG:Debug>:-O0> # 最適化なし
            $<$<CONFIG:Debug>:-fno-omit-frame-pointer> # スタックトレース保持
        )
        
        # Windows(MinGW)ではSanitizerが標準でリンクできない場合があるため除外
        if(NOT WIN32)
            target_compile_options(${target} PRIVATE
                $<$<CONFIG:Debug>:-fsanitize=address,undefined> # Sanitizer有効
            )
            target_link_options(${target} PRIVATE
                $<$<CONFIG:Debug>:-fsanitize=address,undefined>
            )
        endif()
    endif()
endfunction()

# ------------------------------------------------------------------------------
# Releaseビルド用オプション
# - 最大速度最適化
# - リンク時最適化（LTO）
# ------------------------------------------------------------------------------
function(set_release_options target)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE
            $<$<CONFIG:Release>:-O3> # 最大速度最適化
            $<$<CONFIG:Release>:-DNDEBUG> # assertマクロ無効化
            $<$<CONFIG:Release>:-flto> # リンク時最適化
            $<$<CONFIG:Release>:-march=native> # CPU固有最適化
        )
        target_link_options(${target} PRIVATE
            $<$<CONFIG:Release>:-flto>
        )
    endif()
endfunction()
