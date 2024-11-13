//
// Created by Marco Speziali on 08/07/2017.
//

#include "functions.h"
#include "../../../lib/macros.h"

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

int doublehashing(void *key, int index, int maxkeys) {
    int clh = closedhashing(key, maxkeys);
    int clhg = closedhashingindex(key, maxkeys);
    return (clh + index * clhg) % maxkeys;
}
