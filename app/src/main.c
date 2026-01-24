/// @file main.c
/// @brief 動画コンポーネント統合アプリケーション

#include "h2511186/component.h"
#include "k2511070/component.h"
#include "layer/layer.h"
#include <stdio.h>

int main(void)
{
	printf("Video Composer - 動画コンポーネント統合アプリケーション\n");

	// コンポーネントの初期化
	h2511186_component_init();
	k2511070_component_init();

	// レイヤーを作成
	HLayer layer = layer_create(SIZE(640, 480));
	if (layer == NULL)
	{
		fprintf(stderr, "Error: Failed to create layer\n");
		return 1;
	}

	// 背景を白で塗りつぶし
	layer_fill(layer, rgba_new(1.0, 1.0, 1.0, 1.0));

	// 赤い矩形
	layer_draw_rect(layer, POINT(50, 50), SIZE(100, 80), rgba_new(1.0, 0.0, 0.0, 1.0));

	// 緑の円
	layer_draw_circle(layer, POINT(300, 200), 60, rgba_new(0.0, 0.8, 0.0, 1.0));

	// 青い三角形
	layer_draw_triangle(layer, POINT(450, 100), POINT(550, 200), POINT(400, 200), rgba_new(0.0, 0.0, 1.0, 1.0));

	// 黒い線
	layer_draw_line(layer, POINT(100, 400), POINT(500, 350), 3, rgba_new(0.0, 0.0, 0.0, 1.0));

	// AA 円（半透明マゼンタ）
	layer_draw_circle_aa(layer, POINT(200, 350), 40, rgba_new(1.0, 0.0, 1.0, 0.7));

	// AA 線
	layer_draw_line_aa(layer, POINT(50, 450), POINT(600, 100), rgba_new(0.0, 0.5, 0.5, 1.0));

	// PPM 出力
	if (layer_save_p6(layer, "output/demo.ppm"))
	{
		printf("Saved: output/demo.ppm\n");
	}
	else
	{
		fprintf(stderr, "Error: Failed to save PPM\n");
	}

	layer_destroy(layer);

	printf("Done.\n");
	return 0;
}
