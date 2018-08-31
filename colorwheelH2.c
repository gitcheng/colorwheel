//
// Calculate RGB from hue angle and chroma on a cartesian coordinates
// See https://en.wikipedia.org/wiki/HSL_and_HSV#Hue_and_chroma for the
// transformation of H2 and C2 (hexagon)
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

const double sqrt3 = 1.7320508075688772935;

void getRGB(int x, int y, int *R, int *G, int *B)
{
    if (x < 0 || x > 1023 || y < 0 || y > 1023) {
        printf("Error, x or y out of range\n");
        abort();
    }
    double xx = (x - 511.5) / 511.5;
    double yy = (y - 511.5) / 511.5;
    double rr = sqrt(xx*xx + yy*yy);
    if (rr > 1) {
        xx = xx / rr;
        yy = yy / rr;
    }

    double dR = 1;
    double dB = dR - xx - yy/sqrt3;
    double dG = dR - xx + yy/sqrt3;

    if (dG > dR && dG > dB) {  // G is the max
        dG = 1;
        dR = dG + xx - yy/sqrt3;
        dB = dG - 2 * yy/sqrt3;
    } else if (dB > dR && dB > dG) {  // B is the max
        dB = 1;
        dR = dB + xx + yy/sqrt3;
        dG = dB + 2 * yy/sqrt3;
    }

    if (dR < 0) dR = 0;
    if (dG < 0) dG = 0;
    if (dB < 0) dB = 0;

    *R = (int)(dR * 255);
    *G = (int)(dG * 255);
    *B = (int)(dB * 255);
}

int main()
{
    int R, G, B;
    FILE *fr, *fg, *fb;
    fr = fopen("h2c2_R.txt", "w");
    fg = fopen("h2c2_G.txt", "w");
    fb = fopen("h2c2_B.txt", "w");
    
    for (int x = 0; x < 1024; x++) {
        for (int y = 0; y < 1024; y++) {    
            getRGB(x, y, &R, &G, &B);
            fprintf(fr, "%d  ", R);
            fprintf(fg, "%d  ", G);
            fprintf(fb, "%d  ", B);
        }
        fprintf(fr, "\n");
        fprintf(fg, "\n");
        fprintf(fb, "\n");
    }
    fclose(fr);
    fclose(fg);
    fclose(fb);

    return 0;
}
