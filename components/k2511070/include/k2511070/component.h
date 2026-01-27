#ifndef K2511070_COMPONENT_H
#define K2511070_COMPONENT_H

/// @file component.h
/// @brief k2511070 開発者のコンポーネント

#include "layer/layer.h"

/// @brief k2511070 コンポーネントの初期化（ダミー）
void k2511070_component_init(void);

/// @brief k2511070 コンポーネントのサンプル描画（ダミー）
/// @param[in] layer 描画先レイヤー
void k2511070_component_draw(HLayer layer);

/// 一つの八分音符を描画する関数
HLayer one_eighth_note(RGBA c, HLayer layer, point p);

#endif // K2511070_COMPONENT_H
