//
// Calculate RGB from hue angle and chroma on a cartesian coordinates
// See https://en.wikipedia.org/wiki/HSL_and_HSV#Hue_and_chroma for the
// transformation.
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

const double pi = 3.14159265358979323846;

void getRGB(int x, int y, int *R, int *G, int *B)
{
    if (x < 0 || x > 1023 || y < 0 || y > 1023) {
        printf("Error, x or y out of range\n");
        abort();
    }
    int xx = x - 512;
    int yy = y - 512;
    double rr = sqrt(xx*xx + yy*yy) / 512.0;
    if (rr > 1) rr = 1;

    double phi = 0;
    if (xx != 0) 
        phi = atan2(-yy, -xx) + pi;   // range [0, 2pi)
    double hprime = 3 * phi / pi;
    double fR, fG, fB;
    fR = fG = fB = 0;
    if (hprime >= 0 && hprime < 1) {
        fR = 1.0;
        fB = 1 - rr;
        fG = fB + rr * hprime;
    } else if (hprime >= 1 && hprime < 2) {
        fG = 1.0;
        fB = 1 - rr;
        fR = fB - rr * (hprime - 2);
    } else if (hprime >= 2 && hprime < 3) {
        fG = 1.0;
        fR = 1 - rr;
        fB = fR + rr * (hprime - 2);
    } else if (hprime >= 3 && hprime < 4) {
        fB = 1.0;
        fR = 1 - rr;
        fG = fR - rr * (hprime - 4);
    } else if (hprime >= 4 && hprime < 5) {
        fB = 1.0;
        fG = 1 - rr;
        fR = fG + rr * (hprime - 4);
    } else if (hprime >= 5 && hprime <= 6) {
        fR = 1.0;
        fG = 1 - rr;
        fB = fG - rr * (hprime - 6);
    } else {
        // something is wrong 
        printf("x = %d,  y = %d,  phi = %f,  hprime = %f", x, y, phi, hprime);
        assert(0);
    }
    *R = (int)(fR * 255);
    *G = (int)(fG * 255);
    *B = (int)(fB * 255);
}

int main()
{
    int R, G, B;
    FILE *fr, *fg, *fb;
    fr = fopen("hc_R.txt", "w");
    fg = fopen("hc_G.txt", "w");
    fb = fopen("hc_B.txt", "w");
    
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
