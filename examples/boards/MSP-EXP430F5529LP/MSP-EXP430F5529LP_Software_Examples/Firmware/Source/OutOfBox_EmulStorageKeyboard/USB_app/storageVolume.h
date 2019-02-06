/*
 * storageVolume.h
 *
 *  Created on: Aug 4, 2013
 *      Author: a0199742
 */

#ifndef STORAGEVOLUME_H_
#define STORAGEVOLUME_H_


/* This needs to reflect the actual amount of data in storageVol[].  With the
 * current linker settings, storageVol occupies 0x10000-->0x24400, so this
 * number is (0x24400 - 0x10000)/512 = 162 sectors.
 */
#define NUMBER_OF_SECTORS_IN_FLASH	162


#endif /* STORAGEVOLUME_H_ */
