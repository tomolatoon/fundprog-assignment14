#include "k2511070/component.h"

/// @file main.c
/// @brief 動画コンポーネント統合アプリケーション
#include "layer/layer.h"
#include <stdio.h>

typedef HLayer(*F)(double);

int main(void)
{
    printf("aaa\n");
	F f[] = {k2511070_bound_one_note_layer};

	HLayer layer = layer_create(SIZE(640, 480));

	double diff = 1 / 30.0;
	for (int j = 0; j*diff<=5.0; ++j)
	{
        double t = j*diff;
        layer_fill(
			layer,
			(RGBA){
				{1.0, 1.0, 1.0},
				1.0
        }
		);

		for (size_t i = 0; i < sizeof(f)/sizeof(f[0]); i++)
		{
			HLayer src = f[i](t);
			layer_composite(layer, src, POINT(0, 0), NULL);
			layer_destroy(src);
		}

		char filename[256];
		snprintf(filename, sizeof(filename), "output/frames/frame_%04d.ppm", j);

		printf("Saving %s...\n", filename);
		layer_save_p3(layer, filename);
	}

	layer_destroy(layer);

	return 0;
}
