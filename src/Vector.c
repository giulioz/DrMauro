/*
 *  Vector.c
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  A C++ std::vector-like heap dynamic array
 */

#include "Vector.h"
#include "ExceptionManager.h"

#include <string.h>

/* ------------------------------------------------------------------ */
/* Vector creation                                                     */

Vector Vector_Create(const size_t initialSize, const size_t elementSize) {
    Vector vector;
    vector.ElementSize = elementSize;
    vector.AllocatedCount = initialSize;
    vector.Count = 0;
    vector.Data = calloc(initialSize, elementSize);
    if (!vector.Data) ThrowError("Malloc failed!");
    return vector;
}

size_t Vector_Count(const Vector* vector) {
    return vector->Count;
}

/* ------------------------------------------------------------------ */
/* Internals                                                          */

static size_t getDataIndex(const Vector* vector, const int index) {
    return index * vector->ElementSize;
}

static void setUnchecked(Vector* vector, const size_t index, const void* element) {
    memcpy((char*)vector->Data + getDataIndex(vector, index), element, vector->ElementSize);
}

static void grow(Vector* vector, const size_t destElemSize) {
    if (!realloc(vector->Data, destElemSize * vector->ElementSize)) {
        ThrowError("Error growing array");
    } else {
        vector->AllocatedCount = destElemSize;
    }
}

/* ------------------------------------------------------------------ */
/* Vector Operations                                                  */

void Vector_Add(Vector* vector, const void* element) {
    if (vector->AllocatedCount < vector->Count)
        grow(vector, vector->Count + 1);

    setUnchecked(vector, vector->Count, element);
    vector->Count++;
}

void Vector_Set(Vector* vector, const size_t index, const void* element) {
    if (index < vector->Count) {
        setUnchecked(vector, index, element);
    } else {
        ThrowError("Set array out of bounds"); /* cannot set outside */
    }
}

void *Vector_Get(const Vector* vector, const size_t index) {
    if (index < vector->Count) {
        return (char*)vector->Data + getDataIndex(vector, index);
    } else {
        ThrowError("Get array out of bounds");
        return NULL;
    }
}

void Vector_Insert(Vector* vector, const size_t index, const void* element) {
    // TODO
    ThrowWarning("Unimplemented: insert on vector");
}

void Vector_Delete(Vector* vector, const size_t index) {
    // TODO
    ThrowWarning("Unimplemented: delete vector element");
}

void Vector_Clear(Vector* vector) {
    vector->Count = 0;
}

void Vector_Free(Vector* vector) {
    free(vector->Data);
    vector->Count = 0;
    vector->AllocatedCount = 0;
}
