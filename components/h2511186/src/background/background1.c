#include <stdio.h>

#define W 640
#define H 480

int main(void) {
    FILE *fp = fopen("background1.ppm", "wb");
    if (!fp) return 1;

    int ground_y = H - 100;

    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", W, H);
    fprintf(fp, "255\n");

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {

            unsigned char r, g, b;

            if (y > ground_y) {
                r = g = b = 255;
            } else if (y == ground_y) {
                r = g = b = 0;
            } else {
                float t = (float)y / (float)ground_y;

                r = (unsigned char)(40  * (1 - t) + 180 * t);
                g = (unsigned char)(60  * (1 - t) + 220 * t);
                b = (unsigned char)(100 * (1 - t) + 240 * t);
            }

            fwrite(&r, 1, 1, fp);
            fwrite(&g, 1, 1, fp);
            fwrite(&b, 1, 1, fp);
        }
    }

    fclose(fp);
    return 0;
}
