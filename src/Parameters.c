/*
 *  Parameters.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Application command line parameters
 */

#define OPTPARSE_IMPLEMENTATION
#include "Parameters.h"
#include <stdio.h>
#include "ExceptionManager.h"

void usage() {
    printf( "DR.MAURO - dr.Mario Clone                        \n"
                    "Usage: drmauro [-f FILE | -d DIFFICULTY] [-s SPEED] [-h] \n"
                    "                                                         \n"
                    "OPTIONS:                                                 \n"
                    "  -f FILE         Load board from FILE                   \n"
                    "  -d DIFFICULTY   Generate random board (default 5)      \n"
                    "  -s SPEED        Game speed                             \n"
                    "  -h              Show this help message                 \n"
    );
}

static void parseParameters(this_p(Parameters)) {
    struct optparse options;
    int option;
    double s = 0.3;
    int diff = 0;

    bool dSet = false;
    bool sSet = false;

    optparse_init(&options, this->argv);
    while ((option = optparse(&options, "f:d:s:h")) != -1) {
        switch (option) {
            case 'f':
                this->boardFile = options.optarg;
                break;
            case 'd':
                diff = options.optarg ? atoi(options.optarg) : 0;
                dSet = true;
                break;
            case 's':
                s = options.optarg ? atof(options.optarg) : 0.0;
                sSet = true;
                break;
            case 'h':
                usage();
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }

    this->speed = (uint32_t) (s * 1000.0);
    this->difficulty = (size_t) diff;

    /* Check game type */
    if (this->boardFile) this->type = GameType_CustomBoard;
    else if (dSet || sSet) this->type = GameType_CustomParams;
}

void Parameters_init(this_p(Parameters), int argc, char **argv) {
    this->argc = argc;
    this->argv = argv;
    this->difficulty = 0;
    this->speed = 0;
    this->boardFile = NULL;
    this->type = GameType_Menu;

    parseParameters(this);
}
