/*
 *  HeapVector.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  A C++ std::vector-like heap-allocated dynamic array
 */

#include "Vector.h"
#include "ExceptionManager.h"

#include <string.h>

static size_t _count(const Vector* vector) {
    return vector->count;
}

/* ------------------------------------------------------------------ */
/* Internals                                                          */

static size_t getDataIndex(const Vector* vector, const size_t index) {
    return index * vector->elementSize;
}

static void setUnchecked(Vector* vector, const size_t index, const void* element) {
    memcpy((char*)vector->data + getDataIndex(vector, index), element, vector->elementSize);
}

static void grow(Vector* vector, const size_t destElemSize) {
    if (!realloc(vector->data, destElemSize * vector->elementSize)) {
        ThrowError("Error rellocing array");
    } else {
        vector->allocatedCount = destElemSize;
    }
}

/* ------------------------------------------------------------------ */
/* Vector Operations                                                  */

static void _add(Vector* vector, const void* element) {
    if (vector->allocatedCount < vector->count)
        grow(vector, vector->count + 1);

    setUnchecked(vector, vector->count, element);
    vector->count++;
}

static void _set(Vector* vector, const size_t index, const void* element) {
    if (index < vector->count) {
        setUnchecked(vector, index, element);
    } else {
        ThrowError("Set array out of bounds"); /* cannot set outside, use add instead */
    }
}

static void* _get(const Vector* vector, const size_t index) {
    if (index < vector->count) {
        return (char*)vector->data + getDataIndex(vector, index);
    } else {
        ThrowError("Get array out of bounds");
        return NULL;
    }
}

static void _insert(Vector* vector, const size_t index, const void* element) {
    // TODO
    ThrowWarning("Unimplemented: insert on vector");
}

static void _delete(Vector* vector, const size_t index) {
    // TODO
    ThrowWarning("Unimplemented: delete vector element");
}

static void _clear(Vector* vector) {
    vector->count = 0;
}

static void _dispose(Vector* vector) {
    free(vector->data);
    vector->count = 0;
    vector->allocatedCount = 0;
}

static void _realloc(Vector* vector, const size_t destSize) {
    if (destSize < vector->count) ThrowError("Realloc destination is less than vector element count");
    else grow(vector, destSize);
}

static void _shrink(Vector *vector) {
    grow(vector, vector->count);
}

/* ------------------------------------------------------------------ */
/* VTABLE                                                             */

static struct Vector_VTABLE _vtable = {
        _count,
        _add,
        _set,
        _get,
        _insert,
        _delete,
        _clear,
        _dispose,
        _realloc,
        _shrink
};

/* ------------------------------------------------------------------ */
/* Vector creation                                                    */

void HeapVector_init(this_p(Vector), const size_t initialSize, const size_t elementSize) {
    this->elementSize = elementSize;
    this->allocatedCount = initialSize;
    this->count = 0;
    this->data = calloc(initialSize, elementSize);
    if (!this->data) ThrowError("Malloc failed!");
    VTP(this) = &_vtable;
}
