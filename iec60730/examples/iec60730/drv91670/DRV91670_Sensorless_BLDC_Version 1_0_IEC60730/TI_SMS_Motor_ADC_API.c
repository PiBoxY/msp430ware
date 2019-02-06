#include "TI_SMS_Motor_ADC_API.h"
#include "TI_SMS_Motor_SPI_API.h"
#include "TI_SMS_Motor_PWM_API.h"

UINT32 TI_SMS_MOTOR_ADC_Init() //called in system init
{ 
	UINT32 errCode = 0;
	UINT8 registerValue =0;
	
	drv91670SpiResetAfeSpi();
    drv91670SpiReleaseAfeSpi();
	registerValue = SPI_Read(ASSIMISC3);
	
#ifdef _DEVICE_DRV_91620_ || _DEVICE_DRV_92200 || _DEVICE_DRV_92250 
    registerValue |= BIT2 | ASSADCENA | ASSPDDIS;  //drive gate with VCC + 6.8 voltage called bootSTRAP
    drv91670SpiResetAfeSpi();
    drv91670SpiReleaseAfeSpi();
    SPI_Write(ASSIMISC3, registerValue);
#endif
 
    return (errCode); 
}


UINT32 TI_SMS_MOTOR_ADC_Chn_Read(ADC_CHN_PARAMS_S *p)
{  
	UINT32 errCode = 0;
    volatile UINT8 adcLsb, adcData;
	volatile UINT16 adcData16Bit;
	
	p->dataReady=FALSE;
	// ADC done need to be high for next adc sample, using P3.0 
	while (!(P3IN & ADC_DONE_LOW));
    // reset SPI 
    drv91670SpiResetAfeSpi(); 
    drv91670SpiReleaseAfeSpi();
	// ADC1 register, SEL3-SEL0 = channel_num, ADC START = 1, mask 4 msb, available channels are 0 to 15
    SPI_Write(ASSIADC1, (UINT8)((p->channelID & 0x0F) << 4)); //select ADC channel first.
     
    drv91670SpiResetAfeSpi(); 
    drv91670SpiReleaseAfeSpi();    
    SPI_Write(ASSIADC1, (UINT8)((p->channelID & 0x0F) << 4) | ADC_START);   // after ADC channel is selected, send ADC_START for the channel. 
    
    while(!(P3IN & ADC_DONE_LOW));  // ADC done need to be high for next adc sample 
    // reset SPI 
    drv91670SpiResetAfeSpi();
    drv91670SpiReleaseAfeSpi();
    // Read ADC MSB data bits 
    adcData16Bit = SPI_Read(ASSIADC1) & 0x03;   
    drv91670SpiResetAfeSpi();
    drv91670SpiReleaseAfeSpi();
    // Read ADC LSB data bits 
    adcLsb = SPI_Read(ASSIADC2);
    // make 10 bit data for usage 
    adcData16Bit = (adcData16Bit << 8 & 0x0300) | adcLsb; // make 10 bit number from two 8 bit numbers
    
#if 0
    p->data = (p->data * p->gain );
    p->data = (p->data >> ADC_GAIN_SHIF5 )+ p->offSet;
#endif

    p->data = adcData16Bit;      
    p->dataReady = TRUE;
    return (errCode);
}



#if ADC_MODE == ADC_INTERRUPT_MODE
/* re do ISR for ADC
#pragma vector=PORT3_VECTOR
__interrupt void Port_3(void)
{   
    Hall_sensor_ISR();	
}
*/
#endif 
