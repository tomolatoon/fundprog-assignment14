/// @file component.c
/// @brief i2511030 開発者のコンポーネント実装（ダミー）

#include "i2511030/component.h"

Point lerp_point(Point a, Point b, double t)
{
	return POINT((int)(a.x + (b.x - a.x) * t), (int)(a.y + (b.y - a.y) * t));
}

HLayer i2511030_circle(Point center)
{
	HLayer layer = layer_create(SIZE(400, 400));

	layer_draw_circle(
		layer,
		center,
		80,
		(RGBA){
			{1.0, 0, 0},
			1.0
    }
	);

	return layer;
}

HLayer i2511030_sample_layer1_generator(double time)
{
	HLayer layer = layer_create(SIZE(400, 400));

	HLayer circle = i2511030_circle(lerp_point(POINT(20, 20), POINT(380, 380), time / 1.0));

	layer_composite(layer, circle, POINT(0, 0), NULL);

	layer_destroy(circle);

	return layer;
}

HLayer i2511030_sample_layer2_generator(double time)
{
	HLayer layer = layer_create(SIZE(400, 400));

	layer_draw_circle(
		layer,
		lerp_point(POINT(380, 20), POINT(20, 380), time / 1.0),
		80,
		(RGBA){
			{0, 1.0, 0},
			1.0
    }
	);

	return layer;
}
