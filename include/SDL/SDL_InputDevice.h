/*
 *  SDL_InputDevice.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef SDLINPUTDEVICE_H
#define SDLINPUTDEVICE_H

#include "Object.h"
#include "Bool.h"
#include "InputDevice.h"

typedef class SDL_InputDevice {
    InputDevice base;
    InputState currentState;
} SDL_InputDevice;

void SDL_InputDevice_init(this_p(SDL_InputDevice));

#endif
