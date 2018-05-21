//
// Created by Giulio Zausa on 15/05/18.
//

#define STB_IMAGE_IMPLEMENTATION
#include "imgtool.h"

void printUsage() {
    printf("imgtool: convert an image to source file\n");
    printf("usage: imgtool source dest\n");
}

int main(int argc, char **argv) {
    FILE *outFile = NULL;
    int x, y, n, i;
    unsigned char *data;

    if (argc != 3) {
        printUsage();
        return 1;
    }

    data = stbi_load(argv[1], &x, &y, &n, 4);

    outFile = fopen(argv[2], "w");
    if (!outFile) {
        printf("error writing: %s\n", argv[2]);
        return 1;
    }

    fprintf(outFile, "/*\n *  %s\n *  Copyright © 2018 Giulio Zausa, Alessio Marotta\n */\n", argv[1]);
    fprintf(outFile, "#ifndef %s_H\n#define %s_H\n\n", argv[1], argv[1]);
    fprintf(outFile, "char file_%s[] = { ", argv[1]);

    for (i = 0; i < x*y*n; ++i) {
        fprintf(outFile, "0x%x, ", data[i]);
    }

    fprintf(outFile, " };\n\n#endif\n");

    fflush(outFile);
    fclose(outFile);

    return 0;
}
