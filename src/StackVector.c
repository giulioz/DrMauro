/*
 *  StackVector.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  A C++ std::vector-like stack-allocated dynamic array
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
    ThrowError("Attempt to grow a stack-allocated vector: out of bounds");
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

static void _set2D(Vector2D* vector, const size_t row, const size_t col, const void* element) {
    size_t index = col + row * vector->width;
    if (index < vector->count) {
        setUnchecked((Vector *) vector, index, element);
    } else {
        ThrowError("Set array out of bounds"); /* cannot set outside, use add instead */
    }
}

static void* _get2D(const Vector2D* vector, const size_t row, const size_t col) {
    size_t index = col + row * vector->width;
    if (index < vector->count) {
        return (char*)vector->data + getDataIndex((const Vector *) vector, index);
    } else {
        ThrowError("Get array out of bounds");
        return NULL;
    }
}

static void* _addEmpty(Vector* vector) {
    if (vector->allocatedCount < vector->count)
        grow(vector, vector->count + 1);

    vector->count++;
    return _get(vector, vector->count - 1);
}

static void _insert(Vector* vector, const size_t index, const void* element) {
    // TODO
    ThrowWarning("Unimplemented: insert on vector");
}

static void _delete(Vector* vector, const size_t index) {
	size_t i;
	if (index == vector->count - 1) {
		vector->count--;
	}
	else {
		for (i = vector->count - 1; i >= index; i--) {
			_set(vector, i - 1, _get(vector, i));
		}
	}
}

static void _clear(Vector* vector) {
    vector->count = 0;
}

static void _dispose(Vector* vector) {
    vector->count = 0;
}

static void _realloc(Vector* vector, const size_t destSize) {
    if (destSize < vector->count) ThrowError("Realloc destination is less than vector element count");
    else grow(vector, destSize);
}

static void _shrink(Vector *vector) {
    ThrowWarning("Attempt to shrink a stack-allocated vector");
}

/* ------------------------------------------------------------------ */
/* VTABLE                                                             */

struct Vector_VTABLE StaticVector_VTABLE_DEFAULT = {
        _count,
        _add,
        _addEmpty,
        _set,
        _get,
        _insert,
        _delete,
        _clear,
        _dispose,
        _realloc,
        _shrink
};

static struct Vector2D_VTABLE StaticVector2D_VTABLE_DEFAULT = {
        {
                _count,
                _add,
                _addEmpty,
                _set,
                _get,
                _insert,
                _delete,
                _clear,
                _dispose,
                _realloc,
                _shrink
        },
        _set2D,
        _get2D
};

/* ------------------------------------------------------------------ */
/* Vector creation                                                    */

void StackVector_init(this_p(Vector), const size_t allocatedSize, const size_t elementSize, void* stackArray) {
    this->elementSize = elementSize;
    this->allocatedCount = allocatedSize;
    this->count = 0;
    this->data = stackArray;
    if (!this->data) ThrowError("Void pointer in stack vector initialization");
    VTP(this) = &StaticVector_VTABLE_DEFAULT;
}

void StackVector2D_init(this_p(Vector2D), size_t width, size_t height, size_t elementSize, void* stackArray) {
    StackVector_init((struct Vector *) this, width * height, elementSize, stackArray);
    VTP(this) = &StaticVector2D_VTABLE_DEFAULT;
    this->count = width*height;
    this->width = width;
    this->height = height;
}
