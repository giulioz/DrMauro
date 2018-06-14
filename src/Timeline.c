/*
*  Timeline.c
*  Copyright � 2018 Giulio Zausa, Alessio Marotta
*/

#include "Timeline.h"

static void update(Timeline *this, uint32_t time) {
	size_t i;
	Vector_foreach(this->events, i) {
		TimelineEvent *event = VT(this->events)->get(&this->events, i);
		if (event->time < time) {
			event->invoke(event->context);
			VT(this->events)->delete(&this->events, i);
			i--;
		}
	}
}

static void addEvent(Timeline *this, void(*invoke)(void *this), uint32_t time, void *context) {
	TimelineEvent *event = VT(this->events)->addEmpty(&this->events);
	event->context = context;
	event->time = time;
	event->invoke = invoke;
}

static void dispose(Timeline *this) {
	VT(this->events)->dispose(&this->events);
}

static struct Timeline_VTABLE _vtable = {
	update, addEvent, dispose
};

void Timeline_init(Timeline *this, size_t prealloc) {
	VTP(this) = &_vtable;
	HeapVector_init(&this->events, prealloc, sizeof(TimelineEvent));
}
