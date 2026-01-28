/// @file component.c
/// @brief k2511070 開発者のコンポーネント実装（ダミー）

#include "k2511070/component.h"
#include <math.h>
#include <stdio.h>

void k2511070_component_init(void)
{
	// TODO: 初期化処理
}

//一個の八分音符っを描画する関数
HLayer one_eighth_note(RGBA c, Point p){
	HLayer layer = layer_create(SIZE(640,480));
	RGBA c1;
	int dx = 0;
	int dy = 0;
	RGBA black = rgba_new(0.0, 0.0, 0.0, 1.0);
	RGBA white = rgba_new(1.0, 1.0, 1.0, 1.0);

	int note[150] = {3,3,3,3,1,1,1,1,3,3,
					 3,3,3,3,1,2,2,1,1,3,
					 3,3,3,3,1,2,2,2,1,1,
					 3,3,3,3,1,2,1,2,2,1,
					 3,3,3,3,1,2,1,1,2,1,
					 3,3,3,3,1,2,1,1,2,1,
					 3,3,3,3,1,2,1,1,2,1,
					 3,3,3,3,1,2,1,1,2,1,
					 3,1,1,1,1,2,1,2,1,1,
					 1,1,2,2,2,2,1,1,1,3,
					 1,2,2,2,0,2,1,3,3,3,
					 1,2,2,2,2,2,1,3,3,3,
					 1,2,2,2,2,2,1,3,3,3,
					 1,2,2,2,2,1,1,3,3,3,
					 1,1,1,1,1,1,3,3,3,3,};

	int i;
    for(i = 0;i < 150;i++){
        if(note[i] == 3){
			//do nothing
		}else if(note[i] == 0){
			c1 = white;
		}else if(note[i] == 1){
			c1 = black;
		}else{
			c1 = c;
		}

		for(int j = 0; j < 2; j++) {
            for(int k = 0; k < 2; k++) {	
                dx = 2 * (i % 10) + k;
                dy = 2 * (i / 10) + j;

                Point draw_p = POINT(p.x + dx, p.y + dy);

                layer_set_pixel(layer, draw_p, c1);
            }
        }
	}
	return layer;
}

HLayer two_eighth_note(RGBA c, Point p){
	HLayer layer = layer_create(SIZE(640,480));
    RGBA c1;
	int dx = 0;
	int dy = 0;
	RGBA black = rgba_new(0.0, 0.0, 0.0, 1.0);
	RGBA white = rgba_new(1.0, 1.0, 1.0, 1.0);

	int note[132] = {3,3,3,1,1,1,1,1,1,1,1,1,
                     3,3,3,1,2,2,2,2,2,2,2,1,
                     3,3,3,1,2,1,1,1,1,1,2,1,
                     3,3,3,1,2,1,3,3,3,1,2,1,
                     3,3,3,1,2,1,3,3,3,1,2,1,
                     3,1,1,1,2,1,3,1,1,1,2,1,
                     1,1,2,2,2,1,1,1,2,2,2,1,
                     1,2,2,0,2,1,1,2,2,0,2,1,
                     1,2,2,2,2,1,1,2,2,2,2,1,
                     1,2,2,2,1,1,1,2,2,2,1,1,
                     1,1,1,1,1,3,1,1,1,1,1,3,};

	int i;
    for(i = 0;i < 132;i++){
        if(note[i] == 3){
			//do nothing
		}else if(note[i] == 0){
			c1 = white;
		}else if(note[i] == 1){
			c1 = black;
		}else{
			c1 = c;
		}

		for(int j = 0; j < 2; j++) {
            for(int k = 0; k < 2; k++) {	
                dx = 2 * (i % 12) + k;
                dy = 2 * (i / 12) + j;

                Point draw_p = POINT(p.x + dx, p.y + dy);

                layer_set_pixel(layer, draw_p, c1);
            }
        }
	}
	return layer;
}

//一つの八分音符が4つ画面上をはねるアニメーション.背景太陽のレイヤーとミクの間
HLayer k2511070_bound_one_note_layer(double time){
	double y = 270;
	double v = 0;
	double g = 2;
	double x_mod;
	int isfalling = 1;
	double x_speed = 50;
	double diff;

	HLayer layer = layer_create(SIZE(640,480));

	RGBA c1 = rgba_new(1.0, 0.5, 0.0, 0.25);
	RGBA c2 = rgba_new(1.0, 0.65, 0.0, 1.0);
	RGBA c3 = rgba_new(0.0, 0.75, 1.0, 1.0);
	RGBA c4 = rgba_new(0.6, 0.2, 0.8, 1.0);

	Point p1 = {0,(int)y};
	Point p2 = {160,(int)y};
	Point p3 = {320,(int)y};
	Point p4 = {480,(int)y};

	x_mod = fmod(x_speed * time, 640.0);

	p1.x = (int)(p1.x + x_speed * time / 640.0 > 1 ? p1.x + x_mod : p1.x + x_speed);
	p2.x = (int)(p2.x + x_speed * time / 640.0 > 1 ? p2.x + x_mod : p2.x + x_speed);
	p3.x = (int)(p3.x + x_speed * time / 640.0 > 1 ? p3.x + x_mod : p3.x + x_speed);
	p4.x = (int)(p4.x + x_speed * time / 640.0 > 1 ? p4.x + x_mod : p4.x + x_speed);

	isfalling = fmod((double)time * 2,2.0) == 0 ? 1 : 0;

	if(isfalling == 1){
		diff = fmod(time,1.0);
		int n = (int)diff * 15;
		for(int i = 0;i < n;i++){
			y += v;
			v += g / 15.0;
		}
	}else{
		diff = fmod(time,1);
		int n = (int)diff * 15;
		for(int i = 0;i < 15 - n;i++){
			y += v;
			v += g / 15;
		}
	}

	p1.y = (int)y;
	p2.y = (int)y;
	p3.y = (int)y;
	p4.y = (int)y;

	HLayer note1 = one_eighth_note(c1, p1);
	HLayer note2 = one_eighth_note(c2, p2);
	HLayer note3 = one_eighth_note(c3, p3);
	HLayer note4 = one_eighth_note(c4, p4);

	layer_composite(layer, note1, p1, NULL);
	layer_composite(layer, note2, p2, NULL);
	layer_composite(layer, note3, p3, NULL);
	layer_composite(layer, note4, p4, NULL);

	layer_destroy(note1);
	layer_destroy(note2);
	layer_destroy(note3);
	layer_destroy(note4);

	return layer;
}
