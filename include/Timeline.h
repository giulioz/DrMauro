/*
*  Timeline.h
*  Copyright � 2018 Giulio Zausa, Alessio Marotta
*/

#ifndef TIMELINE_H
#define TIMELINE_H

#include "Object.h"
#include "Bool.h"
#include "IntTypes.h"
#include "Vector.h"

typedef struct TimelineEvent {
	void (*invoke)(void *this);
	uint32_t time;
	void *context;
} TimelineEvent;

class Timeline;

struct Timeline_VTABLE {
	void (*update)(Timeline *this, uint32_t time);
	void (*addEvent)(Timeline *this, void(*invoke)(void *this), uint32_t time, void *context);
	void (*dispose)(Timeline *this);
};

typedef class Timeline {
	struct Timeline_VTABLE *VTABLE;

	Vector events;
} Timeline;

void Timeline_init(Timeline *this, size_t prealloc);

#endif
