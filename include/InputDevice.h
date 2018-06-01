/*
 *  InputDevice.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef INPUTDEVICE_H
#define INPUTDEVICE_H

#include "Object.h"
#include "Bool.h"
#include "IntTypes.h"

class InputDevice;

struct InputDevice_VTABLE {

};

typedef class InputDevice {
    struct InputDevice_VTABLE *VTABLE;
} InputDevice;

void InputDevice_init(this_p(InputDevice));

#endif
