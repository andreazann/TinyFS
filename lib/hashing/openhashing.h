//
// Created by Marco Speziali on 01/07/2017.
//

#ifndef TINYFS_OPENHASHING_H
#define TINYFS_OPENHASHING_H

#define LINEAR_PROBING(hashingfn, key, index, size) \
    ((hashingfn(key) + (index)) % (size))

#define DOUBLE_HASHING_PROBING(hashingfn1, hashingfn2, key, index, size) \
    ((hashingfn1(key) + (index) * hashingfn2(key)) % (size))

#endif //TINYFS_OPENHASHING_H
