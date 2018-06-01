/*
 *  AudioManager.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "Object.h"
#include "Bool.h"
#include <soundio/soundio.h>

typedef class AudioManager {
    struct SoundIo *soundio;
    struct SoundIoDevice *device;
    struct SoundIoOutStream *outstream;
} AudioManager;

extern AudioManager AudioManager_Instance;

#endif
