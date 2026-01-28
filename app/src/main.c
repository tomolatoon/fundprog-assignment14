/// @file main.c
/// @brief 動画コンポーネント統合アプリケーション

#include "h2511186/component.h"
#include "k2511070/component.h"
#include "layer/layer.h"
#include <stdio.h>

#define SECONDS 10.0
#define FPS     30.0
#define WIDTH   640
#define HEIGHT  480

typedef HLayer (*video_layer)(double);
video_layer layers[] = {h2511186_background_layer, h2511186_sun_layer, h2511186_ground_layer, h2511186_miku_layer};

int main(void)
{
	HLayer canvas = layer_create(SIZE(WIDTH, HEIGHT));
	double diff   = 1 / FPS;
	for (int frame_num = 0; diff * frame_num <= SECONDS; ++frame_num)
	{
		double t = diff * frame_num;
		layer_fill(
			canvas,
			(RGBA){
				{1.0, 1.0, 1.0},
				1.0
        }
		);

		for (size_t i = 0; i < sizeof(layers) / sizeof(layers[0]); ++i)
		{
			HLayer src = layers[i](t);
			if (src)
			{
				layer_composite(canvas, src, POINT(0, 0), NULL);
				layer_destroy(src);
			}
		}

		char filename[256];
		snprintf(filename, sizeof(filename), "output/frames/frame_%04d.ppm", frame_num);
		printf("Saving %s...\n", filename);
		layer_save_p3(canvas, filename);
	}

	layer_destroy(canvas);

	return 0;
}
