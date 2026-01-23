#include "rgba.h"

#include "unity.h"

// Unityに必要
void setUp(void) {}

void tearDown(void) {}

// ============== RGB テスト ==============

void test_rgb_new(void)
{
	RGB rgb = rgb_new(0.5, 0.3, 0.1);
	TEST_ASSERT_EQUAL_DOUBLE(0.5, rgb.r);
	TEST_ASSERT_EQUAL_DOUBLE(0.3, rgb.g);
	TEST_ASSERT_EQUAL_DOUBLE(0.1, rgb.b);
}

void test_rgb_add(void)
{
	RGB a      = rgb_new(0.2, 0.3, 0.4);
	RGB b      = rgb_new(0.1, 0.2, 0.3);
	RGB result = rgb_add(a, b);

	TEST_ASSERT_EQUAL_DOUBLE(0.3, result.r);
	TEST_ASSERT_EQUAL_DOUBLE(0.5, result.g);
	TEST_ASSERT_EQUAL_DOUBLE(0.7, result.b);
}

void test_rgb_mul(void)
{
	RGB rgb    = rgb_new(0.2, 0.4, 0.6);
	RGB result = rgb_mul(rgb, 0.5);

	TEST_ASSERT_EQUAL_DOUBLE(0.1, result.r);
	TEST_ASSERT_EQUAL_DOUBLE(0.2, result.g);
	TEST_ASSERT_EQUAL_DOUBLE(0.3, result.b);
}

void test_rgb_div(void)
{
	RGB rgb    = rgb_new(0.2, 0.4, 0.6);
	RGB result = rgb_div(rgb, 2.0);

	TEST_ASSERT_EQUAL_DOUBLE(0.1, result.r);
	TEST_ASSERT_EQUAL_DOUBLE(0.2, result.g);
	TEST_ASSERT_EQUAL_DOUBLE(0.3, result.b);
}

void test_rgb_lerp(void)
{
	RGB from = rgb_new(0.0, 0.0, 0.0);
	RGB to   = rgb_new(1.0, 1.0, 1.0);

	// t = 0
	RGB result0 = rgb_lerp(from, to, 0.0);
	TEST_ASSERT_EQUAL_DOUBLE(0.0, result0.r);
	TEST_ASSERT_EQUAL_DOUBLE(0.0, result0.g);
	TEST_ASSERT_EQUAL_DOUBLE(0.0, result0.b);

	// t = 0.5
	RGB result_half = rgb_lerp(from, to, 0.5);
	TEST_ASSERT_EQUAL_DOUBLE(0.5, result_half.r);
	TEST_ASSERT_EQUAL_DOUBLE(0.5, result_half.g);
	TEST_ASSERT_EQUAL_DOUBLE(0.5, result_half.b);

	// t = 1
	RGB result1 = rgb_lerp(from, to, 1.0);
	TEST_ASSERT_EQUAL_DOUBLE(1.0, result1.r);
	TEST_ASSERT_EQUAL_DOUBLE(1.0, result1.g);
	TEST_ASSERT_EQUAL_DOUBLE(1.0, result1.b);
}

// ============== RGBA テスト ==============

void test_rgba_new(void)
{
	RGBA rgba = rgba_new(0.1, 0.2, 0.3, 0.5);
	TEST_ASSERT_EQUAL_DOUBLE(0.1, rgba.rgb.r);
	TEST_ASSERT_EQUAL_DOUBLE(0.2, rgba.rgb.g);
	TEST_ASSERT_EQUAL_DOUBLE(0.3, rgba.rgb.b);
	TEST_ASSERT_EQUAL_DOUBLE(0.5, rgba.a);
}

void test_rgba_from_rgb(void)
{
	RGB  rgb  = rgb_new(0.5, 0.6, 0.7);
	RGBA rgba = rgba_from_rgb(rgb, 0.8);

	TEST_ASSERT_EQUAL_DOUBLE(0.5, rgba.rgb.r);
	TEST_ASSERT_EQUAL_DOUBLE(0.6, rgba.rgb.g);
	TEST_ASSERT_EQUAL_DOUBLE(0.7, rgba.rgb.b);
	TEST_ASSERT_EQUAL_DOUBLE(0.8, rgba.a);
}

void test_rgba_blend(void)
{
	// テスト: 不透明な前景色を何に重ねても前景色になる
	RGBA dst    = rgba_new(1.0, 0.0, 0.0, 1.0); // 赤
	RGBA src    = rgba_new(0.0, 1.0, 0.0, 1.0); // 緑 (不透明)
	RGBA result = rgba_blend(dst, src);

	TEST_ASSERT_EQUAL_DOUBLE(src.rgb.r, result.rgb.r);
	TEST_ASSERT_EQUAL_DOUBLE(src.rgb.g, result.rgb.g);
	TEST_ASSERT_EQUAL_DOUBLE(src.rgb.b, result.rgb.b);
	TEST_ASSERT_EQUAL_DOUBLE(src.a, result.a);

	// テスト: 完全に透明な前景色なら背景色が変わらない
	RGBA transparent_src = rgba_new(0.0, 0.0, 1.0, 0.0);
	RGBA result2         = rgba_blend(dst, transparent_src);

	TEST_ASSERT_EQUAL_DOUBLE(dst.rgb.r, result2.rgb.r);
	TEST_ASSERT_EQUAL_DOUBLE(dst.rgb.g, result2.rgb.g);
	TEST_ASSERT_EQUAL_DOUBLE(dst.rgb.b, result2.rgb.b);
	TEST_ASSERT_EQUAL_DOUBLE(dst.a, result2.a);

	// テスト: 50% アルファブレンド
	RGBA half_src = rgba_new(0.0, 1.0, 0.0, 0.5); // 緑 (50%)
	RGBA result3  = rgba_blend(dst, half_src);
	// expected out_a = 0.5 + 1.0 * 0.5 = 1.0
	// expected out_rgb = (green * 0.5 + red * 1.0 * 0.5) / 1.0 = (0.5, 0.5, 0)

	TEST_ASSERT_EQUAL_DOUBLE(0.5, result3.rgb.r);
	TEST_ASSERT_EQUAL_DOUBLE(0.5, result3.rgb.g);
	TEST_ASSERT_EQUAL_DOUBLE(0.0, result3.rgb.b);
	TEST_ASSERT_EQUAL_DOUBLE(1.0, result3.a);
}

void test_rgba_lerp(void)
{
	RGBA from = rgba_new(0.0, 0.0, 0.0, 0.0);
	RGBA to   = rgba_new(1.0, 1.0, 1.0, 1.0);

	// t = 0
	RGBA result0 = rgba_lerp(from, to, 0.0);
	TEST_ASSERT_EQUAL_DOUBLE(0.0, result0.rgb.r);
	TEST_ASSERT_EQUAL_DOUBLE(0.0, result0.a);

	// t = 0.5
	RGBA result_half = rgba_lerp(from, to, 0.5);
	TEST_ASSERT_EQUAL_DOUBLE(0.5, result_half.rgb.r);
	TEST_ASSERT_EQUAL_DOUBLE(0.5, result_half.rgb.g);
	TEST_ASSERT_EQUAL_DOUBLE(0.5, result_half.rgb.b);
	TEST_ASSERT_EQUAL_DOUBLE(0.5, result_half.a);

	// t = 1
	RGBA result1 = rgba_lerp(from, to, 1.0);
	TEST_ASSERT_EQUAL_DOUBLE(1.0, result1.rgb.r);
	TEST_ASSERT_EQUAL_DOUBLE(1.0, result1.a);
}

// ============== 配列変換テスト ==============

void test_rgb_to_array(void)
{
	RGB    rgb = rgb_new(0.1, 0.2, 0.3);
	double array[3];
	rgb_to_array(rgb, array);
	TEST_ASSERT_EQUAL_DOUBLE(0.1, array[0]);
	TEST_ASSERT_EQUAL_DOUBLE(0.2, array[1]);
	TEST_ASSERT_EQUAL_DOUBLE(0.3, array[2]);
}

void test_rgb_from_array(void)
{
	double array[3] = {0.4, 0.5, 0.6};
	RGB    rgb      = rgb_from_array(array);
	TEST_ASSERT_EQUAL_DOUBLE(0.4, rgb.r);
	TEST_ASSERT_EQUAL_DOUBLE(0.5, rgb.g);
	TEST_ASSERT_EQUAL_DOUBLE(0.6, rgb.b);
}

void test_rgba_to_array(void)
{
	RGBA   rgba = rgba_new(0.1, 0.2, 0.3, 0.5);
	double array[4];
	rgba_to_array(rgba, array);
	TEST_ASSERT_EQUAL_DOUBLE(0.1, array[0]);
	TEST_ASSERT_EQUAL_DOUBLE(0.2, array[1]);
	TEST_ASSERT_EQUAL_DOUBLE(0.3, array[2]);
	TEST_ASSERT_EQUAL_DOUBLE(0.5, array[3]);
}

void test_rgba_from_array(void)
{
	double array[4] = {0.7, 0.8, 0.9, 1.0};
	RGBA   rgba     = rgba_from_array(array);
	TEST_ASSERT_EQUAL_DOUBLE(0.7, rgba.rgb.r);
	TEST_ASSERT_EQUAL_DOUBLE(0.8, rgba.rgb.g);
	TEST_ASSERT_EQUAL_DOUBLE(0.9, rgba.rgb.b);
	TEST_ASSERT_EQUAL_DOUBLE(1.0, rgba.a);
}

// ============== Main ==============

int main(void)
{
	UNITY_BEGIN();

	// RGB テスト
	RUN_TEST(test_rgb_new);
	RUN_TEST(test_rgb_add);
	RUN_TEST(test_rgb_mul);
	RUN_TEST(test_rgb_div);
	RUN_TEST(test_rgb_lerp);
	RUN_TEST(test_rgb_to_array);
	RUN_TEST(test_rgb_from_array);

	// RGBA テスト
	RUN_TEST(test_rgba_new);
	RUN_TEST(test_rgba_from_rgb);
	RUN_TEST(test_rgba_blend);
	RUN_TEST(test_rgba_lerp);
	RUN_TEST(test_rgba_to_array);
	RUN_TEST(test_rgba_from_array);

	return UNITY_END();
}

