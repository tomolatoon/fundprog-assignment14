/// @file main.c
/// @brief 動画コンポーネント統合アプリケーション

#include "h2511186/component.h"
#include "i2511030/component.h"
#include "k2511070/component.h"
#include "layer/layer.h"
#include <stdio.h>

int main(void)
{
	GeneratorF f[] = {i2511030_sample_layer1_generator, i2511030_sample_layer2_generator};

	HLayer layer = layer_create(SIZE(400, 400));

	double diff = 1 / 10.0;
	for (double t = 0; t < 1.0; t += diff)
	{
		layer_fill(
			layer,
			(RGBA){
				{1.0, 1.0, 1.0},
				1.0
        }
		);

		for (size_t i = 0; i < sizeof(f) / sizeof(f[0]); i++)
		{
			HLayer src = f[i](t);
			layer_composite(layer, src, POINT(0, 0), NULL);
			layer_destroy(src);
		}

		char filename[256];
		snprintf(filename, sizeof(filename), "output/frames/frame_%04d.ppm", (int)(t / diff));

		printf("Saving %s...\n", filename);
		layer_save_p3(layer, filename);
	}

	layer_destroy(layer);

	return 0;
}
