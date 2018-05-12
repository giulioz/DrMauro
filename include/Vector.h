/*
 *  Vector.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  A C++ std::vector-like heap dynamic array
 */

#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct {
    void* Data;
    size_t ElementSize;     /* bytes per element */
    size_t Count;           /* in elements */
    size_t AllocatedCount;  /* free space (elements, not bytes) */
} Vector;

Vector Vector_Create(size_t initialSize, size_t elementSize);
size_t Vector_Count(const Vector* vector);
void Vector_Add(Vector* vector, const void* element);
void Vector_Set(Vector* vector, size_t index, const void* element);
void *Vector_Get(const Vector* vector, size_t index);
void Vector_Insert(Vector* vector, size_t index, const void* element);
void Vector_Delete(Vector* vector, size_t index);
void Vector_Clear(Vector* vector);
void Vector_Free(Vector* vector);

#define Vector_Foreach(vec,i) for ((i) = 0; (i) < (vec).Count; (i)++)

#endif
