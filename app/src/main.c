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

	// サンプル: レイヤーを作成して各コンポーネントで描画
	HLayer layer = layer_create(640, 480);
	if (layer == NULL)
	{
		fprintf(stderr, "Error: Failed to create layer\n");
		return 1;
	}

	// 各コンポーネントの描画（現在はダミー）
	h2511186_component_draw(layer);
	k2511070_component_draw(layer);

	// TODO: PPM出力、GIF変換など

	layer_destroy(layer);

	printf("Done.\n");
	return 0;
}
