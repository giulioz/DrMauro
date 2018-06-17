/*
 *  Random.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Generatore di numeri casuali totalmente prevedibile
 *  ma con buone proprietà statistiche, ottimo per generare due campi
*/

#include "Random.h"

#include "math.h"

/* nsa will like this */
static uint32_t randomBetween(this_p(Random), uint32_t min, uint32_t max) {
    this->n *= 1103515245;
    this->n += 124345;
    //double out1 = fabs(cos(this->n)) * (max - min);
    return (uint32_t)(((double)this->n / 0xFFFFFFFF) * (max - min) + min);
}

static struct Random_VTABLE _vtable = {
        randomBetween
};

void Random_init(this_p(Random), uint32_t seed) {
    VTP(this) = &_vtable;
    this->n = seed;
}
