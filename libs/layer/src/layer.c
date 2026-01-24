/// @file layer.c
/// @brief Layer ライブラリの実装

#include "layer/layer.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// 内部構造体定義
// ============================================================================

/// @brief Layer 構造体の実体
struct Layer
{
	size_t width;  ///< 幅（ピクセル）
	size_t height; ///< 高さ（ピクセル）
	RGBA*  pixels; ///< ピクセルデータ（row-major, 左上原点）
};

// ============================================================================
// 内部ヘルパー関数
// ============================================================================

/// @brief 座標がレイヤー内かどうかを判定
inline static bool is_inside(HLayer layer, int x, int y)
{
	return 0 <= x && (size_t)x < layer->width && 0 <= y && (size_t)y < layer->height;
}

/// @brief ピクセルインデックスを計算
inline static size_t pixel_index(HLayer layer, int x, int y)
{
	return (size_t)y * layer->width + (size_t)x;
}

/// @brief 値を範囲内にクランプ
inline static int clamp_int(int value, int min, int max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

/// @brief double を 0.0-1.0 にクランプ
inline static double clamp01(double v)
{
	if (v < 0.0) return 0.0;
	if (v > 1.0) return 1.0;
	return v;
}

/// @brief 内部用: ピクセルをブレンド設定（AA用）
inline static void blend_pixel(HLayer layer, int x, int y, RGBA color)
{
	if (is_inside(layer, x, y))
	{
		size_t idx         = pixel_index(layer, x, y);
		layer->pixels[idx] = rgba_blend(layer->pixels[idx], color);
	}
}

// ============================================================================
// ライフサイクル関数
// ============================================================================

#define LAYER_NULL_CHECK_RET_NULL(layer)                                                                                    \
	if (layer == NULL)                                                                                                      \
	{                                                                                                                       \
		return NULL;                                                                                                        \
	}

#define LAYER_CREATE_AND_NULL_CHECK_RET(layer, size)                                                                        \
	HLayer layer = layer_create(size);                                                                                      \
	if (layer == NULL)                                                                                                      \
	{                                                                                                                       \
		return NULL;                                                                                                        \
	}

/// @brief ピクセルへの直接参照（範囲チェックなし）
#define PIXEL_AT(layer, x, y) ((layer)->pixels[(y) * (layer)->width + (x)])

/// @brief 一時レイヤーから内容を移譲し、一時レイヤーのシェルを解放
#define LAYER_ADOPT_TEMP(layer, temp)                                                                                       \
	do                                                                                                                      \
	{                                                                                                                       \
		if ((temp) == NULL) return;                                                                                         \
		free((layer)->pixels);                                                                                              \
		(layer)->width  = (temp)->width;                                                                                    \
		(layer)->height = (temp)->height;                                                                                   \
		(layer)->pixels = (temp)->pixels;                                                                                   \
		free(temp);                                                                                                         \
	} while (0)

HLayer layer_create(Size size)
{
	if (size.w == 0 || size.h == 0)
	{
		return NULL;
	}

	HLayer layer = malloc(sizeof(Layer));
	if (layer == NULL)
	{
		return NULL;
	}

	layer->width  = size.w;
	layer->height = size.h;
	layer->pixels = calloc(size.w * size.h, sizeof(RGBA));

	if (layer->pixels == NULL)
	{
		free(layer);
		return NULL;
	}

	return layer;
}

void layer_destroy(HLayer layer)
{
	if (layer == NULL)
	{
		return;
	}
	free(layer->pixels);
	free(layer);
}

HLayer layer_clone(HLayer layer)
{
	LAYER_NULL_CHECK_RET_NULL(layer);

	LAYER_CREATE_AND_NULL_CHECK_RET(clone, SIZE(layer->width, layer->height));

	memcpy(clone->pixels, layer->pixels, layer->width * layer->height * sizeof(RGBA));
	return clone;
}

void layer_swap(HLayer a, HLayer b)
{
	if (a == NULL || b == NULL)
	{
		return;
	}

	// 構造体の全メンバを入れ替え
	size_t tmp_width  = a->width;
	size_t tmp_height = a->height;
	RGBA*  tmp_pixels = a->pixels;

	a->width  = b->width;
	a->height = b->height;
	a->pixels = b->pixels;

	b->width  = tmp_width;
	b->height = tmp_height;
	b->pixels = tmp_pixels;
}

HLayer layer_from_rgba_1d(Size size, const RGBA* data)
{
	LAYER_NULL_CHECK_RET_NULL(data);

	LAYER_CREATE_AND_NULL_CHECK_RET(layer, size);

	memcpy(layer->pixels, data, size.w * size.h * sizeof(RGBA));
	return layer;
}

HLayer layer_from_rgb_1d(Size size, const RGB* data)
{
	LAYER_NULL_CHECK_RET_NULL(data);

	LAYER_CREATE_AND_NULL_CHECK_RET(layer, size);

	for (size_t i = 0; i < size.w * size.h; ++i)
	{
		layer->pixels[i] = rgba_from_rgb(data[i], 1.0);
	}
	return layer;
}

HLayer layer_from_rgba_2d(Size size, const RGBA* const* data)
{
	LAYER_NULL_CHECK_RET_NULL(data);

	LAYER_CREATE_AND_NULL_CHECK_RET(layer, size);

	for (size_t y = 0; y < size.h; ++y)
	{
		memcpy(&layer->pixels[y * size.w], data[y], size.w * sizeof(RGBA));
	}
	return layer;
}

HLayer layer_from_rgb_2d(Size size, const RGB* const* data)
{
	LAYER_NULL_CHECK_RET_NULL(data);

	LAYER_CREATE_AND_NULL_CHECK_RET(layer, size);

	for (size_t y = 0; y < size.h; ++y)
	{
		for (size_t x = 0; x < size.w; ++x)
		{
			layer->pixels[y * size.w + x] = rgba_from_rgb(data[y][x], 1.0);
		}
	}
	return layer;
}

// ============================================================================
// 情報取得関数
// ============================================================================

size_t layer_get_width(HLayer layer)
{
	return layer->width;
}

size_t layer_get_height(HLayer layer)
{
	return layer->height;
}

Size layer_get_size(HLayer layer)
{
	return SIZE(layer->width, layer->height);
}

// ============================================================================
// ピクセル操作関数
// ============================================================================

RGBA layer_get_pixel(HLayer layer, Point p)
{
	if (!is_inside(layer, p.x, p.y))
	{
		return rgba_new(0.0, 0.0, 0.0, 0.0);
	}
	return layer->pixels[pixel_index(layer, p.x, p.y)];
}

void layer_set_pixel(HLayer layer, Point p, RGBA color)
{
	if (is_inside(layer, p.x, p.y))
	{
		layer->pixels[pixel_index(layer, p.x, p.y)] = color;
	}
}

void layer_fill(HLayer layer, RGBA color)
{
	size_t total = layer->width * layer->height;
	for (size_t i = 0; i < total; ++i)
	{
		layer->pixels[i] = color;
	}
}

// ============================================================================
// ファイル入出力関数
// ============================================================================

bool layer_save_p3(HLayer layer, const char* filename)
{
	if (layer == NULL || filename == NULL)
	{
		return false;
	}

	FILE* fp = fopen(filename, "w");
	if (fp == NULL)
	{
		return false;
	}

	// PPM P3 ヘッダ
	fprintf(fp, "P3\n%zu %zu\n255\n", layer->width, layer->height);

	// ピクセルデータ（テキスト）
	for (size_t y = 0; y < layer->height; ++y)
	{
		for (size_t x = 0; x < layer->width; ++x)
		{
			RGBA px      = layer->pixels[y * layer->width + x];
			RGB  applied = rgba_apply_alpha(px);
			int  r       = (int)(clamp01(applied.r) * 255.0 + 0.5);
			int  g       = (int)(clamp01(applied.g) * 255.0 + 0.5);
			int  b       = (int)(clamp01(applied.b) * 255.0 + 0.5);
			fprintf(fp, "%d %d %d\n", r, g, b);
		}
	}

	fclose(fp);
	return true;
}

bool layer_save_p6(HLayer layer, const char* filename)
{
	if (layer == NULL || filename == NULL)
	{
		return false;
	}

	FILE* fp = fopen(filename, "wb");
	if (fp == NULL)
	{
		return false;
	}

	// PPM P6 ヘッダ
	fprintf(fp, "P6\n%zu %zu\n255\n", layer->width, layer->height);

	// ピクセルデータ（バイナリ）
	for (size_t y = 0; y < layer->height; ++y)
	{
		for (size_t x = 0; x < layer->width; ++x)
		{
			RGBA          px      = layer->pixels[y * layer->width + x];
			RGB           applied = rgba_apply_alpha(px);
			unsigned char rgb[3];
			rgb[0] = (unsigned char)(clamp01(applied.r) * 255.0 + 0.5);
			rgb[1] = (unsigned char)(clamp01(applied.g) * 255.0 + 0.5);
			rgb[2] = (unsigned char)(clamp01(applied.b) * 255.0 + 0.5);
			fwrite(rgb, 1, 3, fp);
		}
	}

	fclose(fp);
	return true;
}

/// @brief PPM のヘッダを読み込む（P3/P6共通）
static bool read_ppm_header(FILE* fp, char* format, size_t* width, size_t* height, int* max_val)
{
	// フォーマット識別子
	if (fscanf(fp, "%2s", format) != 1)
	{
		return false;
	}

	// コメントをスキップ
	int c;
	while ((c = fgetc(fp)) != EOF)
	{
		if (c == '#')
		{
			while ((c = fgetc(fp)) != EOF && c != '\n')
			{}
		}
		else if (!isspace(c))
		{
			ungetc(c, fp);
			break;
		}
	}

	// 幅、高さ、最大値
	if (fscanf(fp, "%zu %zu %d", width, height, max_val) != 3)
	{
		return false;
	}

	// ヘッダ終端の空白を読み飛ばす
	fgetc(fp);

	return true;
}

HLayer layer_load_p3(const char* filename)
{
	if (filename == NULL)
	{
		return NULL;
	}

	FILE* fp = fopen(filename, "r");
	if (fp == NULL)
	{
		return NULL;
	}

	char   format[4];
	size_t width, height;
	int    max_val;

	if (!read_ppm_header(fp, format, &width, &height, &max_val))
	{
		fclose(fp);
		return NULL;
	}

	if (strcmp(format, "P3") != 0)
	{
		fclose(fp);
		return NULL;
	}

	HLayer layer = layer_create(SIZE(width, height));
	if (layer == NULL)
	{
		fclose(fp);
		return NULL;
	}

	double scale = 1.0 / (double)max_val;
	for (size_t i = 0; i < width * height; ++i)
	{
		int r, g, b;
		if (fscanf(fp, "%d %d %d", &r, &g, &b) != 3)
		{
			layer_destroy(layer);
			fclose(fp);
			return NULL;
		}
		layer->pixels[i] = rgba_new(r * scale, g * scale, b * scale, 1.0);
	}

	fclose(fp);
	return layer;
}

HLayer layer_load_p6(const char* filename)
{
	if (filename == NULL)
	{
		return NULL;
	}

	FILE* fp = fopen(filename, "rb");
	if (fp == NULL)
	{
		return NULL;
	}

	char   format[4];
	size_t width, height;
	int    max_val;

	if (!read_ppm_header(fp, format, &width, &height, &max_val))
	{
		fclose(fp);
		return NULL;
	}

	if (strcmp(format, "P6") != 0)
	{
		fclose(fp);
		return NULL;
	}

	HLayer layer = layer_create(SIZE(width, height));
	if (layer == NULL)
	{
		fclose(fp);
		return NULL;
	}

	double scale = 1.0 / (double)max_val;
	for (size_t i = 0; i < width * height; ++i)
	{
		unsigned char rgb[3];
		if (fread(rgb, 1, 3, fp) != 3)
		{
			layer_destroy(layer);
			fclose(fp);
			return NULL;
		}
		layer->pixels[i] = rgba_new(rgb[0] * scale, rgb[1] * scale, rgb[2] * scale, 1.0);
	}

	fclose(fp);
	return layer;
}

// ============================================================================
// 幾何図形描画関数（アンチエイリアスなし）
// ============================================================================

void layer_draw_rect(HLayer layer, Point pos, Size size, RGBA color)
{
	int x_start = clamp_int(pos.x, 0, (int)layer->width);
	int y_start = clamp_int(pos.y, 0, (int)layer->height);
	int x_end   = clamp_int(pos.x + (int)size.w, 0, (int)layer->width);
	int y_end   = clamp_int(pos.y + (int)size.h, 0, (int)layer->height);

	for (int y = y_start; y < y_end; ++y)
	{
		for (int x = x_start; x < x_end; ++x)
		{
			layer->pixels[pixel_index(layer, x, y)] = color;
		}
	}
}

void layer_draw_line(HLayer layer, Point p1, Point p2, size_t thickness, RGBA color)
{
	if (thickness == 0)
	{
		return;
	}

	// 太さ1の場合は Bresenham アルゴリズムで描画（ポリゴン化によるドット抜け防止）
	if (thickness == 1)
	{
		int x0 = p1.x;
		int y0 = p1.y;
		int x1 = p2.x;
		int y1 = p2.y;

		int dx  = abs(x1 - x0);
		int dy  = abs(y1 - y0);
		int sx  = (x0 < x1) ? 1 : -1;
		int sy  = (y0 < y1) ? 1 : -1;
		int err = dx - dy;

		while (1)
		{
			layer_set_pixel(layer, POINT(x0, y0), color);

			if (x0 == x1 && y0 == y1) break;

			int e2 = 2 * err;
			if (e2 > -dy)
			{
				err -= dy;
				x0  += sx;
			}
			if (e2 < dx)
			{
				err += dx;
				y0  += sy;
			}
		}
		return;
	}

	// 方向ベクトル
	double dx  = (double)(p2.x - p1.x);
	double dy  = (double)(p2.y - p1.y);
	double len = sqrt(dx * dx + dy * dy);

	// 長さ0の場合は点を描画
	if (len < 0.001)
	{
		int half = (int)thickness / 2;
		for (int ty = -half; ty <= half; ++ty)
		{
			for (int tx = -half; tx <= half; ++tx)
			{
				layer_set_pixel(layer, POINT(p1.x + tx, p1.y + ty), color);
			}
		}
		return;
	}

	// 法線ベクトル（線に垂直、長さ = thickness/2）
	double nx = -dy / len * ((double)thickness / 2.0);
	double ny = dx / len * ((double)thickness / 2.0);

	// 4頂点を計算
	Point v0 = POINT((int)round((double)p1.x + nx), (int)round((double)p1.y + ny));
	Point v1 = POINT((int)round((double)p1.x - nx), (int)round((double)p1.y - ny));
	Point v2 = POINT((int)round((double)p2.x - nx), (int)round((double)p2.y - ny));
	Point v3 = POINT((int)round((double)p2.x + nx), (int)round((double)p2.y + ny));

	// 凸多角形として描画 (v0 -> v3 -> v2 -> v1 の順)
	Point points[4] = {v0, v3, v2, v1};
	layer_draw_polygon(layer, points, 4, color);
}

void layer_draw_circle(HLayer layer, Point center, size_t radius, RGBA color)
{
	int r  = (int)radius;
	int r2 = r * r;

	for (int dy = -r; dy <= r; ++dy)
	{
		for (int dx = -r; dx <= r; ++dx)
		{
			if (dx * dx + dy * dy <= r2)
			{
				layer_set_pixel(layer, POINT(center.x + dx, center.y + dy), color);
			}
		}
	}
}

/// @brief 三角形塗りつぶし用のスキャンライン関数
static void fill_triangle_scanline(HLayer layer, Point p1, Point p2, Point p3, RGBA color)
{
	// 頂点を Y 座標でソート
	if (p1.y > p2.y)
	{
		Point t = p1;
		p1      = p2;
		p2      = t;
	}
	if (p2.y > p3.y)
	{
		Point t = p2;
		p2      = p3;
		p3      = t;
	}
	if (p1.y > p2.y)
	{
		Point t = p1;
		p1      = p2;
		p2      = t;
	}

	int total_height = p3.y - p1.y;
	if (total_height == 0)
	{
		return;
	}

	for (int y = p1.y; y <= p3.y; ++y)
	{
		bool second_half    = (y > p2.y) || (p2.y == p1.y);
		int  segment_height = second_half ? (p3.y - p2.y) : (p2.y - p1.y);
		if (segment_height == 0) segment_height = 1;

		double alpha = (double)(y - p1.y) / total_height;
		double beta  = second_half ? (double)(y - p2.y) / segment_height : (double)(y - p1.y) / segment_height;

		int ax = (int)(p1.x + (p3.x - p1.x) * alpha);
		int bx = second_half ? (int)(p2.x + (p3.x - p2.x) * beta) : (int)(p1.x + (p2.x - p1.x) * beta);

		if (ax > bx)
		{
			int t = ax;
			ax    = bx;
			bx    = t;
		}

		for (int x = ax; x <= bx; ++x)
		{
			layer_set_pixel(layer, POINT(x, y), color);
		}
	}
}

void layer_draw_triangle(HLayer layer, Point p1, Point p2, Point p3, RGBA color)
{
	fill_triangle_scanline(layer, p1, p2, p3, color);
}

// 凸多角形を一括で描画（アンチエイリアスなし）
void layer_draw_polygon(HLayer layer, const Point* points, size_t n, RGBA color)
{
	if (n < 3) return;

	// バウンディングボックス計算
	int min_x = (int)layer->width - 1;
	int max_x = 0;
	int min_y = (int)layer->height - 1;
	int max_y = 0;

	for (size_t i = 0; i < n; ++i)
	{
		if (points[i].x < min_x) min_x = points[i].x;
		if (points[i].x > max_x) max_x = points[i].x;
		if (points[i].y < min_y) min_y = points[i].y;
		if (points[i].y > max_y) max_y = points[i].y;
	}

	// 画面外判定
	if (min_x >= (int)layer->width || max_x < 0 || min_y >= (int)layer->height || max_y < 0) return;

	min_x = clamp_int(min_x, 0, (int)layer->width - 1);
	max_x = clamp_int(max_x, 0, (int)layer->width - 1);
	min_y = clamp_int(min_y, 0, (int)layer->height - 1);
	max_y = clamp_int(max_y, 0, (int)layer->height - 1);

	// Edge Equation の係数を事前計算
	int    count = (int)n;
	double nx[count];
	double ny[count];
	double c[count];

	// 回転方向を判定 (Y軸下向き: 正=CW, 負=CCW)
	double signed_area = 0.0;
	for (size_t i = 0; i < n; ++i)
	{
		Point p1     = points[i];
		Point p2     = points[(i + 1) % n];
		signed_area += (double)p1.x * (double)p2.y - (double)p2.x * (double)p1.y;
	}

	// CCWなら法線を反転させるための係数
	double orientation = (signed_area < 0.0) ? -1.0 : 1.0;

	for (size_t i = 0; i < n; ++i)
	{
		Point  p1  = points[i];
		Point  p2  = points[(i + 1) % n];
		double dx  = (double)(p2.x - p1.x);
		double dy  = (double)(p2.y - p1.y);
		double len = sqrt(dx * dx + dy * dy);
		if (len > 0.0)
		{
			// Edge Function: (px - x1)*dy - (py - y1)*dx
			nx[i] = (-dy / len) * orientation;
			ny[i] = (dx / len) * orientation;
			c[i]  = -((double)p1.x * nx[i] + (double)p1.y * ny[i]);
		}
		else
		{
			nx[i] = ny[i] = c[i] = 0.0;
		}
	}

	for (int y = min_y; y <= max_y; ++y)
	{
		for (int x = min_x; x <= max_x; ++x)
		{
			double px     = (double)x + 0.5;
			double py     = (double)y + 0.5;
			int    inside = 1;

			for (size_t i = 0; i < n; ++i)
			{
				// ピクセル中心がエッジの内側(>=0)か判定
				// 少し余裕を持たせるなら -0.001 とか
				if (px * nx[i] + py * ny[i] + c[i] < 0.0)
				{
					inside = 0;
					break;
				}
			}

			if (inside)
			{
				// 非AAなので単純代入
				layer_set_pixel(layer, POINT(x, y), color);
			}
		}
	}
}

// ============================================================================
// 幾何図形描画関数（アンチエイリアスあり）
// ============================================================================

// 前方宣言なし

void layer_draw_line_aa(HLayer layer, Point p1, Point p2, size_t thickness, RGBA color)
{
	if (thickness == 0)
	{
		return;
	}

	// 方向ベクトル
	double dx  = (double)(p2.x - p1.x);
	double dy  = (double)(p2.y - p1.y);
	double len = sqrt(dx * dx + dy * dy);

	// 長さ0の場合は円を描画
	if (len < 0.001)
	{
		layer_draw_circle_aa(layer, p1, thickness / 2, color);
		return;
	}

	// 法線ベクトル（線に垂直、長さ = thickness/2）
	double nx = -dy / len * ((double)thickness / 2.0);
	double ny = dx / len * ((double)thickness / 2.0);

	// 4頂点を計算（doubleで保持）
	double v0x = (double)p1.x + nx, v0y = (double)p1.y + ny;
	double v1x = (double)p1.x - nx, v1y = (double)p1.y - ny;
	double v2x = (double)p2.x - nx, v2y = (double)p2.y - ny;
	double v3x = (double)p2.x + nx, v3y = (double)p2.y + ny;

	// Edge Function用のポイント（整数に丸める）
	Point pv0 = POINT((int)round(v0x), (int)round(v0y));
	Point pv1 = POINT((int)round(v1x), (int)round(v1y));
	Point pv2 = POINT((int)round(v2x), (int)round(v2y));
	Point pv3 = POINT((int)round(v3x), (int)round(v3y));

	// 凸多角形として描画 (v0 -> v3 -> v2 -> v1 の順)
	Point points[4] = {pv0, pv3, pv2, pv1};
	layer_draw_polygon_aa(layer, points, 4, color);
}

void layer_draw_rect_aa(HLayer layer, Point pos, Size size, RGBA color)
{
	// 矩形は軸平行のため、内部を非AAで塗りつぶしてエッジをAA線で描画
	// 境界ピクセルのカバレッジを計算するより内部塗り+エッジがシンプル
	layer_draw_rect(layer, pos, size, color);

	// 4辺をAA線で上書き（エッジを滑らかにする）
	Point tl = pos;
	Point tr = POINT(pos.x + (int)size.w - 1, pos.y);
	Point bl = POINT(pos.x, pos.y + (int)size.h - 1);
	Point br = POINT(pos.x + (int)size.w - 1, pos.y + (int)size.h - 1);

	layer_draw_line_aa(layer, tl, tr, 1, color);
	layer_draw_line_aa(layer, tr, br, 1, color);
	layer_draw_line_aa(layer, br, bl, 1, color);
	layer_draw_line_aa(layer, bl, tl, 1, color);
}

void layer_draw_circle_aa(HLayer layer, Point center, size_t radius, RGBA color)
{
	int r = (int)radius;

	for (int y = -r; y <= r; ++y)
	{
		for (int x = -r; x <= r; ++x)
		{
			double dist     = sqrt((double)(x * x + y * y));
			double coverage = clamp01((double)r - dist + 0.5);

			if (coverage > 0.0)
			{
				RGBA blended = rgba_from_rgb(color.rgb, color.a * coverage);
				blend_pixel(layer, center.x + x, center.y + y, blended);
			}
		}
	}
}

/// @brief サブピクセルサンプリングでカバレッジを計算（2x2）
// 凸多角形を一括で描画（アンチエイリアス対応）
void layer_draw_polygon_aa(HLayer layer, const Point* points, size_t n, RGBA color)
{
	if (n < 3) return;

	// バウンディングボックス計算
	int min_x = (int)layer->width - 1;
	int max_x = 0;
	int min_y = (int)layer->height - 1;
	int max_y = 0;

	for (size_t i = 0; i < n; ++i)
	{
		if (points[i].x < min_x) min_x = points[i].x;
		if (points[i].x > max_x) max_x = points[i].x;
		if (points[i].y < min_y) min_y = points[i].y;
		if (points[i].y > max_y) max_y = points[i].y;
	}

	// 画面外判定
	if (min_x >= (int)layer->width || max_x < 0 || min_y >= (int)layer->height || max_y < 0) return;

	min_x = clamp_int(min_x, 0, (int)layer->width - 1);
	max_x = clamp_int(max_x, 0, (int)layer->width - 1);
	min_y = clamp_int(min_y, 0, (int)layer->height - 1);
	max_y = clamp_int(max_y, 0, (int)layer->height - 1);

	// Edge Equation の係数を事前計算
	// VLA (Variable Length Array) を使用
	int    count = (int)n;
	double nx[count];
	double ny[count];
	double c[count];

	// 回転方向を判定 (Y軸下向き: 正=CW, 負=CCW)
	double signed_area = 0.0;
	for (size_t i = 0; i < n; ++i)
	{
		Point p1     = points[i];
		Point p2     = points[(i + 1) % n];
		signed_area += (double)p1.x * (double)p2.y - (double)p2.x * (double)p1.y;
	}

	// CCWなら法線を反転させるための係数
	double orientation = (signed_area < 0.0) ? -1.0 : 1.0;

	for (size_t i = 0; i < n; ++i)
	{
		Point  p1  = points[i];
		Point  p2  = points[(i + 1) % n];
		double dx  = (double)(p2.x - p1.x);
		double dy  = (double)(p2.y - p1.y);
		double len = sqrt(dx * dx + dy * dy);
		if (len > 0.0)
		{
			// Edge Function: (px - x1)*dy - (py - y1)*dx
			// (-dy, dx) は進行方向右側（CW内側）を向く法線
			// CCWの場合はこれを反転させて内側に向ける
			nx[i] = (-dy / len) * orientation;
			ny[i] = (dx / len) * orientation;

			// c項: -(x1*nx + y1*ny)
			c[i] = -((double)p1.x * nx[i] + (double)p1.y * ny[i]);
		}
		else
		{
			nx[i] = ny[i] = c[i] = 0.0;
		}
	}

	for (int y = min_y; y <= max_y; ++y)
	{
		for (int x = min_x; x <= max_x; ++x)
		{
			double px = (double)x + 0.5;
			double py = (double)y + 0.5;

			double min_dist = 1000.0; // 十分大きな値
			int    inside   = 1;

			// 全エッジに対する距離をチェック
			for (size_t i = 0; i < n; ++i)
			{
				// 距離 (内側が正)
				double dist = px * nx[i] + py * ny[i] + c[i];

				// 1ピクセル以上外側なら描画対象外（早期終了）
				if (dist < -0.5)
				{
					inside = 0;
					break;
				}

				if (dist < min_dist)
				{
					min_dist = dist;
				}
			}

			if (inside)
			{
				// 最小距離に基づいてカバレッジを計算 (-0.5 〜 0.5 の範囲で遷移)
				double coverage = clamp01(min_dist + 0.5);
				if (coverage > 0.0)
				{
					blend_pixel(layer, x, y, rgba_from_rgb(color.rgb, color.a * coverage));
				}
			}
		}
	}
}

void layer_draw_triangle_aa(HLayer layer, Point p1, Point p2, Point p3, RGBA color)
{
	Point points[3] = {p1, p2, p3};
	layer_draw_polygon_aa(layer, points, 3, color);
}

// ============================================================================
// 変形関数（新規レイヤー作成版）
// ============================================================================

HLayer layer_clip_to(HLayer layer, Point pos, Size size)
{
	HLayer result = layer_create(size);
	if (result == NULL)
	{
		return NULL;
	}

	for (size_t y = 0; y < size.h; ++y)
	{
		for (size_t x = 0; x < size.w; ++x)
		{
			result->pixels[y * size.w + x] = layer_get_pixel(layer, POINT(pos.x + (int)x, pos.y + (int)y));
		}
	}
	return result;
}

HLayer layer_resize_nearest_to(HLayer layer, Size new_size)
{
	HLayer result = layer_create(new_size);
	if (result == NULL)
	{
		return NULL;
	}

	double x_scale = (double)layer->width / (double)new_size.w;
	double y_scale = (double)layer->height / (double)new_size.h;

	for (size_t y = 0; y < new_size.h; ++y)
	{
		for (size_t x = 0; x < new_size.w; ++x)
		{
			int src_x                          = (int)((double)x * x_scale);
			int src_y                          = (int)((double)y * y_scale);
			result->pixels[y * new_size.w + x] = layer_get_pixel(layer, POINT(src_x, src_y));
		}
	}
	return result;
}

/// @brief RGBAの加重平均（Bilinear補間用）
static RGBA rgba_lerp_bilinear(RGBA tl, RGBA tr, RGBA bl, RGBA br, double fx, double fy)
{
	// 上辺の補間
	double t_r = tl.rgb.r * (1.0 - fx) + tr.rgb.r * fx;
	double t_g = tl.rgb.g * (1.0 - fx) + tr.rgb.g * fx;
	double t_b = tl.rgb.b * (1.0 - fx) + tr.rgb.b * fx;
	double t_a = tl.a * (1.0 - fx) + tr.a * fx;

	// 下辺の補間
	double b_r = bl.rgb.r * (1.0 - fx) + br.rgb.r * fx;
	double b_g = bl.rgb.g * (1.0 - fx) + br.rgb.g * fx;
	double b_b = bl.rgb.b * (1.0 - fx) + br.rgb.b * fx;
	double b_a = bl.a * (1.0 - fx) + br.a * fx;

	// 上下の補間
	return rgba_new(
		t_r * (1.0 - fy) + b_r * fy,
		t_g * (1.0 - fy) + b_g * fy,
		t_b * (1.0 - fy) + b_b * fy,
		t_a * (1.0 - fy) + b_a * fy
	);
}

HLayer layer_resize_bilinear_to(HLayer layer, Size new_size)
{
	HLayer result = layer_create(new_size);
	if (result == NULL)
	{
		return NULL;
	}

	double x_scale = (double)(layer->width - 1) / (double)(new_size.w - 1);
	double y_scale = (double)(layer->height - 1) / (double)(new_size.h - 1);

	for (size_t y = 0; y < new_size.h; ++y)
	{
		for (size_t x = 0; x < new_size.w; ++x)
		{
			double src_x = (double)x * x_scale;
			double src_y = (double)y * y_scale;

			int x0 = (int)floor(src_x);
			int y0 = (int)floor(src_y);
			int x1 = clamp_int(x0 + 1, 0, (int)layer->width - 1);
			int y1 = clamp_int(y0 + 1, 0, (int)layer->height - 1);

			double fx = src_x - (double)x0;
			double fy = src_y - (double)y0;

			RGBA tl = layer_get_pixel(layer, POINT(x0, y0));
			RGBA tr = layer_get_pixel(layer, POINT(x1, y0));
			RGBA bl = layer_get_pixel(layer, POINT(x0, y1));
			RGBA br = layer_get_pixel(layer, POINT(x1, y1));

			result->pixels[y * new_size.w + x] = rgba_lerp_bilinear(tl, tr, bl, br, fx, fy);
		}
	}
	return result;
}

HLayer layer_flip_horizontal_to(HLayer layer)
{
	HLayer result = layer_create(SIZE(layer->width, layer->height));
	if (result == NULL)
	{
		return NULL;
	}

	for (size_t y = 0; y < layer->height; ++y)
	{
		for (size_t x = 0; x < layer->width; ++x)
		{
			result->pixels[y * layer->width + x] = layer->pixels[y * layer->width + (layer->width - 1 - x)];
		}
	}
	return result;
}

HLayer layer_flip_vertical_to(HLayer layer)
{
	HLayer result = layer_create(SIZE(layer->width, layer->height));
	if (result == NULL)
	{
		return NULL;
	}

	for (size_t y = 0; y < layer->height; ++y)
	{
		memcpy(&result->pixels[y * layer->width], &PIXEL_AT(layer, 0, layer->height - 1 - y), layer->width * sizeof(RGBA));
	}
	return result;
}

HLayer layer_downsample_2x_to(HLayer layer)
{
	Size   new_size = SIZE(layer->width / 2, layer->height / 2);
	HLayer result   = layer_create(new_size);
	if (result == NULL)
	{
		return NULL;
	}

	for (size_t y = 0; y < new_size.h; ++y)
	{
		for (size_t x = 0; x < new_size.w; ++x)
		{
			// 2x2 ピクセルの平均
			RGBA tl = PIXEL_AT(layer, x * 2, y * 2);
			RGBA tr = PIXEL_AT(layer, x * 2 + 1, y * 2);
			RGBA bl = PIXEL_AT(layer, x * 2, y * 2 + 1);
			RGBA br = PIXEL_AT(layer, x * 2 + 1, y * 2 + 1);

			RGB tl_rgb = rgba_apply_alpha(tl);
			RGB tr_rgb = rgba_apply_alpha(tr);
			RGB bl_rgb = rgba_apply_alpha(bl);
			RGB br_rgb = rgba_apply_alpha(br);

			RGB    r = rgb_mul(rgb_add(rgb_add(tl_rgb, tr_rgb), rgb_add(bl_rgb, br_rgb)), 0.25);
			double a = (tl.a + tr.a + bl.a + br.a) / 4.0;

			PIXEL_AT(result, x, y) = (RGBA){r, a};
		}
	}
	return result;
}

// ============================================================================
// 変形関数（破壊的変更版）
// ============================================================================

void layer_clip(HLayer layer, Point pos, Size size)
{
	HLayer temp = layer_clip_to(layer, pos, size);
	LAYER_ADOPT_TEMP(layer, temp);
}

void layer_resize_nearest(HLayer layer, Size new_size)
{
	HLayer temp = layer_resize_nearest_to(layer, new_size);
	LAYER_ADOPT_TEMP(layer, temp);
}

void layer_resize_bilinear(HLayer layer, Size new_size)
{
	HLayer temp = layer_resize_bilinear_to(layer, new_size);
	LAYER_ADOPT_TEMP(layer, temp);
}

void layer_flip_horizontal(HLayer layer)
{
	for (size_t y = 0; y < layer->height; ++y)
	{
		for (size_t x = 0; x < layer->width / 2; ++x)
		{
			size_t left          = y * layer->width + x;
			size_t right         = y * layer->width + (layer->width - 1 - x);
			RGBA   t             = layer->pixels[left];
			layer->pixels[left]  = layer->pixels[right];
			layer->pixels[right] = t;
		}
	}
}

void layer_flip_vertical(HLayer layer)
{
	RGBA* row_buf = malloc(layer->width * sizeof(RGBA));
	if (row_buf == NULL)
	{
		return;
	}

	for (size_t y = 0; y < layer->height / 2; ++y)
	{
		RGBA* top    = &PIXEL_AT(layer, 0, y);
		RGBA* bottom = &PIXEL_AT(layer, 0, layer->height - 1 - y);
		memcpy(row_buf, top, layer->width * sizeof(RGBA));
		memcpy(top, bottom, layer->width * sizeof(RGBA));
		memcpy(bottom, row_buf, layer->width * sizeof(RGBA));
	}
	free(row_buf);
}

void layer_downsample_2x(HLayer layer)
{
	HLayer temp = layer_downsample_2x_to(layer);
	LAYER_ADOPT_TEMP(layer, temp);
}

// ============================================================================
// レイヤー合成関数
// ============================================================================

void layer_composite(HLayer dst, HLayer src, Point pos, BlendFunc blend)
{
	BlendFunc blend_fn = blend ? blend : rgba_blend;

	for (size_t sy = 0; sy < src->height; ++sy)
	{
		int dy = pos.y + (int)sy;
		if (dy < 0 || (size_t)dy >= dst->height)
		{
			continue;
		}

		for (size_t sx = 0; sx < src->width; ++sx)
		{
			int dx = pos.x + (int)sx;
			if (dx < 0 || (size_t)dx >= dst->width)
			{
				continue;
			}

			RGBA src_px = src->pixels[sy * src->width + sx];
			RGBA dst_px = dst->pixels[(size_t)dy * dst->width + (size_t)dx];

			dst->pixels[(size_t)dy * dst->width + (size_t)dx] = blend_fn(dst_px, src_px);
		}
	}
}
