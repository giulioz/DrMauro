/*
 *  Object.h
 *  Copyright © 2018 Giulio Zausa, Alessio Marotta
 *
 *  Macros for OO programming
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>

#define class struct
#define this_p(type) class type *this
#define new(type) malloc(sizeof(type));
//#define delete(ptr) free(ptr);
#define VTP(obj) obj->VTABLE
#define VT(obj) obj.VTABLE

/*
 * Example usage:
 *
 * typedef class Test Test;
 *
 * struct Test_VTABLE { void (*print)(this_p(Test)); };
 *
 * typedef class Test { struct Test_VTABLE *VTABLE; }
 *
 * Test test;
 * VT(test)->print();
 */

#endif
