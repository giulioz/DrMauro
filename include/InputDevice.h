/*
 *  InputDevice.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include "Object.h"
#include "Bool.h"
#include "IntTypes.h"

typedef class InputState {
    bool rotateLeftButton, rotateRightButton;
    bool enterButton;
    bool upButton, downButton, leftButton, rightButton;
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
