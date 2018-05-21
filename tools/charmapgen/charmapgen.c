//
// Created by Giulio Zausa on 21/05/18.
//

#include "stdio.h"

int main(int argc, char **argv) {
    unsigned char i;
    for (i = 0; i < 255; i++) {
        if (i >= 'a' && i <= 'z')
            printf("%d, ", i - 'a');
        else if (i >= 'A' && i <= 'Z')
            printf("%d, ", i - 'A');
        else if (i >= '0' && i <= '9')
            printf("%d, ", i - '0' + 25);
        else
            printf("0, ");
    }

    return 0;
}
