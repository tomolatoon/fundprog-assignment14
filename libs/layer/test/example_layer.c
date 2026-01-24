/// @file example_layer.c
/// @brief Layer ライブラリの機能デモ
///
/// 各種描画関数を使用して画像を生成し、output/examples/ に保存します。
/// ビルド後、./build/debug/libs/layer/example_layer で実行できます。

#include "layer/layer.h"
#include <stdio.h>

// ============================================================================
// 描画関数比較（左: AA なし、右: AA あり）
// ============================================================================

/// @brief 線描画の比較（Bresenham vs Wu）
static void example_draw_line(void)
{
	printf("=== 線描画 ===\n");

	HLayer layer = layer_create(SIZE(400, 200));
	layer_fill(layer, rgba_new(1.0, 1.0, 1.0, 1.0));

	// 中央線
	layer_draw_line(layer, POINT(200, 0), POINT(200, 200), 1, rgba_new(0.8, 0.8, 0.8, 1.0));

	// 左: AA なし（Bresenham）
	layer_draw_line(layer, POINT(20, 180), POINT(180, 20), 1, rgba_new(0.0, 0.0, 0.0, 1.0));
	layer_draw_line(layer, POINT(40, 180), POINT(160, 20), 2, rgba_new(1.0, 0.0, 0.0, 1.0));
	layer_draw_line(layer, POINT(60, 180), POINT(140, 20), 3, rgba_new(0.0, 0.5, 0.0, 1.0));

	// 右: AA あり（長方形ポリゴン）
	layer_draw_line_aa(layer, POINT(220, 180), POINT(380, 20), 1, rgba_new(0.0, 0.0, 0.0, 1.0));
	layer_draw_line_aa(layer, POINT(240, 180), POINT(360, 20), 2, rgba_new(1.0, 0.0, 0.0, 1.0));
	layer_draw_line_aa(layer, POINT(260, 180), POINT(340, 20), 3, rgba_new(0.0, 0.5, 0.0, 1.0));

	layer_save_p6(layer, "output/examples/draw_line.ppm");
	printf("Saved: output/examples/draw_line.ppm\n");
	layer_destroy(layer);
}

/// @brief 矩形描画の比較
static void example_draw_rect(void)
{
	printf("=== 矩形描画 ===\n");

	HLayer layer = layer_create(SIZE(400, 200));
	layer_fill(layer, rgba_new(1.0, 1.0, 1.0, 1.0));

	// 中央線
	layer_draw_line(layer, POINT(200, 0), POINT(200, 200), 1, rgba_new(0.8, 0.8, 0.8, 1.0));

	// 左: AA なし
	layer_draw_rect(layer, POINT(20, 20), SIZE(160, 80), rgba_new(1.0, 0.0, 0.0, 1.0));
	layer_draw_rect(layer, POINT(40, 110), SIZE(120, 70), rgba_new(0.0, 0.5, 0.0, 0.7));

	// 右: AA あり
	layer_draw_rect_aa(layer, POINT(220, 20), SIZE(160, 80), rgba_new(1.0, 0.0, 0.0, 1.0));
	layer_draw_rect_aa(layer, POINT(240, 110), SIZE(120, 70), rgba_new(0.0, 0.5, 0.0, 0.7));

	layer_save_p6(layer, "output/examples/draw_rect.ppm");
	printf("Saved: output/examples/draw_rect.ppm\n");
	layer_destroy(layer);
}

/// @brief 円描画の比較
static void example_draw_circle(void)
{
	printf("=== 円描画 ===\n");

	HLayer layer = layer_create(SIZE(400, 200));
	layer_fill(layer, rgba_new(1.0, 1.0, 1.0, 1.0));

	// 中央線
	layer_draw_line(layer, POINT(200, 0), POINT(200, 200), 1, rgba_new(0.8, 0.8, 0.8, 1.0));

	// 左: AA なし
	layer_draw_circle(layer, POINT(100, 100), 70, rgba_new(0.0, 0.5, 0.8, 1.0));
	layer_draw_circle(layer, POINT(60, 60), 30, rgba_new(1.0, 0.5, 0.0, 0.7));

	// 右: AA あり
	layer_draw_circle_aa(layer, POINT(300, 100), 70, rgba_new(0.0, 0.5, 0.8, 1.0));
	layer_draw_circle_aa(layer, POINT(260, 60), 30, rgba_new(1.0, 0.5, 0.0, 0.7));

	layer_save_p6(layer, "output/examples/draw_circle.ppm");
	printf("Saved: output/examples/draw_circle.ppm\n");
	layer_destroy(layer);
}

/// @brief 三角形描画の比較
static void example_draw_triangle(void)
{
	printf("=== 三角形描画 ===\n");

	HLayer layer = layer_create(SIZE(400, 200));
	layer_fill(layer, rgba_new(1.0, 1.0, 1.0, 1.0));

	// 中央線
	layer_draw_line(layer, POINT(200, 0), POINT(200, 200), 1, rgba_new(0.8, 0.8, 0.8, 1.0));

	// 左: AA なし
	layer_draw_triangle(layer, POINT(100, 20), POINT(180, 180), POINT(20, 180), rgba_new(0.8, 0.0, 0.3, 1.0));

	// 右: AA あり
	layer_draw_triangle_aa(layer, POINT(300, 20), POINT(380, 180), POINT(220, 180), rgba_new(0.8, 0.0, 0.3, 1.0));

	layer_save_p6(layer, "output/examples/draw_triangle.ppm");
	printf("Saved: output/examples/draw_triangle.ppm\n");
	layer_destroy(layer);
}

/// @brief 多角形描画の比較
static void example_draw_polygon(void)
{
	printf("=== 多角形描画 ===\n");

	HLayer layer = layer_create(SIZE(400, 200));
	layer_fill(layer, rgba_new(1.0, 1.0, 1.0, 1.0));

	// 中央線
	layer_draw_line(layer, POINT(200, 0), POINT(200, 200), 1, rgba_new(0.8, 0.8, 0.8, 1.0));

	// 五角形の頂点
	Point pentagon_l[] = {POINT(100, 30), POINT(170, 60), POINT(150, 140), POINT(50, 140), POINT(30, 60)};
	Point pentagon_r[] = {POINT(300, 30), POINT(370, 60), POINT(350, 140), POINT(250, 140), POINT(230, 60)};

	// 左: AA なし
	layer_draw_polygon(layer, pentagon_l, 5, rgba_new(0.5, 0.0, 0.8, 1.0));

	// 右: AA あり
	layer_draw_polygon_aa(layer, pentagon_r, 5, rgba_new(0.5, 0.0, 0.8, 1.0));

	layer_save_p6(layer, "output/examples/draw_polygon.ppm");
	printf("Saved: output/examples/draw_polygon.ppm\n");
	layer_destroy(layer);
}

// ============================================================================
// その他の機能デモ
// ============================================================================

/// @brief レイヤー合成
static void example_composite(void)
{
	printf("=== レイヤー合成 ===\n");

	// 背景レイヤー
	HLayer bg = layer_create(SIZE(400, 200));
	layer_fill(bg, rgba_new(0.2, 0.2, 0.4, 1.0));

	// 前景レイヤー（半透明円）
	HLayer fg = layer_create(SIZE(150, 150));
	layer_fill(fg, rgba_new(0.0, 0.0, 0.0, 0.0));
	layer_draw_circle_aa(fg, POINT(75, 75), 60, rgba_new(1.0, 0.5, 0.0, 0.7));

	// 合成
	layer_composite(bg, fg, POINT(50, 25), NULL);
	layer_composite(bg, fg, POINT(200, 25), NULL);

	layer_save_p6(bg, "output/examples/composite.ppm");
	printf("Saved: output/examples/composite.ppm\n");
	layer_destroy(bg);
	layer_destroy(fg);
}

/// @brief 変形関数デモ
static void example_transforms(void)
{
	printf("=== 変形 ===\n");

	// 元画像
	HLayer original = layer_create(SIZE(200, 150));
	layer_fill(original, rgba_new(1.0, 1.0, 1.0, 1.0));
	layer_draw_rect(original, POINT(10, 10), SIZE(80, 60), rgba_new(1.0, 0.0, 0.0, 1.0));
	layer_draw_triangle(original, POINT(150, 20), POINT(190, 80), POINT(110, 80), rgba_new(0.0, 0.0, 1.0, 1.0));
	layer_save_p6(original, "output/examples/transform_original.ppm");
	printf("Saved: output/examples/transform_original.ppm\n");

	// 水平反転
	HLayer flipped_h = layer_flip_horizontal_to(original);
	layer_save_p6(flipped_h, "output/examples/transform_flip_h.ppm");
	printf("Saved: output/examples/transform_flip_h.ppm\n");
	layer_destroy(flipped_h);

	// 垂直反転
	HLayer flipped_v = layer_flip_vertical_to(original);
	layer_save_p6(flipped_v, "output/examples/transform_flip_v.ppm");
	printf("Saved: output/examples/transform_flip_v.ppm\n");
	layer_destroy(flipped_v);

	// リサイズ（Bilinear補間）
	HLayer resized = layer_resize_bilinear_to(original, SIZE(100, 75));
	layer_save_p6(resized, "output/examples/transform_resize.ppm");
	printf("Saved: output/examples/transform_resize.ppm\n");
	layer_destroy(resized);

	// クリッピング
	HLayer clipped = layer_clip_to(original, POINT(10, 10), SIZE(80, 60));
	layer_save_p6(clipped, "output/examples/transform_clip.ppm");
	printf("Saved: output/examples/transform_clip.ppm\n");
	layer_destroy(clipped);

	layer_destroy(original);
}

/// @brief スーパーサンプリングデモ
static void example_supersampling(void)
{
	printf("=== スーパーサンプリング ===\n");

	// 2倍サイズで描画
	HLayer big = layer_create(SIZE(400, 400));
	layer_fill(big, rgba_new(1.0, 1.0, 1.0, 1.0));
	layer_draw_circle(big, POINT(200, 200), 150, rgba_new(0.0, 0.5, 0.8, 1.0));
	layer_draw_line(big, POINT(50, 50), POINT(350, 350), 4, rgba_new(0.8, 0.0, 0.0, 1.0));

	// ダウンサンプル（AA効果）
	HLayer small = layer_downsample_2x_to(big);
	layer_save_p6(small, "output/examples/supersampled.ppm");
	printf("Saved: output/examples/supersampled.ppm\n");

	layer_destroy(big);
	layer_destroy(small);
}

int main(void)
{
	printf("Layer ライブラリ機能デモ\n");
	printf("出力先: output/examples/\n");
	printf("形式: 左=AA なし / 右=AA あり\n\n");

	// 塗りつぶし描画
	example_draw_line();
	example_draw_rect();
	example_draw_circle();
	example_draw_triangle();
	example_draw_polygon();


	// その他
	example_composite();
	example_transforms();
	example_supersampling();

	printf("\n完了! 画像を確認してください。\n");
	return 0;
}
