/*
 *  AudioTestState.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 */

#include "AudioTestState.h"

#include "ExceptionManager.h"

static const float PI = 3.1415926535f;
static float seconds_offset = 0.0f;
static void write_callback(struct SoundIoOutStream *outstream,
                           int frame_count_min, int frame_count_max)
{
    const struct SoundIoChannelLayout *layout = &outstream->layout;
    float float_sample_rate = outstream->sample_rate;
    float seconds_per_frame = 1.0f / float_sample_rate;
    struct SoundIoChannelArea *areas;
    int frames_left = frame_count_max;
    int err;

    while (frames_left > 0) {
        int frame_count = frames_left;

        if ((err = soundio_outstream_begin_write(outstream, &areas, &frame_count))) {
            ThrowError(soundio_strerror(err));
        }

        if (!frame_count)
            break;

        float pitch = 440.0f;
        float radians_per_second = pitch * 2.0f * PI;
	int frame, channel;
        for (frame = 0; frame < frame_count; frame += 1) {
            float sample = sinf((seconds_offset + frame * seconds_per_frame) * radians_per_second);
            for (channel = 0; channel < layout->channel_count; channel += 1) {
                float *ptr = (float*)(areas[channel].ptr + areas[channel].step * frame);
                *ptr = sample;
            }
        }
        seconds_offset = fmodf(seconds_offset +
                               seconds_per_frame * frame_count, 1.0f);

        if ((err = soundio_outstream_end_write(outstream))) {
            ThrowError(soundio_strerror(err));
        }

        frames_left -= frame_count;
    }
}

static void draw(this_p(GameState)) {

}

static bool update(this_p(GameState)) {
    AudioTestState *state = (AudioTestState *) this;
    soundio_wait_events(state->soundio);
    return true;
}

static void load(this_p(GameState)) {
    AudioTestState *state = (AudioTestState *) this;

    int err;
    state->soundio = soundio_create();
    if (!state->soundio) {
        ThrowError("out of memory");
    }

    if ((err = soundio_connect(state->soundio))) {
        ThrowError(soundio_strerror(err));
    }

    soundio_flush_events(state->soundio);

    int default_out_device_index = soundio_default_output_device_index(state->soundio);
    if (default_out_device_index < 0) {
        ThrowError("no output device found");
    }

    state->device = soundio_get_output_device(state->soundio, default_out_device_index);
    if (!state->device) {
        ThrowError("out of memory");
    }

    state->outstream = soundio_outstream_create(state->device);
    state->outstream->format = SoundIoFormatFloat32NE;
    state->outstream->write_callback = write_callback;

    if ((err = soundio_outstream_open(state->outstream))) {
        ThrowError(soundio_strerror(err));
    }

    if (state->outstream->layout_error)
        ThrowError(soundio_strerror(state->outstream->layout_error));

    if ((err = soundio_outstream_start(state->outstream))) {
        ThrowError(soundio_strerror(err));
    }
}

static void unload(this_p(GameState)) {
    AudioTestState *state = (AudioTestState *) this;
    soundio_outstream_destroy(state->outstream);
    soundio_device_unref(state->device);
    soundio_destroy(state->soundio);
}

static struct GameState_VTABLE _vtable = {
        update, draw
};

void AudioTestState_init(this_p(AudioTestState), Engine *engine) {
    this->base.engine = engine;
    VT(this->base) = &_vtable;
}
