#ifndef I2511030_COMPONENT_H
#define I2511030_COMPONENT_H

/// @file component.h
/// @brief i2511030 開発者のコンポーネント

#include "layer/layer.h"

typedef HLayer (*GeneratorF)(double time);

typedef struct
{
	size_t     width;
	size_t     height;
	double     time_length;
	GeneratorF generator;
} video_layer;

HLayer i2511030_circle(Point center);

HLayer i2511030_sample_layer1_generator(double time);

// video_layer i2511030_sample_layer1();

HLayer i2511030_sample_layer2_generator(double time);

// video_layer i2511030_sample_layer2();

#endif // I2511030_COMPONENT_H
