/* ESI_ESIOSC.h
 *
 */

#ifndef ESI_OSC_H_
#define ESI_OSC_H_

//---- ESIOSC frequency setting
#define ESIOSC_Default  146    // ESIOSC frequency is 4.784 MHz

#define ESIOSC_3MHz      92    // ESIOSC frequency is 3.015 MHz
#define ESIOSC_4MHz     122    // ESIOSC frequency is 3.998 MHz
#define ESIOSC_5MHz     153    // ESIOSC frequency is 5.014 MHz
#define ESIOSC_6MHz     183    // ESIOSC frequency is 5.997 MHz
#define ESIOSC_7MHz     214    // ESIOSC frequency is 7.012 MHz

void EsioscInit(unsigned char frequency);
unsigned char EsioscReCal(unsigned char target);
unsigned char EsioscMeasure(void);

#endif /* ESI_OSC_H_ */
