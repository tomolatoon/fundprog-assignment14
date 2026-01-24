/// @file layer.c
/// @brief Layer ライブラリの実装（スケルトン）

#include "layer/layer.h"

#include <stdlib.h>
#include <string.h>

// ============== 内部構造体定義 ==============

/// @brief Layer 構造体の実体
struct Layer
{
	size_t width;  ///< 幅（ピクセル）
	size_t height; ///< 高さ（ピクセル）
	RGBA*  pixels; ///< ピクセルデータ（row-major, 左上原点）
};

// ============== ライフサイクル関数 ==============

HLayer layer_create(size_t width, size_t height)
{
	if (width == 0 || height == 0)
	{
		return NULL;
	}

	HLayer layer = malloc(sizeof(Layer));
	if (layer == NULL)
	{
		return NULL;
	}

	layer->width  = width;
	layer->height = height;
	layer->pixels = calloc(width * height, sizeof(RGBA));

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

// ============== 情報取得関数 ==============

size_t layer_get_width(HLayer layer)
{
	return layer->width;
}

size_t layer_get_height(HLayer layer)
{
	return layer->height;
}

// ============== ピクセル操作関数 ==============

RGBA layer_get_pixel(HLayer layer, size_t x, size_t y)
{
	if (x >= layer->width || y >= layer->height)
	{
		// 範囲外は透明を返す
		return rgba_new(0.0, 0.0, 0.0, 0.0);
	}

	return layer->pixels[y * layer->width + x];
}

void layer_set_pixel(HLayer layer, size_t x, size_t y, RGBA color)
{
	if (x >= layer->width || y >= layer->height)
	{
		return; // 範囲外は無視
	}

	layer->pixels[y * layer->width + x] = color;
}

void layer_fill(HLayer layer, RGBA color)
{
	size_t total = layer->width * layer->height;
	for (size_t i = 0; i < total; i++)
	{
		layer->pixels[i] = color;
	}
}

// ============== 幾何図形描画関数（スタブ） ==============

void layer_draw_rect(HLayer layer, int x, int y, size_t width, size_t height, RGBA color)
{
	// TODO: 実装予定
	(void)layer;
	(void)x;
	(void)y;
	(void)width;
	(void)height;
	(void)color;
}

void layer_draw_circle(HLayer layer, int cx, int cy, size_t radius, RGBA color)
{
	// TODO: 実装予定
	(void)layer;
	(void)cx;
	(void)cy;
	(void)radius;
	(void)color;
}

// ============== レイヤー合成関数（スタブ） ==============

void layer_composite(HLayer dst, HLayer src, int x, int y, BlendFunc blend)
{
	// TODO: 実装予定
	(void)dst;
	(void)src;
	(void)x;
	(void)y;
	(void)blend;
}

// ============== ファイル出力関数（スタブ） ==============

bool layer_save_ppm(HLayer layer, const char* filename)
{
	// TODO: 実装予定
	(void)layer;
	(void)filename;
	return false;
}

// ============== 変形関数（スタブ） ==============

HLayer layer_rotate(HLayer layer, double angle_degrees)
{
	// TODO: 実装予定
	(void)layer;
	(void)angle_degrees;
	return NULL;
}
