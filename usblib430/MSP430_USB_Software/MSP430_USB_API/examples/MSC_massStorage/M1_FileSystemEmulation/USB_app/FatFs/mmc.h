/* --COPYRIGHT--,NULL
 **
 * --/COPYRIGHT--*/
#include "diskio.h"


DRESULT disk_write (
    uint8_t drv,			/* Physical drive nmuber (0) */
    const uint8_t *buff,	/* Point16_ter to the data to be written */
    uint32_t sector,		/* Start sector number (LBA) */
    uint8_t count 			/* Sector count (1..128) */
    );

DRESULT disk_read (
    uint8_t drv,			/* Physical drive nmuber (0) */
    uint8_t *buff,			/* Point16_ter to the data buffer to store read data */
    uint32_t sector,		/* Start sector number (LBA) */
    uint8_t count		 	/* Sector count (1..128) */
    );
//Released_Version_5_20_06_02
