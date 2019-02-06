/*!
 *  @file   CTS_HAL.h
 *
 *  @brief       
 *
 *  @par    Project:
 *             MSP432 Capacitive Touch Library
 *
 *  @par    Developed using:
 *             CCS Version : 6.1.0.00104 , w/support for GCC extensions (--gcc)
 *  \n         IAR Embedded Workbench for ARM Version : 7.40.3.8938
 *
 *  @author  C. Sterzik
 *  @author  T. Hwang
 *  @author  E. Fu
 *  @author	 B. Moore
 *
 *  @version     v0.1
 *
 *  @par    Supported Hardware Configurations:
 *              - TI_CTS_RO_CTIO_TA2_WDTA_HAL()
 *              - TI_CTS_RO_CTIO_TA2_TA3_HAL()
 *              - TI_CTS_fRO_CTIO_TA2_TA3_HAL()
 *
 */

#ifndef CTS_HAL_H_
#define CTS_HAL_H_

#include "structure.h"

void TI_CTS_RO_CTIO_TA2_WDTA_HAL(const struct Sensor *,uint16_t *);

void TI_CTS_RO_CTIO_TA2_TA3_HAL(const struct Sensor *,uint16_t *);

void TI_CTS_fRO_CTIO_TA2_TA3_HAL(const struct Sensor *,uint16_t *);


#endif /* CTS_HAL_H_ */
