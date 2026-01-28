#include <stdio.h>

#define W 640
#define H 480

int main(void) {
    FILE *fp = fopen("ground3.ppm", "wb");
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

                int rr = 40;
                int gg = 70;
                int bb = 40;

                int dy = y - ground_y;

                if (dy < grad_height) {
                    float t = 1.0f - (float)dy / (float)grad_height;

                    rr += (int)(t * 20);
                    gg += (int)(t * 30);
                    bb += (int)(t * 40);

                    if (rr > 255) rr = 255;
                    if (gg > 255) gg = 255;
                    if (bb > 255) bb = 255;
                }

                r = (unsigned char)rr;
                g = (unsigned char)gg;
                b = (unsigned char)bb;

            } else if (y == ground_y) {

                r = 10;
                g = 10;
                b = 20;

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