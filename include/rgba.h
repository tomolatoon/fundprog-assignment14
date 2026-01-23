#ifndef RGBA_H
#define RGBA_H

/// @file rgba.h
/// @brief RGB/RGBA カラー構造体と操作

/// @brief RGB カラー構造体 (0.0 ~ 1.0 の範囲)
typedef struct RGB
{
	double r; ///< 赤成分
	double g; ///< 緑成分
	double b; ///< 青成分
} RGB;

/// @brief RGBA カラー構造体 (0.0 ~ 1.0 の範囲)
/// Alpha: 0.0 = 完全透明, 1.0 = 完全不透明
typedef struct RGBA
{
	RGB    rgb; ///< RGB成分
	double a;   ///< アルファ成分
} RGBA;

// ============== RGB 関数 ==============

/// @brief 新しい RGB カラーを作成する
RGB rgb_new(double r, double g, double b);

/// @brief 2つの RGB カラーを加算する
RGB rgb_add(RGB a, RGB b);

/// @brief RGB カラーをスカラー倍する
RGB rgb_mul(RGB rgb, double scalar);

/// @brief RGB カラーをスカラーで除算する
RGB rgb_div(RGB rgb, double scalar);

/// @brief 2つの RGB カラー間を線形補間する
/// @param from 開始色
/// @param to 終了色
/// @param t 補間係数 (0.0 ~ 1.0)
RGB rgb_lerp(RGB from, RGB to, double t);

// ============== RGBA 関数 ==============

/// @brief 新しい RGBA カラーを作成する
RGBA rgba_new(double r, double g, double b, double a);

/// @brief 指定したアルファ値を持つ RGBA カラーを RGB から作成する
RGBA rgba_from_rgb(RGB rgb, double a);

/// @brief アルファブレンド: src を dst の上に合成する
/// @param dst 背景色 (Destination)
/// @param src 前景色 (Source)
/// @return 合成結果
RGBA rgba_blend(RGBA dst, RGBA src);

/// @brief 2つの RGBA カラー間を線形補間する
/// @param from 開始色
/// @param to 終了色
/// @param t 補間係数 (0.0 ~ 1.0)
RGBA rgba_lerp(RGBA from, RGBA to, double t);

/// @brief RGB成分にアルファ値を乗算した RGB カラーを返す
/// @param rgba 変換元の RGBA
/// @return アルファ乗算済みの RGB
RGB rgba_apply_alpha(RGBA rgba);

// ============== 配列変換関数 ==============

/// @brief RGB カラーを配列に変換する
/// @param rgb 変換元の RGB
/// @param out_array 変換結果を格納する配列 (サイズ3以上)
/// @pre out_array は NULL であってはならない
void rgb_to_array(RGB rgb, double* out_array);

/// @brief 配列から RGB カラーを作成する
/// @param array RGB成分を含む配列 (サイズ3以上)
/// @return 作成された RGB
/// @pre array は NULL であってはならない
RGB rgb_from_array(const double* array);

/// @brief RGBA カラーを配列に変換する
/// @param rgba 変換元の RGBA
/// @param out_array 変換結果を格納する配列 (サイズ4以上)
/// @pre out_array は NULL であってはならない
void rgba_to_array(RGBA rgba, double* out_array);

/// @brief 配列から RGBA カラーを作成する
/// @param array RGBA成分を含む配列 (サイズ4以上)
/// @return 作成された RGBA
/// @pre array は NULL であってはならない
RGBA rgba_from_array(const double* array);

#endif // RGBA_H
