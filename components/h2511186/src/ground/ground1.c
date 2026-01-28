#include <stdio.h>

#define W 640
#define H 480

int main(void) {
    FILE *fp = fopen("ground1.ppm", "wb");
    if (!fp) return 1;

    int ground_y = H - 100;
    int grad_height = 40;

    // P6 ヘッダ
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", W, H);
    fprintf(fp, "255\n");

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {

            unsigned char r, g, b;

            if (y > ground_y) {

                int rr = 140;
                int gg = 190;
                int bb = 120;

                int dy = y - ground_y;

                if (dy < grad_height) {
                    float t = 1.0f - (float)dy / (float)grad_height;

                    rr -= (int)(t * 10);
                    gg -= (int)(t * 20);
                    bb -= (int)(t * 5);

                    if (rr < 0) rr = 0;
                    if (gg < 0) gg = 0;
                    if (bb < 0) bb = 0;
                }

                r = (unsigned char)rr;
                g = (unsigned char)gg;
                b = (unsigned char)bb;

            } else {
                r = b = 255;
                g = 0;
            }

            fwrite(&r, 1, 1, fp);
            fwrite(&g, 1, 1, fp);
            fwrite(&b, 1, 1, fp);
        }
    }

    fclose(fp);
    return 0;
}