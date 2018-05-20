/*
 *  Color.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Colors are defined as RGBA (8 bit per channel) packed into an int
 */

#include "Color.h"

Color Color_FromInt(uint32_t code) {
    /* HACK: watch out for alignment */
    Color c;
    uint32_t *m = (uint32_t *) &c;
    *m = code;
    return c;
}
