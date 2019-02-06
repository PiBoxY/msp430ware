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

#define LZ4_MAGIC_NUMBER                0x184d2204

#define LZ4_MAX_BLOCK_SIZE              0x00100000

#define LZ4_FLG_VERSION                 0x40
#define LZ4_FLG_BLOCK_INDEP             0x20
#define LZ4_FLG_BLOCK_CHECKSUM          0x10
#define LZ4_FLG_CONTENT_SIZE            0x08
#define LZ4_FLG_CONTENT_CHECKSUM        0x04
#define LZ4_BD_BYTE                     0x70

#define LZ4_MIN_MATCH                   4
#define LZ4_LAST_MATCH_DIST             12
#define LZ4_LAST_LITERAL_LENGTH         5

#define LZ4_TOKEN_LITERAL_MASK          0xf0
#define LZ4_TOKEN_MATCH_MASK            0x0f

static inline uint16_t read16(const uint8_t* ptr)
{
    /* Odd address */
    if ((uintptr_t)ptr & 1) {
        uint16_t value = ptr[0];
        value += (uint16_t)ptr[1] << 8;
        return value;
    }
    /* Even address */
    else {
        return *(uint16_t *)ptr;
    }
}

static inline uint32_t read32(const uint8_t* ptr)
{
    /* Odd address */
    if ((uintptr_t)ptr & 1) {
        uint32_t value = ptr[0];
        value += (uint32_t)(*(uint16_t *)(ptr+1)) << 8;
        value += (uint32_t)(ptr[3]) << 24;
        return value;
    }
    /* Even address */
    else {
        return *(uint32_t *)ptr;
    }
}

static inline uint8_t *write16(uint8_t *ptr, uint16_t data)
{
    if ((uintptr_t)ptr & 1) {
        *ptr++ = data & 0xff;
        *ptr++ = (data >> 8) & 0xff;
        return ptr;
    }
    else {
        *((uint16_t *)ptr) = data;
        return ptr+2;
    }
}

static inline uint8_t *write32(uint8_t *ptr, uint32_t data)
{
    if ((uintptr_t)ptr & 1) {
        *ptr++ = data & 0xff;
        *ptr++ = (data >> 8) & 0xff;
        *ptr++ = (data >> 16) & 0xff;
        *ptr++ = (data >> 24) & 0xff;
        return ptr;
    }
    else {
        *((uint32_t *)ptr) = data;
        return ptr+4;
    }
}

#if defined(LZ4_CRC16_HASH)

#if !defined(__MSP430_HAS_CRC__)
static inline uint16_t crc16(uint8_t* data_p, uint16_t length){
    uint8_t x;
    uint16_t crc = 0xFFFF;

    while (length--){
        x = crc >> 8 ^ *data_p++;
        x ^= x>>4;
        crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x <<5)) ^ ((uint16_t)x);
    }
    return crc;
}
#endif // __MSP430_HAS_CRC16__

static inline uint16_t calculateHashCRC16(uint32_t input)
{
#if defined(__MSP430_HAS_CRC__)
    CRCINIRES = 0xffff;
    CRCDIRB = input & 0xffff;
    CRCDIRB = (input >> 16) & 0xffff;
    return CRCINIRES;
#else // __MSP430_HAS_CRC16__
    return crc16((uint8_t *)&input, 4);
#endif // __MSP430_HAS_CRC16__
}

#else // LZ4_CRC16_HASH

#define LZ4_HASH_VALUE                  0x9E3779B1

static inline uint16_t calculateHashMPY32(uint32_t input)
{
#if defined(__MSP430_HAS_MPY32)
    MPY32L = LZ4_HASH_VALUE & 0xffff;
    MPY32H = (LZ4_HASH_VALUE >> 16) & 0xffff;
    OP2L = input & 0xffff;
    OP2H = (input >> 16) & 0xffff;
    return RES1;
#else // __MSP430_HAS_MPY32
    return (((input) * LZ4_HASH_VALUE) >> 16);
#endif // __MSP430_HAS_MPY32
}

#endif // LZ4_CRC16_HASH

static inline uint16_t calculateHash(uint32_t input)
{
#if defined(LZ4_CRC16_HASH)
    return calculateHashCRC16(input);
#else // LZ4_CRC16_HASH
    return calculateHashMPY32(input);
#endif // LZ4_CRC16_HASH
}

static inline uint16_t checkMatch(const uint8_t *pos1, const uint8_t *pos2)
{
    uint16_t length = 0;
    while ((*pos1++ == *pos2++) && (length != 0xffff)) {
        length++;
    }
    return length;
}

uint32_t lz4_compress(const lz4_compressParams *params, lz4_status *status)
{
    uint8_t *dstPtr;
    uint8_t *posPtr;
    uint8_t flgByte;
    uint32_t dstLength;
    uint32_t checksum;
    uint32_t blockLength;
    const uint8_t *srcPtr;
    const uint8_t *blockPtr;
    lz4_compressBlockParams blockParams;
    
    /* Initialize local pointers. */
    srcPtr = params->src;
    dstPtr = params->dst;
    posPtr = dstPtr;
    
    /* Set frame magic number */
    posPtr = write32(posPtr, LZ4_MAGIC_NUMBER);
    
    /* Set LZ4 frame FLG byte. */
    flgByte = LZ4_FLG_VERSION | LZ4_FLG_BLOCK_INDEP;
    if (params->addBlockChecksum) {
        flgByte |= LZ4_FLG_BLOCK_CHECKSUM;
    }
    if (params->addContentSize) {
        flgByte |= LZ4_FLG_CONTENT_SIZE;
    }
    if (params->addContentChecksum) {
        flgByte |= LZ4_FLG_CONTENT_CHECKSUM;
    }
    *posPtr++ = flgByte;
    
    /* Set LZ4 fram BD byte. */
    *posPtr++ = LZ4_BD_BYTE;
    
    /* Set content size if parameter is enabled. */
    if (params->addContentSize) {
        posPtr = write32(posPtr, params->length);
        posPtr = write32(posPtr, 0);
    }
    
    /* Calculate and set header checksum */
    checksum = xxhash_compute((void *)(dstPtr+4), (uintptr_t)posPtr - (uintptr_t)(dstPtr+4), 0);
    *posPtr++ = (checksum >> 8) & 0xff;
    
    /* Compress data blocks. */
    blockPtr = params->src;
    blockLength = params->length;
    blockParams.hashTable = params->hashTable;
    blockParams.hashLog2Size = params->hashLog2Size;
    blockParams.addBlockChecksum = params->addBlockChecksum;
    while (blockLength > LZ4_MAX_BLOCK_SIZE) {
        blockParams.src = blockPtr;
        blockParams.dst = posPtr;
        blockParams.length = LZ4_MAX_BLOCK_SIZE;
        dstLength = lz4_compressBlock(&blockParams, status);
        
        /* Increment pointer by compressed size. */
        posPtr += dstLength;
        
        /* Increment block pointer and decrement block length. */
        blockPtr += LZ4_MAX_BLOCK_SIZE;
        blockLength -= LZ4_MAX_BLOCK_SIZE;
    }
    
    /* Compress final block. */
    blockParams.src = blockPtr;
    blockParams.dst = posPtr;
    blockParams.length = blockLength;
    dstLength = lz4_compressBlock(&blockParams, status);
        
    /* Increment pointer by compressed size. */
    posPtr += dstLength;
    
    /* Set end mark. */
    posPtr = write32(posPtr, 0);
    
    /* Set content checksum if parameter is enabled. */
    if (params->addContentChecksum) {
        /* Calculate content checksum and add to end of the frame. */
        checksum = xxhash_compute((void *)srcPtr, params->length, 0);
        posPtr = write32(posPtr, checksum);
    }
    
    /* Return total frame size. */
    return ((uintptr_t)posPtr - (uintptr_t)dstPtr);
}

uint32_t lz4_compressBlock(const lz4_compressBlockParams *params, lz4_status *status)
{
    uint8_t token;
    uint8_t *dstPtr;
    uint8_t *dstOrigin;
    const uint8_t *srcPtr;
    const uint8_t *posPtr;
    const uint8_t *matchPtr;
    const uint8_t *srcPtrEnd;
    uint16_t index;
    uint16_t posOffset;
    uint16_t backOffset;
    uint16_t matchOffset;
    uint16_t *hashTable;
    uint16_t hashMask;
    uint16_t hashLength;
    uint16_t matchLength;
    uint16_t literalLength;
    uint16_t remainingLength;
    uint32_t checksum;
    uint32_t blockLength;
    
    /* Initialize local pointers and counters and masks. */
    dstOrigin = params->dst;
    dstPtr = dstOrigin+4;
    srcPtr = params->src;
    posPtr = params->src;
    matchPtr = params->src;
    srcPtrEnd = srcPtr + params->length - LZ4_LAST_MATCH_DIST;

    /* Initialize the provided hash table */
    hashTable = (uint16_t *)params->hashTable;
    hashLength = 1 << (params->hashLog2Size - 1);
    hashMask = hashLength - 1;
    while (hashLength--) {
        *hashTable++ = 0xffff;
    }
    hashTable = (uint16_t *)params->hashTable;
          
    /* Run until the end of input */
    while (matchPtr < srcPtrEnd) {
        /* Calculate the hash index. */
        index = calculateHash(read32(matchPtr)) & hashMask;
        
        /* Check if index is used */
        posOffset = (uintptr_t)matchPtr - (uintptr_t)srcPtr;
        matchOffset = hashTable[index];
        backOffset = posOffset - matchOffset;
        if ((matchOffset != 0xffff) && (backOffset != 0)) {
            /* Check if there is a match. */
            matchLength = checkMatch(matchPtr, matchPtr-backOffset);
            if (matchLength >= LZ4_MIN_MATCH) {
                /* Saturate match length to end of src block */
                if ((matchPtr + matchLength) > (srcPtrEnd + (LZ4_LAST_MATCH_DIST-LZ4_LAST_LITERAL_LENGTH))) {
                    matchLength = srcPtrEnd + (LZ4_LAST_MATCH_DIST-LZ4_LAST_LITERAL_LENGTH) - matchPtr;
                }
                
                /* Calculate token length values. */
                matchLength -= LZ4_MIN_MATCH;
                literalLength = (uintptr_t)matchPtr - (uintptr_t)posPtr;
                
                /* Calculate token value and write it to destination. */
                if (matchLength >= 15) {
                    token = LZ4_TOKEN_MATCH_MASK;
                }
                else {
                    token = matchLength & LZ4_TOKEN_MATCH_MASK;
                }
                if (literalLength >= 15) {
                    /* Add additional literal length codes */
                    *dstPtr++ = LZ4_TOKEN_LITERAL_MASK | token;
                    literalLength -= 15;
                    while (literalLength >= 255) {
                        *dstPtr++ = 255;
                        literalLength -= 255;
                    }
                    *dstPtr++ = literalLength;
                }
                else {
                    *dstPtr++ = ((literalLength << 4) & LZ4_TOKEN_LITERAL_MASK) | token;
                }
                
                /* Copy the literals. */
                while (posPtr != matchPtr) {
                    *dstPtr++ = *posPtr++;
                }
                
                /* Add the 16-bit back pointer */
                dstPtr = write16(dstPtr, backOffset);
                    
                /* Add additional literal length codes if necessary. */
                if (matchLength >= 15) {
                    uint16_t matchLengthCopy = matchLength;
                    matchLengthCopy -= 15;
                    while (matchLengthCopy >= 255) {
                        *dstPtr++ = 255;
                        matchLengthCopy -= 255;
                    }
                    *dstPtr++ = matchLengthCopy;
                }
                
                /* Store position on hash */
                hashTable[index] = posOffset;
                
                /* Move positional pointer forwards past match and update hash table. */
                matchPtr += matchLength + LZ4_MIN_MATCH;
                posPtr = matchPtr;
                posOffset = (uintptr_t)posPtr - (uintptr_t)srcPtr;
                index = calculateHash(read32(posPtr-2)) & hashMask;
                hashTable[index] = posOffset-2;
            }
            else {
                /* Store the current back pointer and increment match pointer. */
                hashTable[index] = posOffset;
                matchPtr++;
            }
        }
        else {
            /* Store the current back pointer and increment match pointer. */
            hashTable[index] = posOffset;
            matchPtr++;
        }
    }
    
    /* Calculate remaining literals length. */
    srcPtrEnd += LZ4_LAST_MATCH_DIST;
    remainingLength = (uintptr_t)srcPtrEnd - (uintptr_t)posPtr;
    
    /* Set remaining literals token and additional length bytes if necessary. */
    if (remainingLength >= 15) {
        *dstPtr++ = LZ4_TOKEN_LITERAL_MASK;
        remainingLength -= 15;
        while (remainingLength >= 255) {
            *dstPtr++ = 255;
            remainingLength -= 255;
        }
        *dstPtr++ = remainingLength;
    }
    else {
        *dstPtr++ = ((remainingLength << 4) & LZ4_TOKEN_LITERAL_MASK);
    }
                
    /* Copy the literals. */
    while (posPtr < srcPtrEnd) {
        *dstPtr++ = *posPtr++;
    }
    
    /* Write the block size. */
    blockLength = (uintptr_t)dstPtr - (uintptr_t)(dstOrigin+4);
    write32(dstOrigin, blockLength);
        
    /* Set block checksum if parameter is enabled. */
    if (params->addBlockChecksum) {
        /* Calculate block checksum and add to end of block. */
        checksum = xxhash_compute((void *)(dstOrigin+4), blockLength, 0);
        dstPtr = write32(dstPtr, checksum);
    }
    
    /* Set status and return size. */
    *status = LZ4_SUCCESS;
    return ((uintptr_t)dstPtr - (uintptr_t)dstOrigin);
}

uint32_t lz4_decompress(const lz4_decompressParams *params, lz4_status *status)
{
    uint8_t *dstPtr;
    const uint8_t *srcPtr;
    uint8_t *dstOrigin;
    uint8_t flgByte;
    uint32_t checksum;
    uint32_t dstLength;
    uint32_t blockLength;
    uint32_t dstBlockLength;
    lz4_decompressBlockParams blockParams;
    
    /* Initialize local pointers and variables. */
    srcPtr = params->src;
    dstOrigin = params->dst;
    dstPtr = dstOrigin;
    dstLength = params->dstLength;
    
    /* Check frame magic number */
    if (read32(srcPtr) != LZ4_MAGIC_NUMBER) {
        /* ERROR, frame magic number mismatch. */
        *status = LZ4_FRAMING_ERROR;
        return 0;
    }
    srcPtr += 4;
    
    /* Read LZ4 frame FLG and BD bytes. */
    flgByte = *srcPtr;
    
    /* Verfy header checksum */
    if (flgByte & LZ4_FLG_CONTENT_SIZE) {
        checksum = xxhash_compute((void *)srcPtr, 10, 0);
        if (srcPtr[10] != ((checksum >> 8) & 0xff)) {
            /* ERROR, frame checksum mismatch. */
            *status = LZ4_FRAMING_ERROR;
            return 0;
        }
        srcPtr += 11;
    }
    else {
        checksum = (xxhash_compute((void *)srcPtr, 2, 0) >> 8) & 0xff;
        if (srcPtr[2] != checksum) {
            /* ERROR, frame checksum mismatch. */
            *status = LZ4_FRAMING_ERROR;
            return 0;
        }
        srcPtr += 3;
    }
    
    /* Process data blocks */
    blockParams.verifyBlockChecksum = params->verifyBlockChecksum && (flgByte & LZ4_FLG_BLOCK_CHECKSUM);
    blockLength = read32(srcPtr);
    while (blockLength != 0) {
        /* Add checksum size to block length if present. */
        if (flgByte & LZ4_FLG_BLOCK_CHECKSUM) {
            blockLength += 4;
        }
        
        /* Set block parameters. */
        blockParams.dst = dstPtr;
        blockParams.src = srcPtr;
        blockParams.dstLength = dstLength;
            
        /* Decompress block. */
        dstBlockLength = lz4_decompressBlock(&blockParams, status);
        
        /* Increment source and destination pointers. */
        srcPtr += blockLength + 4;
        dstPtr += dstBlockLength;
        dstLength -= dstBlockLength;
        
        /* Check status of block decompress operation. */
        if (*status != LZ4_SUCCESS) {
            /* Return partial decompressed length. */
            dstLength = (uintptr_t)dstPtr - (uintptr_t)dstOrigin;
            return dstLength;
        }
        
        /* Read next block size. */
        blockLength = read32(srcPtr);
    }
    
    /* Get total decompressed size. */
    dstLength = (uintptr_t)dstPtr - (uintptr_t)dstOrigin;
            
    /* Verfy content checksum if enabled. */
    if ((flgByte & LZ4_FLG_CONTENT_CHECKSUM) && params->verifyContentChecksum) {
        checksum = xxhash_compute((void *)dstOrigin, dstLength, 0);
        if (checksum != read32(srcPtr+4)) {
            /* ERROR, content checksum mismatch. */
            *status = LZ4_CONTENT_CHECKSUM_ERROR;
            return 0;
        }
    }
    
    /* Return the total decompressed length. */
    return dstLength;
}

uint32_t lz4_decompressBlock(const lz4_decompressBlockParams *params, lz4_status *status)
{
    uint8_t token;
    uint8_t *dstPtr;
    uint8_t *dstOrigin;
    const uint8_t *srcPtr;
    const uint8_t *srcEnd;
    uint16_t matchOffset;
    uint16_t matchLength;
    uint16_t literalLength;
    int32_t dstLength;
    uint32_t checksum;
    uint32_t blockLength;
    
    /* Initialize local pointers and variables. */
    srcPtr = params->src;
    dstOrigin = params->dst;
    dstPtr = dstOrigin;
    dstLength = params->dstLength;
    
    /* Read block length and increment source pointer. */
    blockLength = read32(srcPtr);
    srcPtr += 4;
    srcEnd = srcPtr + blockLength;
    
    /* Check if checksum is present */
    if (params->verifyBlockChecksum) {
        checksum = xxhash_compute((void *)(srcPtr), blockLength, 0);
        if (checksum != read32(srcPtr+blockLength)) {
            /* ERROR, block checksum mismatch. */
            *status = LZ4_BLOCK_CHECKSUM_ERROR;
            return 0;
        }
    }
    
    /* Decompress the data block. */
    while (srcPtr < srcEnd) {
        /* Read in token byte and extract literal and match lengths. */
        token = *srcPtr++;
        matchLength = (token & LZ4_TOKEN_MATCH_MASK) + LZ4_MIN_MATCH;
        literalLength = (token & LZ4_TOKEN_LITERAL_MASK) >> 4;
        
        /* Read in additional literal length bytes if necessary. */
        if (literalLength == 15) {
            while (*srcPtr == 255) {
                literalLength += *srcPtr++;
            }
            literalLength += *srcPtr++;
        }
        
        /* Check that there are enough bytes remaining in the dst buffer. */
        if ((int32_t)literalLength > dstLength) {
            /* Copy as many bytes as possible. */
            while (dstLength--) {
                *dstPtr++ = *srcPtr++;
            }
            
            /* Set status as partial and return the total block length. */
            *status = LZ4_PARTIAL_SUCCESS;
            return ((uintptr_t)dstPtr - (uintptr_t)dstOrigin);
        }
        
        /* Copy literal bytes to destination buffer. */
        dstLength -= literalLength;
        while (literalLength--) {
            *dstPtr++ = *srcPtr++;
        }
        
        /* Check if we're at the end of the block since last token does not contain match. */
        if (srcPtr >= srcEnd) {
            break;
        }
        
        /* Read in match offset. */
        matchOffset = read16(srcPtr);
        srcPtr += 2;
        
        /* Read in additional match length bytes if necessary. */
        if (matchLength == (15 + LZ4_MIN_MATCH)) {
            while (*srcPtr == 255) {
                matchLength += *srcPtr++;
            }
            matchLength += *srcPtr++;
        }
        
        /* Check that match offset is within range. */
        if (matchOffset > ((uintptr_t)dstPtr - (uintptr_t)dstOrigin)) {
            *status = LZ4_PARTIAL_SUCCESS;
            return 0;
        }
        
        /* Check that there are enough bytes remaining in the dst buffer. */
        if ((int32_t)matchLength > dstLength) {
            /* Copy as many bytes as possible. */
            while (dstLength--) {
                *dstPtr = *(dstPtr-matchOffset);
                dstPtr++;
            }
            
            /* Set status as partial and return the total block length. */
            *status = LZ4_PARTIAL_SUCCESS;
            return ((uintptr_t)dstPtr - (uintptr_t)dstOrigin);
        }
        
        /* Copy match bytes to destination buffer. */
        dstLength -= matchLength;
        while (matchLength--) {
            *dstPtr = *(dstPtr-matchOffset);
            dstPtr++;
        }
    }
    
    /* Set status as success and return the total block length. */
    *status = LZ4_SUCCESS;
    return ((uintptr_t)dstPtr - (uintptr_t)dstOrigin);
}

uint32_t lz4_getContentSize(const uint8_t *src, lz4_status *status)
{
    const uint8_t *srcPtr;
    uint32_t contentSize;
    
    /* Initialize local pointers and variables. */
    srcPtr = src;
    
    /* Check frame magic number to make sure header is valid. */
    if (read32(srcPtr) != LZ4_MAGIC_NUMBER) {
        /* ERROR, frame magic number mismatch. */
        *status = LZ4_FRAMING_ERROR;
        return 0;
    }
    srcPtr += 4;
    
    /* Check if content size flag is set. */
    if (*srcPtr & LZ4_FLG_CONTENT_SIZE) {
        /* Read and return content size. */
        contentSize = read32(srcPtr+2);
        *status = LZ4_SUCCESS;
        return contentSize;
    }
    else {
        /* Content size flag is not set. */
        *status = LZ4_NO_CONTENT_SIZE;
        return 0;
    }
}
