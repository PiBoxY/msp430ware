/* --COPYRIGHT--,FRAM-Utilities
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * This source code is part of FRAM Utilities for MSP430 FRAM Microcontrollers.
 * Visit http://www.ti.com/tool/msp-fram-utilities for software information and
 * download.
 * --/COPYRIGHT--*/
#ifndef LZ4_STREAM_H_
#define LZ4_STREAM_H_

//******************************************************************************
//
//! \addtogroup lz4_api
//! @{
//
//******************************************************************************

#include "lz4.h"

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

//******************************************************************************
//
//! \brief LZ4 streaming API state.
//
//******************************************************************************
typedef enum {
    //! Next byte is the token
    LZ4_BLOCK_SIZE,
    //! Next byte is the token
    LZ4_TOKEN,
    //! Next byte is the literal length
    LZ4_LITERAL_LENGTH,
    //! Next byte is a literal character
    LZ4_LITERAL,
    //! Next byte is the match length
    LZ4_MATCH_LENGTH,
    //! Next byte is the low match address offset
    LZ4_MATCH_OFFSET_LOW,
    //! Next byte is the high match address offset
    LZ4_MATCH_OFFSET_HIGH
} lz4_stream_state;

//******************************************************************************
//
//! \brief Decompression state for a streaming LZ4 block.
//
//******************************************************************************
typedef struct lz4_stream_decompressBlockState {
    //! Decompress stream state.
    lz4_stream_state state;
    //! Length of literals.
    uint16_t literalLength;
    //! Length of match.
    uint16_t matchLength;
    //! Offset address of match.
    uint16_t matchOffset;
    //! Pointer to the destination data buffer to write.
    uint8_t *dstPtr;
    //! Destination buffer pointer origin.
    uint8_t *dstOrigin;
    //! Length of the destination data buffer.
    //!
    uint32_t dstLength;
} lz4_stream_decompressBlockState;

//******************************************************************************
//
//! \brief Decompression parameters for a streaming LZ4 block.
//
//******************************************************************************
typedef struct lz4_stream_decompressBlockParams {
    //! Pointer to the destination data buffer to store compressed data.
    void *dst;
    //! Length of the destination data buffer.
    int32_t dstLength;
    //! Compressed LZ4 data contains block size.
    //! - false
    //! - true
    bool containsBlockSize;
} lz4_stream_decompressBlockParams;

//******************************************************************************
//
//! \brief Initialize LZ4 stream decompression.
//!
//! Initialize LZ4 decompression using a stream of data blocks. The streaming
//! API's can be used when data is sent in chunks such as over-the-air or wired
//! serial communication and removes the need to buffer then entire compressed
//! data before running decompression, reducing total system memory used.
//!
//! This function must be first called to initialize the state before calling
//! lz4_stream_decompressBlock() to decompress data.
//!
//! \param params Pointer to the LZ4 decompression strean parameter structure.
//! \param state  Pointer to the LZ4 decompression stream state.
//! \param status Pointer to a LZ4 status that will contain the result status.
//! \return none
//
//******************************************************************************
extern void lz4_stream_decompressBlockInit(const lz4_stream_decompressBlockParams *params, lz4_stream_decompressBlockState *state, lz4_status *status);

//******************************************************************************
//
//! \brief Decompress a single LZ4 block as a stream of data.
//!
//! Continue decompression using a stream of data blocks. The streaming API's
//! can be used when data is sent in chunks such as over-the-air or wired
//! serial communication and removes the need to buffer then entire compressed
//! data before running decompression, reducing total system memory used.
//!
//! The lz4_stream_decompressBlockInit() function must first be called to
//! initialize the state before decompressing data.
//!
//! \param state    Pointer to the LZ4 decompression stream state.
//! \param data     Pointer to a block of data to continue decompression with.
//! \param length   Length of data block in bytes.
//! \param status   Pointer to a LZ4 status that will contain the result status.
//! \return The current length of decompressed data.
//
//******************************************************************************
extern uint32_t lz4_stream_decompressBlock(lz4_stream_decompressBlockState *state, const void *data, uint16_t length, lz4_status *status);

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

#endif /* LZ4_STREAM_H_ */
