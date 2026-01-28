#ifndef H2511186_COMPONENT_H
#define H2511186_COMPONENT_H

/// @file component.h
/// @brief h2511186 開発者のコンポーネント

#include "layer/layer.h"

void h2511186_component_init(void);
HLayer h2511186_background_layer(double time);
HLayer h2511186_sun_layer(double time);
HLayer h2511186_miku_layer(double time);
HLayer h2511186_ground_layer(double time);
HLayer scale2x(HLayer src);

#endif // H2511186_COMPONENT_H
