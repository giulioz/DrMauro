/*
 * Created by Giulio Zausa on 15/05/18.
 */

#define STB_IMAGE_IMPLEMENTATION
#include "imgtool.h"
#include "../../include/graphics/Color.h"

static int colorsSize = 15;
static unsigned char colors[15][4] = {
        {0, 0, 0, 0},
        {255, 255, 255, 255},
        {164, 0, 59, 0},
        {123, 30, 183, 255},
        {255, 144, 146, 255},
        {255, 210, 211, 255},
        {255, 176, 100, 255},
        {255, 223, 192, 255},
        {222, 205, 72, 255},
        {0, 190, 188, 255},
        {0, 109, 107, 255},
        {165, 216, 247, 255},
        {102, 102, 102, 255},
        {0, 0, 0, 255},
        {164, 0, 59, 255},
};
static Color *palette = (Color *) &colors;

void swapColor(Color* color) {
    unsigned char temp = color->r;
    color->r = color->b;
    color->b = temp;
}

unsigned char findPaletteEntry(Color value) {
    unsigned char i;
    for (i = 0; i < colorsSize; i++) {
        if (value.r == palette[i].r
         && value.g == palette[i].g
         && value.b == palette[i].b
         && value.a == palette[i].a) {
            return i;
        }
        if (value.a == 0) {
            return 0;
        }
    }
    printf("color not found: %d %d %d %d\n", value.r, value.g, value.b, value.a);
}

void printUsage() {
    printf("imgtool: convert an image to source file\n");
    printf("usage: imgtool source\n");
}

int main(int argc, char **argv) {
    int x, y, n, i;
    Color *data;

    if (argc != 2) {
        printUsage();
        return 1;
    }

    data = (Color *) stbi_load(argv[1], &x, &y, &n, 4);

    for (i = 0; i < x*y; i++) {
        swapColor(data + i);
        printf("0x%x, ", findPaletteEntry(data[i]));
    }

    return 0;
}
