#include "rgba.h"

// ============== RGB 関数 ==============

RGB rgb_new(double r, double g, double b) {
    RGB rgb = {r, g, b};
    return rgb;
}

RGB rgb_add(RGB a, RGB b) {
    return rgb_new(a.r + b.r, a.g + b.g, a.b + b.b);
}

RGB rgb_mul(RGB rgb, double scalar) {
    return rgb_new(rgb.r * scalar, rgb.g * scalar, rgb.b * scalar);
}

RGB rgb_div(RGB rgb, double scalar) {
    return rgb_new(rgb.r / scalar, rgb.g / scalar, rgb.b / scalar);
}

RGB rgb_lerp(RGB from, RGB to, double t) {
    return rgb_new(
        from.r + (to.r - from.r) * t,
        from.g + (to.g - from.g) * t,
        from.b + (to.b - from.b) * t
    );
}

// ============== RGBA 関数 ==============

RGBA rgba_new(double r, double g, double b, double a) {
    RGBA rgba = {{r, g, b}, a};
    return rgba;
}

RGBA rgba_from_rgb(RGB rgb, double a) {
    RGBA rgba = {rgb, a};
    return rgba;
}

RGBA rgba_blend(RGBA dst, RGBA src) {
    double out_a = src.a + dst.a * (1.0 - src.a);

    if (out_a == 0.0) {
        return rgba_new(0.0, 0.0, 0.0, 0.0);
    }

    // out_rgb = (src.rgb * src.a + dst.rgb * dst.a * (1 - src.a)) / out_a
    RGB blended_rgb = rgb_div(
        rgb_add(
            rgb_mul(src.rgb, src.a),
            rgb_mul(dst.rgb, dst.a * (1.0 - src.a))
        ),
        out_a
    );

    return rgba_from_rgb(blended_rgb, out_a);
}

RGBA rgba_lerp(RGBA from, RGBA to, double t) {
    return rgba_from_rgb(
        rgb_lerp(from.rgb, to.rgb, t),
        from.a + (to.a - from.a) * t
    );
}
