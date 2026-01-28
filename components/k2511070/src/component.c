/// @file component.c
/// @brief k2511070 開発者のコンポーネント実装（ダミー）

#include "k2511070/component.h"
#include <math.h>
#include <stdio.h>

void k2511070_component_init(void)
{
	// TODO: 初期化処理
}

//一個の八分音符を描画する関数
HLayer one_eighth_note(RGBA c, Point p){
	HLayer layer = layer_create(SIZE(640,480));
	
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
					 1,1,1,1,1,1,3,3,3,3};

	int i;
    for(i = 0;i < 150;i++){
        if(note[i] == 3){
			//do nothing
		}else if(note[i] == 0){
			for(int j = 0; j < 3; j++) {
				for(int k = 0; k < 3; k++) {	
					dx = 3 * (i % 10) + k;
					dy = 3 * (i / 10) + j;
				
					Point draw_p = POINT(p.x + dx, p.y + dy);
				
					layer_set_pixel(layer, draw_p, white);
				}
			}
		}else if(note[i] == 1){
			for(int j = 0; j < 3; j++) {
				for(int k = 0; k < 3; k++) {	
					dx = 3 * (i % 10) + k;
					dy = 3 * (i / 10) + j;
				
					Point draw_p = POINT(p.x + dx, p.y + dy);
				
					layer_set_pixel(layer, draw_p, black);
				}
			}
		}else{
			for(int j = 0; j < 3; j++) {
				for(int k = 0; k < 3; k++) {	
					dx = 3 * (i % 10) + k;
					dy = 3 * (i / 10) + j;
				
					Point draw_p = POINT(p.x + dx, p.y + dy);
				
					layer_set_pixel(layer, draw_p, c);
				}
			}
		}
	}
	return layer;
}

HLayer two_eighth_note(RGBA c, Point p){
	HLayer layer = layer_create(SIZE(640,480));
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
			for(int j = 0; j < 3; j++) {
				for(int k = 0; k < 3; k++) {	
					dx = 3 * (i % 12) + k;
					dy = 3 * (i / 12) + j;
				
					Point draw_p = POINT(p.x + dx, p.y + dy);
				
					layer_set_pixel(layer, draw_p, white);
				}
			}
		}else if(note[i] == 1){
			for(int j = 0; j < 3; j++) {
				for(int k = 0; k < 3; k++) {	
					dx = 3 * (i % 12) + k;
					dy = 3 * (i / 12) + j;
				
					Point draw_p = POINT(p.x + dx, p.y + dy);
				
					layer_set_pixel(layer, draw_p, black);
				}
			}
		}else{
			for(int j = 0; j < 3; j++) {
				for(int k = 0; k < 3; k++) {	
					dx = 3 * (i % 12) + k;
					dy = 3 * (i / 12) + j;
				
					Point draw_p = POINT(p.x + dx, p.y + dy);
				
					layer_set_pixel(layer, draw_p, c);
				}
			}
		}
	}
	return layer;
}

//一つの八分音符が4つ画面上をスクロールするアニメーション.背景太陽のレイヤーとミクの間
HLayer k2511070_bound_one_note_layer(double time){
	RGBA white = rgba_new(1.0, 1.0, 1.0, 0.0);
	double y = 270;
	double x_speed = 200;

	HLayer layer = layer_create(SIZE(640,480));
	layer_fill(layer,white);

	RGBA c1 = rgba_new(1.0, 0.4, 0.15, 1.0);
	RGBA c2 = rgba_new(1.0, 0.65, 0.0, 1.0);
	RGBA c3 = rgba_new(0.0, 0.75, 1.0, 1.0);
	RGBA c4 = rgba_new(0.6, 0.2, 0.8, 1.0);

	Point p0 = {0.0};
	Point p1 = {0,(int)y};
	Point p2 = {160,(int)y};
	Point p3 = {320,(int)y};
	Point p4 = {480,(int)y};

	p1.x = (int)(p1.x + x_speed * time > 640.0 ? fmod(p1.x + x_speed * time,640.0) : p1.x + x_speed * time);
	p2.x = (int)(p2.x + x_speed * time > 640.0 ? fmod(p2.x + x_speed * time,640.0) : p2.x + x_speed * time);
	p3.x = (int)(p3.x + x_speed * time > 640.0 ? fmod(p3.x + x_speed * time,640.0) : p3.x + x_speed * time);
	p4.x = (int)(p4.x + x_speed * time > 640.0 ? fmod(p4.x + x_speed * time,640.0) : p4.x + x_speed * time);

	p1.y = (int)y;
	p2.y = (int)y;
	p3.y = (int)y;
	p4.y = (int)y;

	layer_fill(layer,white);
	HLayer note1 = one_eighth_note(c1, p1);
	HLayer note2 = one_eighth_note(c2, p2);
	HLayer note3 = one_eighth_note(c3, p3);
	HLayer note4 = one_eighth_note(c4, p4);

	layer_composite(layer, note1, p0, NULL);
	layer_composite(layer, note2, p0, NULL);
	layer_composite(layer, note3, p0, NULL);
	layer_composite(layer, note4, p0, NULL);

	layer_destroy(note1);
	layer_destroy(note2);
	layer_destroy(note3);
	layer_destroy(note4);

	return layer;
}

//2つの八分音符が3つ画面上をスクロールするアニメーション.背景太陽のレイヤーとミクの間
HLayer k2511070_bound_two_note_layer(double time){
	RGBA white = rgba_new(1.0, 1.0, 1.0, 0.0);
	double y = 320;
	double x_speed = 200;

	HLayer layer = layer_create(SIZE(640,480));
	layer_fill(layer,white);

	RGBA c1 = rgba_new(1.0, 0.5, 0.42, 1.0);
	RGBA c2 = rgba_new(0.0, 0.0, 0.44, 1.0);
	RGBA c3 = rgba_new(0.0, 0.81, 0.55, 1.0);
	RGBA c4 = rgba_new(0.1, 0.1, 0.4, 1.0);

	Point p0 = {0.0};
	Point p1 = {80,(int)y};
	Point p2 = {240,(int)y};
	Point p3 = {400,(int)y};
	Point p4 = {560,(int)y};

	p1.x = (int)(p1.x + x_speed * time > 640.0 ? fmod(p1.x + x_speed * time,640.0) : p1.x + x_speed * time);
	p2.x = (int)(p2.x + x_speed * time > 640.0 ? fmod(p2.x + x_speed * time,640.0) : p2.x + x_speed * time);
	p3.x = (int)(p3.x + x_speed * time > 640.0 ? fmod(p3.x + x_speed * time,640.0) : p3.x + x_speed * time);
	p4.x = (int)(p4.x + x_speed * time > 640.0 ? fmod(p4.x + x_speed * time,640.0) : p4.x + x_speed * time);

	p1.y = (int)y;
	p2.y = (int)y;
	p3.y = (int)y;
	p4.y = (int)y;

	layer_fill(layer,white);
	HLayer note1 = two_eighth_note(c1, p1);
	HLayer note2 = two_eighth_note(c2, p2);
	HLayer note3 = two_eighth_note(c3, p3);
	HLayer note4 = two_eighth_note(c4, p4);

	layer_composite(layer, note1, p0, NULL);
	layer_composite(layer, note2, p0, NULL);
	layer_composite(layer, note3, p0, NULL);
	layer_composite(layer, note4, p0, NULL);

	layer_destroy(note1);
	layer_destroy(note2);
	layer_destroy(note3);
	layer_destroy(note4);

	return layer;
}

//二色の長方形で画面切り替えをする関数.一番上のレイヤーに
HLayer change(double time){
	RGBA white = rgba_new(1.0, 1.0, 1.0, 0.0);
	RGBA lightblue = rgba_new(0.53, 0.81, 1.0, 1.0);
	RGBA blue = rgba_new(0.0, 0.2, 1.0, 1.0);
	RGBA orange = rgba_new(1.0, 0.55, 0.0, 1.0);
	RGBA darkblue = rgba_new(0.1, 0.1, 0.44, 1.0);
	RGBA c1;
	HLayer layer = layer_create(SIZE(640,480));
	layer_fill(layer,white);
	Size s = SIZE(640 / 16,480 * 2);

	int v = 5760;
	double t1 = 2.5;
	double t2 = 5.0;
	double t3 = 7.5;
	Point p = {0,-960};
	int dx = 640 / 16;
	double dy = (double)p.y / 16.0;

	int i;
	if(t1 - 1 <= time && time <= t1 + 1){
		p.y += (int)(v * (time - t1));
		for(i = 0;i < 16;i++){
			c1 = i % 2 == 0 ? lightblue : blue;
			layer_draw_rect(layer, p, s, c1);
			p.x += dx;
			p.y += (int)dy;
		}
	}

	if(t2 - 1 <= time && time <= t2 + 1){
		p.y += (int)(v * (time - t2));
		for(i = 0;i < 16;i++){
			c1 = i % 2 == 0 ? blue : orange;
			layer_draw_rect(layer, p, s, c1);
			p.x += dx;
			p.y += (int)dy;
		}
	}

	if(t3 - 1 <= time && time <= t3 + 1){
		p.y += (int)(v * (time - t3));
		for(i = 0;i < 16;i++){
			c1 = i % 2 == 0 ? orange :darkblue;
			layer_draw_rect(layer, p, s, c1);
			p.x += dx;
			p.y += (int)dy;
		}
	}
	return layer;
}
