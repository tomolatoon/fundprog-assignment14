#include <stdio.h>

#define W 640
#define H 480

int main(void) {
    FILE *fp = fopen("background3.ppm", "wb");
    if (!fp) return 1;

    int ground_y = H - 100;

    // P6 ヘッダ
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

                if (t < 0.5f) {
                    float u = t / 0.5f;
                    r = (unsigned char)(60  * (1 - u) + 255 * u);
                    g = (unsigned char)(40  * (1 - u) + 150 * u);
                    b = (unsigned char)(120 * (1 - u) + 80  * u);
                } else {
                    float u = (t - 0.5f) / 0.5f;
                    r = (unsigned char)(255 * (1 - u) + 255 * u);
                    g = (unsigned char)(150 * (1 - u) + 210 * u);
                    b = (unsigned char)(80  * (1 - u) + 120 * u);
                }
            }

            fwrite(&r, 1, 1, fp);
            fwrite(&g, 1, 1, fp);
            fwrite(&b, 1, 1, fp);
        }
    }

    fclose(fp);
    return 0;
}