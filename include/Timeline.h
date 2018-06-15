/*
*  Timeline.h
*  Copyright � 2018 Giulio Zausa, Alessio Marotta
*/

#ifndef TIMELINE_H
#define TIMELINE_H

#include "Object.h"
#include "Bool.h"
#include "CIntTypes.h"
#include "Vector.h"

typedef struct TimelineEvent {
	void (*invoke)(void *this);
	uint32_t time;
	void *context;
} TimelineEvent;

class Timeline;

struct Timeline_VTABLE {
	void (*update)(class Timeline *this, uint32_t time);
	void (*addEvent)(class Timeline *this, void(*invoke)(void *this), uint32_t time, void *context);
	void (*dispose)(class Timeline *this);
};

typedef class Timeline {
	struct Timeline_VTABLE *VTABLE;

	Vector events;
} Timeline;

void Timeline_init(Timeline *this, size_t prealloc);

#endif
