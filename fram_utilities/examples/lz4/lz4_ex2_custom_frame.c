/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
//******************************************************************************
//  lz4_ex2_custom_frame - Compress a block of text with CRC16 checksum.
// 
//  This example demonstrates how to compress a block of text into a custom
//  frame with CRC16 checksum. The advantage is a significantly faster checksum
//  calculation using the hardware CRC module compared to the software xxHash
//  implementation.
// 
//    +-------------------+-----------------+-----------------------+
//    | 32-bit block size | compressed data | 16-bit CRC16 checksum |
//    +-------------------+-----------------+-----------------------+
//
//  Brent Peterson
//  Texas Instruments Inc.
//  January 2016
//******************************************************************************

#include "msp430.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "lz4.h"

const char loremIpsum[] = "\
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi eget dui at turp\
is blandit condimentum. Sed a nisl turpis. Sed iaculis urna id blandit tristiqu\
e. Ut sagittis sapien sit amet justo pretium tincidunt. Nulla ac hendrerit libe\
ro. Aenean in eros velit. Nulla vel nibh elit. Curabitur mollis quis dolor eget\
 vehicula. Curabitur vitae turpis viverra, mollis mauris tempor, vehicula odio.\
Vivamus feugiat justo dui, id auctor nunc faucibus in. In at tellus id elit sce\
lerisque ornare. Donec at ultricies felis. Sed sed ultrices nibh, sit amet mole\
stie justo. Suspendisse nec eleifend risus. Sed consectetur ullamcorper nisi in\
 tempor. Sed accumsan tortor et enim consectetur, ac vulputate tortor fringilla\
. Aliquam at condimentum massa. Suspendisse eget ante venenatis, facilisis sem \
eu, tempor ligula. Praesent orci diam, aliquam nec velit in, malesuada porttito\
r orci. Sed nibh nulla, molestie id congue vitae, elementum et nisi. Duis matti\
s vel enim vitae vestibulum. Curabitur posuere augue sit amet lacus aliquam, la\
cinia commodo mauris dignissim. Curabitur id posuere nisi. Phasellus bibendum, \
mauris quis maximus mollis, urna nisi pharetra orci, eget pulvinar arcu enim eg\
et mi.\
Fusce sed nibh hendrerit, finibus nunc vestibulum, sagittis purus. Maecenas ut \
tellus lacinia, sodales nisl sit amet, scelerisque erat. Ut et iaculis nulla, s\
it amet lacinia sem. Mauris ut eros scelerisque, sodales quam ac, vestibulum nu\
lla. Curabitur tristique molestie pretium. Pellentesque et ornare est, at ultri\
cies dolor. Ut eu est erat. Aenean vel massa placerat, rutrum ipsum non, tincid\
unt purus. Curabitur gravida gravida pellentesque. In imperdiet, tellus ut inte\
rdum aliquam, orci nisi commodo mauris, vitae dignissim ipsum neque ac nisl. Cr\
as vitae finibus justo. Ut quis congue augue. Nulla ac porta dolor. Proin moles\
tie maximus lacus at mattis. Proin sodales at nibh sit amet lacinia.\
Praesent porta justo libero, faucibus ornare dui aliquam in. Vivamus maximus ma\
ssa quis lacus consequat mattis. Maecenas et luctus sapien. Duis rutrum nisl au\
gue, non finibus ligula bibendum sit amet. Cum sociis natoque penatibus et magn\
is dis parturient montes, nascetur ridiculus mus. Aenean eros libero, sagittis \
bibendum massa eu, convallis iaculis nulla. Integer sit amet ultrices lacus, eg\
estas dapibus felis. Cras iaculis vel nisi a iaculis. In rhoncus nunc ac mauris\
 posuere, et lobortis lacus ultricies. Nulla hendrerit pellentesque sapien vel \
mollis. Praesent congue odio sed pulvinar pharetra. Phasellus vehicula malesuad\
a ligula, consequat iaculis nulla porta vitae. Nulla quam mauris, laoreet nec u\
ltricies efficitur, ullamcorper et est. In congue metus ante, ut sagittis metus\
 ultricies sodales. In fermentum congue accumsan.\
Proin sagittis id eros semper venenatis. Nunc eget metus euismod, ornare nunc e\
get, pellentesque lacus. Etiam efficitur, sem eget pulvinar maximus, magna elit\
 eleifend urna, in commodo risus massa sed tellus. Aenean in dui ante. Phasellu\
s a vehicula velit. Maecenas eget laoreet est, suscipit tristique odio. Nulla i\
d sapien et felis venenatis imperdiet. Phasellus rutrum nisl at leo auctor vulp\
utate. Ut quam urna, facilisis in lectus sed, interdum convallis est. Integer f\
aucibus nulla neque, nec auctor eros fringilla at. Nulla elementum lorem nibh.";

#define DATA_SIZE       (sizeof(loremIpsum))

#define HASHSIZE        4096
#define HASHLOG2SIZE    12

#if defined(__TI_COMPILER_VERSION__)

#pragma PERSISTENT(compressedData)
uint8_t compressedData[LZ4_COMPRESS_MAX_SIZE(DATA_SIZE)] = {0};

#pragma PERSISTENT(decompressedData)
uint8_t decompressedData[DATA_SIZE] = {0};

#elif defined(__IAR_SYSTEMS_ICC__)

__persistent uint8_t compressedData[LZ4_COMPRESS_MAX_SIZE(DATA_SIZE)] = {0};

__persistent uint8_t decompressedData[DATA_SIZE] = {0};

#endif

volatile uint32_t compressedSize;
volatile uint32_t decompressedSize;

extern uint32_t msp_compress(const void *src, void *dst, uint32_t length);
extern uint32_t msp_decompress(const void *src, void *dst, uint32_t length);

int main( void )
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    // Compress text to custom frame using CRC16 for checksum
    compressedSize = msp_compress((const void *)loremIpsum, (void *)compressedData, sizeof(loremIpsum));
    
    // Verify that the data was successfully compressed.
    if (!compressedSize) {
        while(1);
    }

    // Decompress data to original text.
    decompressedSize = msp_decompress((const void *)compressedData, (void *)decompressedData, sizeof(decompressedData));
    
    // Verify that the data was successfully decompressed.
    if (!decompressedSize) {
        while(1);
    }
    
    // Check the the decompressed data matches the original file.
    if (memcmp((void *)loremIpsum, (void *)decompressedData, sizeof(loremIpsum))) {
        while(1);
    }

    return 0;
}

#if defined(__TI_COMPILER_VERSION__)

#pragma PERSISTENT(hashMemory)
uint8_t hashMemory[HASHSIZE] = {0};

#elif defined(__IAR_SYSTEMS_ICC__)

__persistent uint8_t hashMemory[HASHSIZE] = {0};

#endif

uint32_t msp_compress(const void *src, void *dst, uint32_t length)
{
    uint8_t *crcPtr;
    uint8_t *crcEnd;
    uint32_t blockLength;
    lz4_status status;
    lz4_compressBlockParams params;

    // Initialize compression parameters and compress data block
    params.src = src;
    params.dst = dst;
    params.length = length;
    params.hashTable = (void *)hashMemory;
    params.hashLog2Size = HASHLOG2SIZE;
    params.addBlockChecksum = false;
    lz4_compressBlock(&params, &status);
    blockLength = *(uint32_t *)dst;
    
    // Check status for success, if not return length of zero.
    if (status != LZ4_SUCCESS) {
        return 0;
    }
    
    // Calculate CRC16 with seed of 0xffff and append to end of the block.
    crcPtr = (uint8_t *)dst + sizeof(uint32_t);
    crcEnd = crcPtr + blockLength;
    CRCINIRES = 0xffff;
    while (crcPtr < crcEnd) {
        CRCDIRB_L = *crcPtr++;
    }
    *crcPtr++ = CRCINIRES & 0x00ff;
    *crcPtr++ = (CRCINIRES & 0xff00) >> 8;
    
    // Return total frame length (block length + 32-bit length + 16-bit CRC).
    return blockLength+sizeof(uint32_t)+sizeof(uint16_t);
}

uint32_t msp_decompress(const void *src, void *dst, uint32_t length)
{
    uint16_t crcValue;
    const uint8_t *crcPtr;
    const uint8_t *crcEnd;
    uint32_t blockLength;
    lz4_status status;
    lz4_decompressBlockParams params;
    
    // Verify CRC16 with seed of 0xffff.
    blockLength = *(const uint32_t *)src;
    crcPtr = (const uint8_t *)src + sizeof(uint32_t);
    crcEnd = crcPtr + blockLength;
    CRCINIRES = 0xffff;
    while (crcPtr < crcEnd) {
        CRCDIRB_L = *crcPtr++;
    }
    
    // Read CRC and check for match
    crcValue = *crcPtr++;
    crcValue += *crcPtr++ << 8;
    if (crcValue != CRCINIRES) {
        // CRC16 mismatch, return length of zero.
        return 0;
    }

    // Initialize decompression parameters and decompress data block
    params.src = src;
    params.dst = dst;
    params.dstLength = length;
    params.verifyBlockChecksum = false;
    length = lz4_decompressBlock(&params, &status);
    
    // Check status for success, if not return length of zero.
    if (status != LZ4_SUCCESS) {
        return 0;
    }
    
    // Return decompressed length.
    return length;
}
