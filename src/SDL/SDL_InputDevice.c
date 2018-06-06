/*
 *  SDL_InputDevice.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "SDL_InputDevice.h"
#include <string.h>

static InputState* getInputState(this_p(InputDevice)) {
    SDL_InputDevice *device = (SDL_InputDevice *) this;
    return &device->currentState;
}

static void reset(this_p(InputDevice)) {
    memset(&((SDL_InputDevice*)this)->currentState, 0, sizeof(InputState));
}

static struct InputDevice_VTABLE _vtable = {
        getInputState, reset
};

void SDL_InputDevice_init(this_p(SDL_InputDevice)) {
    VT(this->base) = &_vtable;
}
