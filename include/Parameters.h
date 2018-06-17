/*
 *  Parameters.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Application command line parameters
 */

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "Object.h"
#include "Bool.h"
#include "CIntTypes.h"
#include "optparse.h"

typedef enum {
    GameType_Menu,
    GameType_CustomParams,
    GameType_CustomBoard
} GameType;

class Parameters;

struct Parameters_VTABLE {
	void (*parseParameters)(this_p(Parameters), int argc, char **argv);
};

typedef class Parameters {
	struct Parameters_VTABLE *VTABLE;

    size_t difficulty;
    uint32_t speed;
    char* boardFile;
    GameType type;
} Parameters;

void Parameters_init(this_p(Parameters));

#endif
