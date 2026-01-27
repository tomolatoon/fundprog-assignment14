#include "h2511186/component.h"
#include <stdio.h>
#include <math.h>

void h2511186_component_init(void)
{
    // 特に初期化なし
}
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
    // マゼンタ → 完全透明
    if (src.rgb.r == 1.0 && src.rgb.g == 0.0 && src.rgb.b == 1.0) {
        return dst;
    }

    //アルファブレンド
    return rgba_blend(dst, src);
}

// ======================================================
// 太陽をレイヤーに描画（frame: 0〜59）
// ======================================================
void h2511186_sun_draw(HLayer layer, int frame)
{
    int W = (int)layer_get_width(layer);
    int H = (int)layer_get_height(layer);

    int ground_y = H - 100;
    float R = 250.0f;
    float cx = (float)W / 2.0f;
    float cy = (float)ground_y;

    float t = 1.0f - (float)frame / 450.0f;
    float theta = 3.14159265358979323846f * t;

    float sun_x = cx + R * cosf(theta);
    float sun_y = cy - R * sinf(theta);

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {

            float dx = (float)x - sun_x;
            float dy = (float)y - sun_y;
            float d = sqrtf(dx*dx + dy*dy);

            // 太陽本体
            if (d < 20.0f) {
                float k = fabsf(t - 0.5f) * 2.0f;

                float r1 = 255.0f, g1 = 180.0f, b1 = 100.0f;
                float r2 = 255.0f, g2 = 240.0f, b2 = 180.0f;

                double r = (double)(r1 * k + r2 * (1 - k)) / 255.0;
                double g = (double)(g1 * k + g2 * (1 - k)) / 255.0;
                double b = (double)(b1 * k + b2 * (1 - k)) / 255.0;

                layer_set_pixel(layer, POINT(x, y), rgba_new(r, g, b, 1.0));
                continue;
            }

            // グロー
            if (d < 60.0f) {
                float alpha = powf(1.0f - (d - 20.0f) / 40.0f, 2.0f);

                float k = fabsf(t - 0.5f) * 2.0f;

                float r1 = 255.0f, g1 = 180.0f, b1 = 100.0f;
                float r2 = 255.0f, g2 = 240.0f, b2 = 180.0f;

                double sr = (double)(r1 * k + r2 * (1 - k)) / 255.0;
                double sg = (double)(g1 * k + g2 * (1 - k)) / 255.0;
                double sb = (double)(b1 * k + b2 * (1 - k)) / 255.0;

                RGBA dst = layer_get_pixel(layer, POINT(x, y));
                RGBA src = rgba_new(sr, sg, sb, (double)alpha);

                layer_set_pixel(layer, POINT(x, y), rgba_blend(dst, src));
            }
        }
    }
}

// ======================================================
// ミク動画（600フレーム）
// ======================================================
void h2511186_miku_layer(HLayer layer)
{
    const int total_frames = 600;

    for (int i = 0; i < total_frames; i++)
{
    printf("Generating frame %d\n", i);

    HLayer frame_layer = layer_clone(layer);
    if (frame_layer == NULL) continue;

    // ============================================
    //  背景の切り替え（4種類 × 5秒）(マゼンタ透過)
    // ============================================
    const char* bg_file;
    if (i < 150) {
        bg_file = "components/h2511186/src/background/background1.ppm";
    } else if (i < 300) {
        bg_file = "components/h2511186/src/background/background2.ppm";
    } else if (i < 450) {
        bg_file = "components/h2511186/src/background/background3.ppm";
    } else {
        bg_file = "components/h2511186/src/background/background4.ppm";
    }

    HLayer bg = layer_load_p6(bg_file);
    if (bg != NULL) {
        layer_composite(frame_layer, bg, POINT(0, 0), NULL);
        layer_destroy(bg);
    }

    // ============================
    //  太陽アニメ（450フレーム）
    // ============================
	
    // 太陽（背景の上、地面の下）
    if (i < 450) {
     h2511186_sun_draw(frame_layer, i);
    }

    // ============================
    //  地面の切り替え（3種類）
    // ============================
    const char* ground_file;
    if (i < 300) {
        ground_file = "components/h2511186/src/ground/ground1.ppm";
    } else if (i < 450) {
        ground_file = "components/h2511186/src/ground/ground2.ppm";
    } else {
        ground_file = "components/h2511186/src/ground/ground3.ppm";
    }

    HLayer ground = layer_load_p6(ground_file);
    if (ground != NULL) {
        layer_composite(frame_layer, ground, POINT(0, 0), miku_blend);
        layer_destroy(ground);
    }

    // ============================
    //  ミク画像（マゼンタ透過）
    // ============================
    const char* ppm_file =
        ((i / 20) % 2 == 0)
        ? "components/h2511186/src/frame1.ppm"
        : "components/h2511186/src/frame2.ppm";

    HLayer img = layer_load_p6(ppm_file);
    if (img != NULL)
    {
        HLayer big = scale2x(img);   // ★ 2倍に拡大
        layer_destroy(img);

       layer_composite(frame_layer, big, POINT(480, 285), miku_blend);
       layer_destroy(big);
    }
    // ============================
    //  保存
    // ============================
    char filename[256];
    sprintf(filename, "output/frames/frame_%04d.ppm", i);
    layer_save_p6(frame_layer, filename);

    layer_destroy(frame_layer);
}

}