#ifndef TI_SMS_MOTOR_ADC_API_H_
#define TI_SMS_MOTOR_ADC_API_H_
#include "TI_SMS_Motor_Com_Def.h"

 
// ADC related 
#define ENABLE_ADC 0x08
#define ADC_SEL_SPEED_IN 0x04  
#define ADC_DONE 0x80  
#define ADC_READ_DELAY 200
#define ADC_START 0x04
#define ADC_DONE_LOW 0xFE    //  BIT 0 used for checking is ADC_DONE output low or high 
#define ADC_DONE_SWITCH_TO_LOW_DELAY 6 // use little bit delay to ADC_DONE bit to switch from high to low 
     
typedef struct { 
	             UINT16  channelID;
	             UINT16  data;
	             UINT16  dataReady; // ADC complete bit to show data is ready for use.
	             UINT16  linkedChannel; // this channel is used in pipeline mode only. This is for Zoom 
                 UINT16  gain;
                 UINT16  offSet;
                } ADC_CHN_PARAMS_S;
                
UINT32 TI_SMS_MOTOR_ADC_Init(); //called in system init
UINT32 TI_SMS_MOTOR_ADC_Chn_Read(ADC_CHN_PARAMS_S *p); 

#endif /*TI_SMS_MOTOR_ADC_API_H_*/
