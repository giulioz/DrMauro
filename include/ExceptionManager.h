/*
 *  ExceptionManager.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Logging functions and exception handler
 */

#ifndef EXCEPTIONMANAGER_H
#define EXCEPTIONMANAGER_H

/* ugly hack for easy error logging, max 50 chars */
#define ErrorConcat(a,b) \
    strcat(a "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", b)

void ThrowWarning(const char* warning);
void ThrowError(const char* error);

#endif
