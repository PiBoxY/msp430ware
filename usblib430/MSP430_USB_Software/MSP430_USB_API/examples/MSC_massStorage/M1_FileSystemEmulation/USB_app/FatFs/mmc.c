/* --COPYRIGHT--,NULL
 **
 * --/COPYRIGHT--*/
/*------------------------------------------------------------------------/
 * /  Bitbanging MMCv3/SDv1/SDv2 (in SPI mode) control module
 * /-------------------------------------------------------------------------/
 * /
 * /  Copyright (C) 2010, ChaN, all right reserved.
 * /
 * / * This software is a free software and there is NO WARRANTY.
 * / * No restriction on use. You can use, modify and redistribute it for
 * /   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
 * / * Redistributions of source code must retain the above copyright notice.
 * /
 * ---------------------------------------------------------------------------*/

#include "msp430.h"
#include "diskio.h"                                                     /* Common include file for FatFs and disk I/O layer */
#include "mmc.h"
#include "../storageVolume.h"
#include "string.h"
extern const uint16_t BYTES_PER_BLOCK;
extern const uint16_t storageVol[];

/*
 *-------------------------------------------------------------------------
 * Platform dependent RTC Function for FatFs module
 *
 * This could be customized to use an actual real-time clock, but we haven't
 * bothered to do so because it isn't really necessary in this app.  The year
 * is always 2001.
 *
 *-------------------------------------------------------------------------
 */
uint32_t get_fattime (void)
{
    uint32_t fattime, year;

    /* Pack date and time into a uint32_t variable */
    year = (uint32_t)2001 - 80;        // rtcYear
    year = year << 25;
    fattime = year | ((uint32_t)9 << 21)     // rtcMon
                   | ((uint32_t)11 << 16)    // rtcMday
                   | ((uint16_t)(4 << 11))    // rtcHour
                   | ((uint16_t)(30 << 5))    // rtcMin
                   | ((uint16_t)(0 >> 1));    // rtcSec

    return (fattime);
}

/*--------------------------------------------------------------------------
 * Module Private Functions
 * ---------------------------------------------------------------------------*/


static
DSTATUS Stat = STA_NOINIT;                            /* Disk status */


/*--------------------------------------------------------------------------
 *
 * Public Functions
 *
 * ---------------------------------------------------------------------------*/


/*
 *-----------------------------------------------------------------------
 * Get Disk Status
 *-----------------------------------------------------------------------
 */

DSTATUS disk_status (
    uint8_t drv                        /* Drive number (0) */
    )
{
    DSTATUS s = Stat;				// s = Stat = STA_NOINIT = 0x01

    return (s);
}

/*
 *-----------------------------------------------------------------------
 * Initialize Disk Drive
 *-----------------------------------------------------------------------
 */

DSTATUS disk_initialize (
    uint8_t drv    /* Physical drive nmuber (0) */
    )
{
    DSTATUS s;

    s = disk_status(drv);               // s = STA_NOINIT = 0x01

    // We do nothing here, b/c MSP430-internal flash is always ready.

    s &= ~STA_NOINIT;					//s = 0x01 & 0xFE = 0x00
    Stat = s;

    return (s);							// returns 0x00
}

/*
 * Writes a 512-uint8_t block to flash.  Flash segments on MSP430F5xx devices
 * are 512 bytes in size -- same as FAT blocks.
 */
void flashWrite_LBA(uint8_t* flashAddr, uint8_t* data)
{
  uint16_t i;
  unsigned short bGIE;

  bGIE  = (__get_SR_register() & GIE);  //save interrupt status
  __disable_interrupt();

  // Erase the segment
  FCTL3 = FWKEY;                        // Clear the lock bit
  FCTL1 = FWKEY+ERASE;                  // Set the Erase bit
  *flashAddr = 0;                       // Dummy write, to erase the segment

  // Write the new data to the segment
  FCTL1 = FWKEY+WRT;                    // Set WRT bit for write operation
  for (i = 0; i < 512; i++)
  {
    *flashAddr++ = *data++;             // Write the block to flash
  }
  FCTL1 = FWKEY;                        // Clear WRT bit
  FCTL3 = FWKEY+LOCK;                   // Set LOCK bit

  __bis_SR_register(bGIE);              // Restore interrupt status
}


/*
 *-----------------------------------------------------------------------
 * Read Sector(s)
 *-----------------------------------------------------------------------
 */

// Reads 'count' blocks/sectors from the medium, which in this case is the
// storageVol[] array, stored in MSP430 internal flash.
DRESULT disk_read (
    uint8_t drv,         /* Physical drive number (0) */
    uint8_t *buff,       /* Pointer to the data buffer to store read data */
    uint32_t sector,     /* Start sector number (LBA) */
    uint8_t count        /* Sector count (1..128) */
    )
{
    uint8_t i;

    for (i=0; i<count; i++)
    {
    	// Copy the appropriate 512-uint8_t block out of storageVol
        memcpy(buff, (const void *) ((sector * 512) + (uint8_t*)&storageVol),
        		BYTES_PER_BLOCK);

        sector++;
        buff += BYTES_PER_BLOCK;
    }
    return RES_OK;
}

/*
 *-----------------------------------------------------------------------
 * Write Sector(s)
 *-----------------------------------------------------------------------
 */

// Writes 'count' blocks/sectors to the medium, which in this case is the
// storageVol[] array, stored in MSP430 internal flash.
DRESULT disk_write (
	    uint8_t drv,         /* Physical drive number (0) */
        const uint8_t *buff, /* Pointer to the data to be written */
        uint32_t sector,     /* Start sector number (LBA) */
        uint8_t count        /* Sector count (1..128) */
        )
{
	uint8_t i;

    for (i=0; i<count; i++)
    {
        flashWrite_LBA((uint8_t*)((sector*512) + (uint8_t*)&storageVol),(uint8_t*)buff);
        sector++;
        buff += BYTES_PER_BLOCK;
    }

    return  RES_OK;
}

/*
 *-----------------------------------------------------------------------
 * Miscellaneous Functions
 *-----------------------------------------------------------------------
 */
DRESULT disk_ioctl (
    uint8_t drv,               /* Physical drive number (0) */
    uint8_t ctrl,              /* Control code */
    uint32_t *buff             /* Buffer to send/receive control data */
    )
{
    DRESULT res;

    res = RES_ERROR;
    switch (ctrl){

        /* Make sure that no pending write process */
        case CTRL_SYNC:
            res = RES_OK;
            break;

        /* Get number of sectors on the disk (uint32_t) */
        case GET_SECTOR_COUNT:
        	*buff = NUMBER_OF_SECTORS_IN_FLASH;
            res = RES_OK;
            break;

        /* Get erase block size in unit of sector (uint32_t) */
        case GET_BLOCK_SIZE:
            *(uint32_t*)buff = 512;
            res = RES_OK;
            break;

        default:
            res = RES_PARERR;
    }

    return (res);
}

//Released_Version_5_20_06_02
