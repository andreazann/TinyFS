//
// Created by Marco Speziali on 01/07/2017.
//

#ifndef TINYFS_CLOSEDHASHING_H
#define TINYFS_CLOSEDHASHING_H

#include <stdint.h>
#include "../macros.h"

unsigned long computeHashing(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

int closedhashing(void *key, int size) {
    double hash = DOUBLE(computeHashing(key));
    double ma = hash *  SILVER_RATIO;
    double fract = FRACTIONAL_PART(ma);

    return (int)(DOUBLE(size) * fract);
}

int closedhashingindex(void *key, int size) {
    unsigned long hash = computeHashing(key);

    return (int) (((15 * hash + 150) % 1031) % size);
}

#endif //TINYFS_CLOSEDHASHING_H
