/*
 *  AudioTestState.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#ifndef AUDIOTESTSTATE_H
#define AUDIOTESTSTATE_H

#include "Object.h"
#include "GameState.h"
#include "Engine.h"
#include "Assets.h"
#include "Bool.h"
#include <soundio/soundio.h>
#include <math.h>

typedef class AudioTestState {
    GameState base;

    struct SoundIo *soundio;
    struct SoundIoDevice *device;
    struct SoundIoOutStream *outstream;
} AudioTestState;

void AudioTestState_init(this_p(AudioTestState), Engine *engine);

#endif
