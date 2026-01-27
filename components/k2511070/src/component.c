/// @file component.c
/// @brief k2511070 開発者のコンポーネント実装（ダミー）

#include "k2511070/component.h"

void k2511070_component_init(void)
{
	// TODO: 初期化処理
}


//一個の八分音符っを描画する関数
HLayer one_ejghth_note(RGBA c, HLayer layer, point p){
	RGBA c1;
	Point p1 = p;
	int dx = 0;
	int dy = 0;
	RGBA black = {{0.0,0.0,0.0},1.0};
	RGBA white = {{1.0,1.0,1.0},1.0};

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
		}else if{note[i] == 1}{
			c1 = black;
		}else{
			c1 = c;
		}
		for(int j = 0;j < 2;j++){
			for(int k = 0;k < 2;k++){
				dx = 2 * (i % 10) + k;
				dy = 2 * (i / 12) + j;
				p->x += dx;
				p->y += dy;
				layer_set_pixel(HLayer layer, Point p, RGBA c1);
			}
		}
	}
}

HLayer two_eighth_note(RGBA c, HLayer layer, point p){
    RGBA c1;
	int dx = 0;
	int dy = 0;
	RGBA black = {{0.0,0.0,0.0},1.0};
	RGBA white = {{1.0,1.0,1.0},1.0};

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
    int i;
    for(i = 0;i < 132;i++){
        if(note[i] == 3){
			//do nothing
		}else if(note[i] == 0){
			c1 = white;
		}else if{note[i] == 1}{
			c1 = black;
		}else{
			c1 = c;
		}
		for(int j = 0;j < 2;j++){
			for(int k = 0;k < 2;k++){
				dx = 2 * (i % 10) + k;
				dy = 2 * (i / 12) + j;
				p->x += dx;
				p->y += dy;
				layer_set_pixel(HLayer layer, Point p, RGBA c1);
			}
		}
	}

    img_write();
}

Hlayer sample_kanaizuka(void){
	RGBA c = {{1.0,0.0,0.0},1.0};
	Point p = {100,100};
	Hlayer layer = layer_create(SIZE(200,200));
	Hlayer note = one_eighth_note(c,layer,p);
	layer_composite(layer, note, POINT(0,0), NULL);
	layer_destroy(note);
	return layer;
}

void k2511070_component_draw(HLayer layer)
{
	// TODO: 描画処理
	(void)layer;
}
