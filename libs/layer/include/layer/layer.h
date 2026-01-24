#ifndef LAYER_H
#define LAYER_H

/// @file layer.h
/// @brief レイヤー（画像バッファ）構造体と操作

#include "rgba/rgba.h"
#include <stdbool.h>
#include <stddef.h>

// ============== 型定義 ==============

/// @brief Layer 構造体（不透明型）
typedef struct Layer Layer;

/// @brief Layer ハンドル型（Layer* の別名）
typedef Layer* HLayer;

/// @brief ピクセル合成関数の型
/// @param[in] dst 背景色
/// @param[in] src 前景色
/// @return 合成結果
typedef RGBA (*BlendFunc)(RGBA dst, RGBA src);

// ============== ライフサイクル関数 ==============

/// @brief 新しいレイヤーを作成する
/// @param[in] width 幅（ピクセル）
/// @param[in] height 高さ（ピクセル）
/// @return 作成されたレイヤーのハンドル、失敗時は NULL
/// @post 戻り値が NULL でない場合、layer_destroy で解放が必要
HLayer layer_create(size_t width, size_t height);

/// @brief レイヤーを破棄する
/// @param[in] layer 破棄するレイヤー（NULL の場合は何もしない）
void layer_destroy(HLayer layer);

// ============== 情報取得関数 ==============

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

// ============== ピクセル操作関数 ==============

/// @brief 指定座標のピクセル色を取得する
/// @param[in] layer レイヤー
/// @param[in] x X座標
/// @param[in] y Y座標
/// @return ピクセルの色（範囲外の場合は透明）
/// @pre layer は NULL であってはならない
RGBA layer_get_pixel(HLayer layer, size_t x, size_t y);

/// @brief 指定座標のピクセル色を設定する
/// @param[in] layer レイヤー
/// @param[in] x X座標
/// @param[in] y Y座標
/// @param[in] color 設定する色
/// @pre layer は NULL であってはならない
void layer_set_pixel(HLayer layer, size_t x, size_t y, RGBA color);

/// @brief レイヤー全体を指定色で塗りつぶす
/// @param[in] layer レイヤー
/// @param[in] color 塗りつぶし色
/// @pre layer は NULL であってはならない
void layer_fill(HLayer layer, RGBA color);

// ============== 幾何図形描画関数（宣言のみ） ==============

/// @brief 長方形を描画する
/// @param[in] layer レイヤー
/// @param[in] x 左上X座標
/// @param[in] y 左上Y座標
/// @param[in] width 幅
/// @param[in] height 高さ
/// @param[in] color 塗りつぶし色
/// @pre layer は NULL であってはならない
void layer_draw_rect(HLayer layer, int x, int y, size_t width, size_t height, RGBA color);

/// @brief 円を描画する
/// @param[in] layer レイヤー
/// @param[in] cx 中心X座標
/// @param[in] cy 中心Y座標
/// @param[in] radius 半径
/// @param[in] color 塗りつぶし色
/// @pre layer は NULL であってはならない
void layer_draw_circle(HLayer layer, int cx, int cy, size_t radius, RGBA color);

// ============== レイヤー合成関数（宣言のみ） ==============

/// @brief 他のレイヤーを合成する
/// @param[in] dst 合成先レイヤー
/// @param[in] src 合成元レイヤー
/// @param[in] x 合成位置X座標
/// @param[in] y 合成位置Y座標
/// @param[in] blend 合成関数（NULL の場合はデフォルトのアルファブレンド）
/// @pre dst は NULL であってはならない
/// @pre src は NULL であってはならない
void layer_composite(HLayer dst, HLayer src, int x, int y, BlendFunc blend);

// ============== ファイル出力関数（宣言のみ） ==============

/// @brief レイヤーをPPM形式でファイルに出力する
/// @param[in] layer レイヤー
/// @param[in] filename 出力ファイル名
/// @return 成功時 true、失敗時 false
/// @pre layer は NULL であってはならない
/// @pre filename は NULL であってはならない
bool layer_save_ppm(HLayer layer, const char* filename);

// ============== 変形関数（宣言のみ） ==============

/// @brief レイヤーを回転した新しいレイヤーを作成する
/// @param[in] layer 元のレイヤー
/// @param[in] angle_degrees 回転角度（度、反時計回り）
/// @return 回転後の新しいレイヤー、失敗時は NULL
/// @pre layer は NULL であってはならない
/// @post 戻り値が NULL でない場合、layer_destroy で解放が必要
HLayer layer_rotate(HLayer layer, double angle_degrees);

#endif // LAYER_H
