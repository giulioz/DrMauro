/*
 *  Vector.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  A C++ std::vector-like dynamic array
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include "Object.h"

struct Vector_VTABLE;
struct Vector2D_VTABLE;

typedef class Vector {
    struct Vector_VTABLE *VTABLE;

    void* data;
    size_t elementSize;     /* bytes per element */
    size_t count;           /* in elements */
    size_t allocatedCount;  /* free space (elements, not bytes) */
} Vector;

typedef class Vector2D {
    struct Vector2D_VTABLE *VTABLE;

    void* data;
    size_t elementSize;     /* bytes per element */
    size_t count;           /* in elements */
    size_t allocatedCount;  /* free space (elements, not bytes) */
    size_t width, height;
} Vector2D;

struct Vector_VTABLE {
    /* Returns element in the vector */
    size_t (*count)(const this_p(Vector));

    /* Adds an element to the end of the vector */
    void (*add)(this_p(Vector), const void* element);

    /* Adds an element empty to the end of the vector */
    void* (*addEmpty)(this_p(Vector));

    /* Sets an element of the vector given an index and a pointer to an element (doesn't change size) */
    void (*set)(this_p(Vector), const size_t index, const void* element);

    /* Returns the pointer to an element */
    void* (*get)(const this_p(Vector), const size_t index);

    /* Insert an element on the vector (changes size) */
    void (*insert)(this_p(Vector), const size_t index, const void* element);

    /* Removes an element (changes size) */
    void (*delete)(this_p(Vector), const size_t index);

    /* Sets the count to 0 */
    void (*clear)(this_p(Vector));

    /* Frees the entire vector (cannot use after) */
    void (*dispose)(this_p(Vector));

    /* Changes vector allocated size */
    void (*realloc)(this_p(Vector), const size_t destSize);

    /* Changes vector allocated size to minimum possible*/
    void (*shrink)(this_p(Vector));
};

struct Vector2D_VTABLE {
    struct Vector_VTABLE base;

    void (*set2D)(this_p(Vector2D), const size_t row, const size_t col, const void* element);
    void* (*get2D)(const this_p(Vector2D), const size_t row, const size_t col);
};

void HeapVector_init(this_p(Vector), size_t initialSize, size_t elementSize);
void StackVector_init(this_p(Vector), size_t allocatedSize, size_t elementSize, void* stackArray);
void StackVector2D_init(this_p(Vector2D), size_t width, size_t height, size_t elementSize, void* stackArray);

extern struct Vector_VTABLE StaticVector_VTABLE_DEFAULT;

#define Vector_foreach(vec,i) for ((i) = 0; (i) < (vec).count; (i)++)

#define StackVector_Empty(name, elementSize, allocSize) \
Vector name = { \
    &StaticVector_VTABLE_DEFAULT, \
    NULL, \
    elementSize, \
    0, \
    allocSize \
}

#define StackVector_Static(name, elementSize, count, allocSize, array) \
Vector name = { \
    &StaticVector_VTABLE_DEFAULT, \
    array, \
    elementSize, \
    count, \
    allocSize \
}

#endif
