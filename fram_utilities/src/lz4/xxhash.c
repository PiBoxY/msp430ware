/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "msp430.h"

#include "lz4.h"
#include "xxhash.h"

#define XXHASH_PRIME1           2654435761U
#define XXHASH_PRIME2           2246822519U
#define XXHASH_PRIME3           3266489917U
#define XXHASH_PRIME4           668265263U
#define XXHASH_PRIME5           374761393U

#define MIN_VECTOR_LENGTH       16

static inline uint32_t rotl32(uint32_t input, uint16_t shift)
{
    return ((input << shift) | (input >> (32 - shift)));
}

static inline uint32_t read32(const uint8_t* ptr, bool aligned)
{
    /* Even address */
    if (aligned) {
        return *(uint32_t *)ptr;
    }
    /* Odd address */
    else {
        uint32_t value = ptr[0];
        value += (uint32_t)(*(uint16_t *)(ptr+1)) << 8;
        value += (uint32_t)(ptr[3]) << 24;
        return value;
    }
}

static inline uint32_t _xxhash_compute(const void *src, uint32_t length, uint32_t seed, bool aligned)
{
    const uint8_t *srcPtr;
    const uint8_t *srcEnd;
    uint32_t vector1;
    uint32_t vector2;
    uint32_t vector3;
    uint32_t vector4;
    uint32_t hashValue;

    /* Initialize pointers. */
    srcPtr = (const uint8_t *)src;
    srcEnd = srcPtr + length - MIN_VECTOR_LENGTH;
    
    /* Process input in vector form. */
    if (length >= MIN_VECTOR_LENGTH) {
        /* Initialize vectors. */
        vector1 = seed + XXHASH_PRIME1 + XXHASH_PRIME2;
        vector2 = seed + XXHASH_PRIME2;
        vector3 = seed + 0;
        vector4 = seed - XXHASH_PRIME1;

        /* Process input in blocks of 16 bytes. */
        while (srcPtr <= srcEnd) {
#if defined(__MSP430_HAS_MPY32__)
            uint32_t temp;
    
            /* Load PRIME2 constant */
            MPY32L = XXHASH_PRIME2 & 0xffff;
            MPY32H = XXHASH_PRIME2 >> 16;
            
            /* Multiply inputs by PRIME2 constant. */
            temp = read32(&srcPtr[0], aligned);
            OP2L = temp & 0xffff;
            OP2H = temp >> 16;
            temp = RES0;
            temp |= ((uint32_t)RES1 << 16);
            vector1 += temp;
            temp = read32(&srcPtr[4], aligned);
            OP2L = temp & 0xffff;
            OP2H = temp >> 16;
            temp = RES0;
            temp |= ((uint32_t)RES1 << 16);
            vector2 += temp;
            temp = read32(&srcPtr[8], aligned);
            OP2L = temp & 0xffff;
            OP2H = temp >> 16;
            temp = RES0;
            temp |= ((uint32_t)RES1 << 16);
            vector3 += temp;
            temp = read32(&srcPtr[12], aligned);
            OP2L = temp & 0xffff;
            OP2H = temp >> 16;
            temp = RES0;
            temp |= ((uint32_t)RES1 << 16);
            vector4 += temp;
            
            /* Rotate vectors. */
            vector1 = rotl32(vector1, 13);
            vector2 = rotl32(vector2, 13);
            vector3 = rotl32(vector3, 13);
            vector4 = rotl32(vector4, 13);
            
            /* Load PRIME1 constant */
            MPY32L = XXHASH_PRIME1 & 0xffff;
            MPY32H = XXHASH_PRIME1 >> 16;
            
            /* Multiply inputs by PRIME1 constant. */
            OP2L = vector1 & 0xffff;
            OP2H = vector1 >> 16;
            vector1 = RES0;
            vector1 |= ((uint32_t)RES1 << 16);
            OP2L = vector2 & 0xffff;
            OP2H = vector2 >> 16;
            vector2 = RES0;
            vector2 |= ((uint32_t)RES1 << 16);
            OP2L = vector3 & 0xffff;
            OP2H = vector3 >> 16;
            vector3 = RES0;
            vector3 |= ((uint32_t)RES1 << 16);
            OP2L = vector4 & 0xffff;
            OP2H = vector4 >> 16;
            vector4 = RES0;
            vector4 |= ((uint32_t)RES1 << 16);
            
            /* Increment source pointer. */
            srcPtr += MIN_VECTOR_LENGTH;
#else //__MSP430_HAS_MPY32__
            vector1 += read32(srcPtr, aligned) * XXHASH_PRIME2;
            vector1 = rotl32(vector1, 13);
            vector1 *= XXHASH_PRIME1;
            srcPtr += 4;
            vector2 += read32(srcPtr, aligned) * XXHASH_PRIME2;
            vector2 = rotl32(vector2, 13);
            vector2 *= XXHASH_PRIME1;
            srcPtr += 4;
            vector3 += read32(srcPtr, aligned) * XXHASH_PRIME2;
            vector3 = rotl32(vector3, 13);
            vector3 *= XXHASH_PRIME1;
            srcPtr += 4;
            vector4 += read32(srcPtr, aligned) * XXHASH_PRIME2;
            vector4 = rotl32(vector4, 13);
            vector4 *= XXHASH_PRIME1;
            srcPtr += 4;
#endif //__MSP430_HAS_MPY32__
        }

        /* Calculate hash value from vectors. */
        hashValue = rotl32(vector1, 1) + rotl32(vector2, 7) + rotl32(vector3, 12) + rotl32(vector4, 18);
    }
    else {
        /* Small input, initialize hash from seed and PRIME5. */
        hashValue  = seed + XXHASH_PRIME5;
    }

    /* Add length to has value. */
    hashValue += length;

    /* Calculate hash with last 4-byte blocks. */
    srcEnd += MIN_VECTOR_LENGTH - 4;
    while (srcPtr <= srcEnd) {
        hashValue += read32(srcPtr, aligned) * XXHASH_PRIME3;
        hashValue = rotl32(hashValue, 17) * XXHASH_PRIME4;
        srcPtr += 4;
    }

    /* Calculate hash with last bytes. */
    srcEnd += 4;
    while (srcPtr < srcEnd) {
        hashValue += *srcPtr++ * XXHASH_PRIME5;
        hashValue = rotl32(hashValue, 11) * XXHASH_PRIME1;
    }

    hashValue ^= hashValue >> 15;
    hashValue *= XXHASH_PRIME2;
    hashValue ^= hashValue >> 13;
    hashValue *= XXHASH_PRIME3;
    hashValue ^= hashValue >> 16;

    return hashValue;
}

uint32_t xxhash_compute(const void *src, uint32_t length, uint32_t seed)
{
    /* Check alignment and compute xxhash. */
    if ((uintptr_t)src & 1) {
        return _xxhash_compute(src, length, seed, false);
    }
    else {
        return _xxhash_compute(src, length, seed, true);
    }
}
