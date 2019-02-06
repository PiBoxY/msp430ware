/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
//******************************************************************************
//  lz4_ex3_stream_decompress - Decompress data chunks using stream functions.
// 
//  This example demonstrates how to decompress LZ4 data in chunks of compressed
//  data. The advantage of the stream decompress functions is less data usage
//  due to only needing to buffer small chunks, one at a time. The performance
//  is slightly worse than the non-streaming API's.
//
//  Brent Peterson
//  Texas Instruments Inc.
//  June 2016
//******************************************************************************

#include "msp430.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "lz4.h"
#include "lz4_stream.h"

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
#define CHUNK_SIZE      100

#define HASHSIZE        4096
#define HASHLOG2SIZE    12

#if defined(__TI_COMPILER_VERSION__)

#pragma PERSISTENT(hashMemory)
uint8_t hashMemory[HASHSIZE] = {0};

#pragma PERSISTENT(compressedData)
uint8_t compressedData[LZ4_COMPRESS_MAX_SIZE(DATA_SIZE)] = {0};

#pragma PERSISTENT(decompressedData)
uint8_t decompressedData[DATA_SIZE] = {0};

#elif defined(__IAR_SYSTEMS_ICC__)

__persistent uint8_t hashMemory[HASHSIZE] = {0};

__persistent uint8_t compressedData[LZ4_COMPRESS_MAX_SIZE(DATA_SIZE)] = {0};

__persistent uint8_t decompressedData[DATA_SIZE] = {0};

#endif

volatile uint32_t compressedSize;
volatile uint32_t decompressedSize;

int main( void )
{
    uint8_t *streamPtr;
    uint16_t streamLength;
    lz4_status status;
    lz4_compressBlockParams compressParams;
    lz4_stream_decompressBlockState streamDecompressState;
    lz4_stream_decompressBlockParams streamDecompressParams;

    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    // Compress data using lz4
    compressParams.src = loremIpsum;
    compressParams.dst = compressedData;
    compressParams.length = DATA_SIZE;
    compressParams.hashTable = (void *)hashMemory;
    compressParams.hashLog2Size = HASHLOG2SIZE;
    compressParams.addBlockChecksum = false;
    compressedSize = lz4_compressBlock(&compressParams, &status);

    // Initialize stream decompress API
    streamDecompressParams.dst = decompressedData;
    streamDecompressParams.dstLength = DATA_SIZE;
    streamDecompressParams.containsBlockSize = true;
    lz4_stream_decompressBlockInit(&streamDecompressParams, &streamDecompressState, &status);
    
    // Initialize local pointers
    streamPtr = compressedData;
    streamLength = compressedSize;
    
    // Decompress CHUNK_SIZE blocks of data using streaming API
    while (streamLength > CHUNK_SIZE) {
        decompressedSize = lz4_stream_decompressBlock(&streamDecompressState, streamPtr, CHUNK_SIZE, &status);
        streamPtr += CHUNK_SIZE;
        streamLength -= CHUNK_SIZE;

        // Check that the operation completed successfully
        if (status != LZ4_SUCCESS) {
            while(1);
        }
    }
    
    // Decompress remaining bytes
    decompressedSize = lz4_stream_decompressBlock(&streamDecompressState, streamPtr, streamLength, &status);

    // Check that the operation completed successfully
    if (status != LZ4_SUCCESS) {
        while(1);
    }
    
    // Check the the decompressed data matches the original file
    if (memcmp((void *)loremIpsum, (void *)decompressedData, sizeof(loremIpsum))) {
        while(1);
    }

    return 0;
}
