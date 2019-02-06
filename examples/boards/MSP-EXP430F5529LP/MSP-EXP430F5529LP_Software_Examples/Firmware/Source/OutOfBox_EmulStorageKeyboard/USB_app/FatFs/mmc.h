/* --COPYRIGHT--,NULL
 **
 * --/COPYRIGHT--*/
#include "diskio.h"


DRESULT disk_write (
    BYTE drv,			/* Physical drive nmuber (0) */
    const BYTE *buff,	/* Pointer to the data to be written */
    DWORD sector,		/* Start sector number (LBA) */
    BYTE count 			/* Sector count (1..128) */
    );

DRESULT disk_read (
    BYTE drv,			/* Physical drive nmuber (0) */
    BYTE *buff,			/* Pointer to the data buffer to store read data */
    DWORD sector,		/* Start sector number (LBA) */
    BYTE count		 	/* Sector count (1..128) */
    );
