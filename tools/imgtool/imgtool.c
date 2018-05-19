//
// Created by Giulio Zausa on 15/05/18.
//

#define STB_IMAGE_IMPLEMENTATION
#include "imgtool.h"

void printUsage() {
    printf("imgtool: convert an image to header\n");
    printf("usage: imgtool source dest\n");
}

int main(int argc, char **argv) {
    FILE *inFile, *outFile;
    int x, y, n;
    unsigned char *data;

    if (argc != 3) {
        printUsage();
        return 1;
    }

    data = stbi_load(argv[1], &x, &y, &n, 3);

    outFile = fopen(argv[1], "w");
    if (!outFile) {
        printf("error writing: %s\n", argv[2]);
        return 1;
    }

    fclose(outFile);

    return 0;
}
