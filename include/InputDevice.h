/*
 *  InputDevice.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include "Object.h"
#include "Bool.h"
#include "CIntTypes.h"
#include "GameBoard.h"

typedef class InputState {
    bool enterButton;
	PillDirection direction1, direction2;
} InputState;

class InputDevice;

struct InputDevice_VTABLE {
    InputState* (*getInputState)(this_p(InputDevice));
    void (*reset)(this_p(InputDevice));
};

typedef class InputDevice {
    struct InputDevice_VTABLE *VTABLE;
} InputDevice;

#endif
