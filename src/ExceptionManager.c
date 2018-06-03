/*
 *  ExceptionManager.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Logging functions and exception handler
 *  (emoji lol)
 */

#include "ExceptionManager.h"

#include <stdlib.h>
#include <stdio.h>
#include "debugbreak.h"

void ThrowWarning(const char* warning) {
    printf("⚠️ WARNING: %s\n", warning);
}

void ThrowError(const char* error) {
    printf("🚫 ERROR: %s\n", error);
    debug_break();
    exit(1); /* TODO: a better exception handling method */
}
