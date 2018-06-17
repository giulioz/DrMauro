/*
 *  Random.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
*/

#ifndef RANDOM_H
#define RANDOM_H

#include "CIntTypes.h"
#include "Object.h"

class Random;

struct Random_VTABLE {
    uint32_t (*randomBetween)(this_p(Random), uint32_t min, uint32_t max);
};

typedef class Random {
    struct Random_VTABLE *VTABLE;
    uint32_t n;
} Random;

void Random_init(this_p(Random), uint32_t seed);

#endif
