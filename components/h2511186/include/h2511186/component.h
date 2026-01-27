#ifndef H2511186_COMPONENT_H
#define H2511186_COMPONENT_H

/// @file component.h
/// @brief h2511186 開発者のコンポーネント

#include "layer/layer.h"

// ミク動画（600フレーム）を生成する
void h2511186_miku_layer(HLayer layer);

// 太陽をレイヤーに描画する（frame: 0〜59）
void h2511186_sun_draw(HLayer layer, int frame);

HLayer scale2x(HLayer src);

#endif // H2511186_COMPONENT_H
