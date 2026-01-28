#ifndef LAYER_H
#define LAYER_H

/// @file layer.h
/// @brief レイヤー（画像バッファ）構造体と操作

#include "rgba/rgba.h"
#include <stdbool.h>
#include <stddef.h>

// ============================================================================
// 型定義
// ============================================================================

/// @brief 2D座標を表す構造体
typedef struct
{
	int x; ///< X座標
	int y; ///< Y座標
} Point;

/// @brief サイズを表す構造体
typedef struct
{
	size_t w; ///< 幅
	size_t h; ///< 高さ
} Size;

/// @brief Layer 構造体（不透明型）
typedef struct Layer Layer;

/// @brief Layer ハンドル型（Layer* の別名）
typedef Layer* HLayer;

/// @brief ピクセル合成関数の型
/// @param[in] dst 背景色
/// @param[in] src 前景色
/// @return 合成結果
typedef RGBA (*BlendFunc)(RGBA dst, RGBA src);

// ============================================================================
// ヘルパーマクロ
// ============================================================================

/// @brief Point を生成するマクロ
#define POINT(x_, y_) ((Point){.x = (x_), .y = (y_)})

/// @brief Size を生成するマクロ
#define SIZE(w_, h_) ((Size){.w = (w_), .h = (h_)})

// ============================================================================
// ライフサイクル関数
// ============================================================================

/// @brief 新しいレイヤーを作成する
/// @param[in] size サイズ（幅・高さ）
/// @return 作成されたレイヤーのハンドル、失敗時は NULL
/// @post 戻り値が NULL でない場合、layer_destroy で解放が必要
HLayer layer_create(Size size);

/// @brief レイヤーを破棄する
/// @param[in] layer 破棄するレイヤー（NULL の場合は何もしない）
void layer_destroy(HLayer layer);

/// @brief レイヤーを複製する
/// @param[in] layer 複製元レイヤー
/// @return 複製されたレイヤー、失敗時は NULL
/// @note layer が NULL の場合は NULL を返す
HLayer layer_clone(HLayer layer);

/// @brief 2つのレイヤーの内容を入れ替える
/// @param[in,out] a レイヤーA
/// @param[in,out] b レイヤーB
/// @pre a, b は NULL であってはならない
void layer_swap(HLayer a, HLayer b);

/// @brief 1D RGBA配列からレイヤーを作成する（データをコピー）
/// @param[in] size サイズ
/// @param[in] data RGBA配列（row-major）
/// @return 作成されたレイヤー、失敗時は NULL
HLayer layer_from_rgba_1d(Size size, const RGBA* data);

/// @brief 1D RGB配列からレイヤーを作成する（α=1.0、データをコピー）
/// @param[in] size サイズ
/// @param[in] data RGB配列（row-major）
/// @return 作成されたレイヤー、失敗時は NULL
HLayer layer_from_rgb_1d(Size size, const RGB* data);

/// @brief 2D RGBA配列からレイヤーを作成する（データをコピー）
/// @param[in] size サイズ
/// @param[in] data RGBA配列（data[y][x]）
/// @return 作成されたレイヤー、失敗時は NULL
HLayer layer_from_rgba_2d(Size size, const RGBA* const* data);

/// @brief 2D RGB配列からレイヤーを作成する（α=1.0、データをコピー）
/// @param[in] size サイズ
/// @param[in] data RGB配列（data[y][x]）
/// @return 作成されたレイヤー、失敗時は NULL
HLayer layer_from_rgb_2d(Size size, const RGB* const* data);

// ============================================================================
// 情報取得関数
// ============================================================================

/// @brief レイヤーの幅を取得する
/// @param[in] layer レイヤー
/// @return 幅（ピクセル）
/// @pre layer は NULL であってはならない
size_t layer_get_width(HLayer layer);

/// @brief レイヤーの高さを取得する
/// @param[in] layer レイヤー
/// @return 高さ（ピクセル）
/// @pre layer は NULL であってはならない
size_t layer_get_height(HLayer layer);

/// @brief レイヤーのサイズを取得する
/// @param[in] layer レイヤー
/// @return サイズ
/// @pre layer は NULL であってはならない
Size layer_get_size(HLayer layer);

// ============================================================================
// ピクセル操作関数
// ============================================================================

/// @brief 指定座標のピクセル色を取得する
/// @param[in] layer レイヤー
/// @param[in] p 座標
/// @return ピクセルの色（範囲外の場合は透明）
/// @pre layer は NULL であってはならない
RGBA layer_get_pixel(HLayer layer, Point p);

/// @brief 指定座標のピクセル色を設定する
/// @param[in] layer レイヤー
/// @param[in] p 座標
/// @param[in] color 設定する色
/// @pre layer は NULL であってはならない
void layer_set_pixel(HLayer layer, Point p, RGBA color);

/// @brief レイヤー全体を指定色で塗りつぶす
/// @param[in] layer レイヤー
/// @param[in] color 塗りつぶし色
/// @pre layer は NULL であってはならない
void layer_fill(HLayer layer, RGBA color);

// ============================================================================
// 幾何図形描画関数（アンチエイリアスなし）
// ============================================================================

/// @brief 長方形を塗りつぶし描画する
/// @param[in,out] layer 描画先レイヤー
/// @param[in] pos 左上座標
/// @param[in] size サイズ
/// @param[in] color 描画色
void layer_draw_rect(HLayer layer, Point pos, Size size, RGBA color);

/// @brief 円を塗りつぶし描画する
/// @param[in,out] layer 描画先レイヤー
/// @param[in] center 中心座標
/// @param[in] radius 半径
/// @param[in] color 描画色
void layer_draw_circle(HLayer layer, Point center, size_t radius, RGBA color);

/// @brief 線を描画する（長方形ポリゴン方式）
/// @param[in,out] layer 描画先レイヤー
/// @param[in] p1 始点
/// @param[in] p2 終点
/// @param[in] thickness 太さ
/// @param[in] color 描画色
void layer_draw_line(HLayer layer, Point p1, Point p2, size_t thickness, RGBA color);

/// @brief 三角形を塗りつぶし描画する
/// @param[in,out] layer 描画先レイヤー
/// @param[in] p1 頂点1
/// @param[in] p2 頂点2
/// @param[in] p3 頂点3
/// @param[in] color 描画色
void layer_draw_triangle(HLayer layer, Point p1, Point p2, Point p3, RGBA color);

/// @brief 多角形を塗りつぶし描画する
/// @param[in,out] layer 描画先レイヤー
/// @param[in] points 頂点配列
/// @param[in] n 頂点数
/// @param[in] color 描画色
void layer_draw_polygon(HLayer layer, const Point* points, size_t n, RGBA color);


// ============================================================================
// 幾何図形描画関数（アンチエイリアスあり - Wu アルゴリズム）
// ============================================================================

/// @brief 線を AA 描画する（長方形ポリゴン方式 + Edge Function AA）
/// @param[in,out] layer 描画先レイヤー
/// @param[in] p1 始点
/// @param[in] p2 終点
/// @param[in] thickness 太さ
/// @param[in] color 描画色
void layer_draw_line_aa(HLayer layer, Point p1, Point p2, size_t thickness, RGBA color);

/// @brief 長方形を AA 描画する（境界カバレッジ計算）
/// @param[in,out] layer 描画先レイヤー
/// @param[in] pos 左上座標
/// @param[in] size サイズ
/// @param[in] color 描画色
void layer_draw_rect_aa(HLayer layer, Point pos, Size size, RGBA color);

/// @brief 円を AA 描画する（Xiaolin Wu 円アルゴリズム）
/// @param[in,out] layer 描画先レイヤー
/// @param[in] center 中心座標
/// @param[in] radius 半径
/// @param[in] color 描画色
void layer_draw_circle_aa(HLayer layer, Point center, size_t radius, RGBA color);

/// @brief 三角形を AA 描画する（各辺を Wu 線で描画）
/// @param[in,out] layer 描画先レイヤー
/// @param[in] p1 頂点1
/// @param[in] p2 頂点2
/// @param[in] p3 頂点3
/// @param[in] color 描画色
void layer_draw_triangle_aa(HLayer layer, Point p1, Point p2, Point p3, RGBA color);

/// @brief 多角形を AA 描画する（各辺を Wu 線で描画）
/// @param[in,out] layer 描画先レイヤー
/// @param[in] points 頂点配列
/// @param[in] n 頂点数
/// @param[in] color 描画色
void layer_draw_polygon_aa(HLayer layer, const Point* points, size_t n, RGBA color);


// ============================================================================
// 変形関数（破壊的変更 - layer 自体を変更）
// ============================================================================

/// @brief レイヤーを切り抜く（破壊的）
/// @param[in,out] layer 操作対象レイヤー
/// @param[in] pos 切り抜き開始位置（左上）
/// @param[in] size 切り抜きサイズ
void layer_clip(HLayer layer, Point pos, Size size);

/// @brief レイヤーをリサイズする（破壊的、Nearest Neighbor補間）
/// @param[in,out] layer 操作対象レイヤー
/// @param[in] new_size 新しいサイズ
void layer_resize_nearest(HLayer layer, Size new_size);

/// @brief レイヤーをリサイズする（破壊的、Bilinear補間）
/// @param[in,out] layer 操作対象レイヤー
/// @param[in] new_size 新しいサイズ
void layer_resize_bilinear(HLayer layer, Size new_size);

/// @brief レイヤーを水平反転する（破壊的）
/// @param[in,out] layer 操作対象レイヤー
void layer_flip_horizontal(HLayer layer);

/// @brief レイヤーを垂直反転する（破壊的）
/// @param[in,out] layer 操作対象レイヤー
void layer_flip_vertical(HLayer layer);

/// @brief レイヤーを 2x ダウンサンプルする（破壊的）
/// @param[in,out] layer 操作対象レイヤー
void layer_downsample_2x(HLayer layer);

// ============================================================================
// 変形関数（新規レイヤー作成 - 元を変更しない）
// ============================================================================

/// @brief レイヤーを切り抜いた新しいレイヤーを作成する
/// @param[in] layer 元レイヤー
/// @param[in] pos 切り抜き開始位置（左上）
/// @param[in] size 切り抜きサイズ
/// @return 新しいレイヤー、失敗時は NULL
HLayer layer_clip_to(HLayer layer, Point pos, Size size);

/// @brief レイヤーをリサイズした新しいレイヤーを作成する（Nearest Neighbor補間）
/// @param[in] layer 元レイヤー
/// @param[in] new_size 新しいサイズ
/// @return 新しいレイヤー、失敗時は NULL
HLayer layer_resize_nearest_to(HLayer layer, Size new_size);

/// @brief レイヤーをリサイズした新しいレイヤーを作成する（Bilinear補間）
/// @param[in] layer 元レイヤー
/// @param[in] new_size 新しいサイズ
/// @return 新しいレイヤー、失敗時は NULL
HLayer layer_resize_bilinear_to(HLayer layer, Size new_size);

/// @brief レイヤーを水平反転した新しいレイヤーを作成する
/// @param[in] layer 元レイヤー
/// @return 新しいレイヤー、失敗時は NULL
HLayer layer_flip_horizontal_to(HLayer layer);

/// @brief レイヤーを垂直反転した新しいレイヤーを作成する
/// @param[in] layer 元レイヤー
/// @return 新しいレイヤー、失敗時は NULL
HLayer layer_flip_vertical_to(HLayer layer);

/// @brief レイヤーを 2x ダウンサンプルした新しいレイヤーを作成する
/// @param[in] layer 元レイヤー
/// @return 新しいレイヤー、失敗時は NULL
HLayer layer_downsample_2x_to(HLayer layer);

// ============================================================================
// レイヤー合成関数
// ============================================================================

/// @brief 他のレイヤーを合成する
/// @param[in] dst 合成先レイヤー
/// @param[in] src 合成元レイヤー
/// @param[in] pos 合成位置
/// @param[in] blend 合成関数（NULL の場合はデフォルトのアルファブレンド）
/// @pre dst は NULL であってはならない
/// @pre src は NULL であってはならない
void layer_composite(HLayer dst, HLayer src, Point pos, BlendFunc blend);

// ============================================================================
// ファイル入出力関数
// ============================================================================

/// @brief レイヤーを PPM P3（テキスト）形式で保存する
/// @param[in] layer 保存するレイヤー
/// @param[in] filename ファイル名（パスを含む）
/// @return 成功した場合は true、失敗した場合は false
bool layer_save_p3(HLayer layer, const char* filename);

/// @brief レイヤーを PPM P6（バイナリ）形式で保存する
/// @param[in] layer 保存するレイヤー
/// @param[in] filename ファイル名（パスを含む）
/// @return 成功した場合は true、失敗した場合は false
bool layer_save_p6(HLayer layer, const char* filename);

/// @brief PPM P3（テキスト）形式からレイヤーを読み込む
/// @param[in] filename ファイル名（パスを含む）
/// @return 作成されたレイヤー、失敗時は NULL
HLayer layer_load_p3(const char* filename);

/// @brief PPM P6（バイナリ）形式からレイヤーを読み込む
/// @param[in] filename ファイル名（パスを含む）
/// @return 作成されたレイヤー、失敗時は NULL
HLayer layer_load_p6(const char* filename);

#endif // LAYER_H
