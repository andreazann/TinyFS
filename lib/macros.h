//
// Created by Marco Speziali on 01/07/2017.
//

#ifndef TINYFS_MACROS_H
#define TINYFS_MACROS_H

#define NEW_ARRAY(type, size) ((type *)malloc(sizeof(type) * (size)))

#define NEW(type) NEW_ARRAY(type, 1)

#define RAND_INIT() srand(time(NULL))

#define RAND_RANGE(min, max) ((min) + rand() % ((max) + 1 - (min)))

#ifndef __bool_true_false_are_defined
#define bool _Bool
#define true 1
#define false 0
#endif

#define DOUBLE(x) ((double)(x))

#define SILVER_RATIO (0.618033988749894) // (√5 - 1)/2 aka Knuth constant or Golden ratio conjugate

#define INTEGER_PART(x) ((int)x)

#define FRACTIONAL_PART(x) ((x) - INTEGER_PART(x))

#endif //TINYFS_MACROS_H
