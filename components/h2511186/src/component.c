#include "h2511186/component.h"
#include <stdio.h>
#include <math.h>

//=============
// 2倍拡大関数
//=============
HLayer scale2x(HLayer src)
{
    int w = (int)layer_get_width(src);
    int h = (int)layer_get_height(src);

    int new_w = w * 2;
    int new_h = h * 2;

    HLayer dst = layer_create(SIZE((size_t)new_w, (size_t)new_h));
    if (!dst) return NULL;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            RGBA c = layer_get_pixel(src, POINT(x, y));

            layer_set_pixel(dst, POINT(x*2,   y*2),   c);
            layer_set_pixel(dst, POINT(x*2+1, y*2),   c);
            layer_set_pixel(dst, POINT(x*2,   y*2+1), c);
            layer_set_pixel(dst, POINT(x*2+1, y*2+1), c);
        }
    }

    return dst;
}

// ======================================================
// マゼンタ透過用 BlendFunc
// ======================================================
static RGBA miku_blend(RGBA dst, RGBA src)
{
    // マゼンタ近似判定（誤差許容）
    if (fabs(src.rgb.r - 1.0) < 0.01 &&
        fabs(src.rgb.g - 0.0) < 0.01 &&
        fabs(src.rgb.b - 1.0) < 0.01)
    {
        return dst;
    }

    return rgba_blend(dst, src);
}


// ======================================================
// 背景を生成
// ======================================================
HLayer h2511186_background_layer(double time)
{
    const char* bg_file;

    if (time < 2.5) {
        bg_file = "components/h2511186/src/background/background1.ppm";
    } else if (time < 5.0) {
        bg_file = "components/h2511186/src/background/background2.ppm";
    } else if (time < 7.5) {
        bg_file = "components/h2511186/src/background/background3.ppm";
    } else {
        bg_file = "components/h2511186/src/background/background4.ppm";
    }

    return layer_load_p6(bg_file); 
}

// ======================================================
// 太陽を生成
// ======================================================
HLayer h2511186_sun_layer(double time)
{
    HLayer sun = layer_create(SIZE(640, 480));
    if (!sun) return NULL;

    int W = 640, H = 480;
    int ground_y = H - 100;
    float R = 250.0f;
    float cx = (float)W / 2.0f;
    float cy = (float)ground_y;

    float t = 1.0f - (float)(time / 7.5);  
    if (t < 0.0f) return sun;


    float theta = 3.14159265358979323846f * t;
    float sun_x = cx + R * cosf(theta);
    float sun_y = cy - R * sinf(theta);

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            float dx = (float)x - sun_x;
            float dy = (float)y - sun_y;
            float d = sqrtf(dx*dx + dy*dy);

            float k = fabsf(t - 0.5f) * 2.0f;
            float r1 = 255.0f, g1 = 180.0f, b1 = 100.0f;
            float r2 = 255.0f, g2 = 240.0f, b2 = 180.0f;

            float r = (r1 * k + r2 * (1 - k)) / 255.0f;
            float g = (g1 * k + g2 * (1 - k)) / 255.0f;
            float b = (b1 * k + b2 * (1 - k)) / 255.0f;

            if (d < 20.0f) {
                layer_set_pixel(sun, POINT(x, y), rgba_new(r, g, b, 1.0));
            } else if (d < 60.0f) {
                float alpha = powf(1.0f - (d - 20.0f) / 40.0f, 2.0f) * (1.0f - (1.0f - t));
                RGBA dst = layer_get_pixel(sun, POINT(x, y));
                RGBA src = rgba_new(r, g, b, alpha);
                layer_set_pixel(sun, POINT(x, y), rgba_blend(dst, src));
            }
        }
    }

    return sun;
}

// ======================================================
// ミクちゃんを生成
// ======================================================
HLayer h2511186_miku_layer(double time)
{
    const char* ppm_file = ((int)(time * 6) % 2 == 0)
        ? "components/h2511186/src/frame1.ppm" : "components/h2511186/src/frame2.ppm";

    HLayer img = layer_load_p6(ppm_file);
    if (!img) return NULL;

    HLayer big = scale2x(img);
    layer_destroy(img);

    HLayer miku = layer_create(SIZE(640, 480));
    if (!miku || !big) {
        layer_destroy(big);
        return miku;
    }

    layer_composite(miku, big, POINT(480, 285), miku_blend);
    layer_destroy(big);
    return miku;
}

// ======================================================
//　地面を生成
// ======================================================
HLayer h2511186_ground_layer(double time)
{
    const char* ground_file = (time < 5.0)
        ? "components/h2511186/src/ground/ground1.ppm"
        : (time < 7.5)
            ? "components/h2511186/src/ground/ground2.ppm"
            : "components/h2511186/src/ground/ground3.ppm";

    // 元画像を読み込む
    HLayer raw = layer_load_p6(ground_file);
    if (!raw) return NULL;

    // 出力レイヤー
    HLayer ground = layer_create(SIZE(640, 480));
    if (!ground) {
        layer_destroy(raw);
        return NULL;
    }

    layer_fill(ground, rgba_new(0,0,0,0));  // 完全透明

    // マゼンタを透明としてコピー
    layer_composite(ground, raw, POINT(0,0), miku_blend);

    layer_destroy(raw);
    return ground;
}

