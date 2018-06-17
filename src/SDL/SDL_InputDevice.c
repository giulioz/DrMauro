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
	SDL_InputDevice *device = (SDL_InputDevice *)this;
	device->currentState.direction1 = PillDirection_Nothing;
	device->currentState.direction2 = PillDirection_Nothing;
	device->currentState.enterButton = false;
}

static struct InputDevice_VTABLE _vtable = {
        getInputState, reset
};

void SDL_InputDevice_init(this_p(SDL_InputDevice)) {
    VT(this->base) = &_vtable;
}
