/*
 * This software adjusts the ESI internal oscillator frequency (ESIOSC).
 *
 */
#include "msp430FR6989.h"

unsigned char v_status=0;

//--------------------------------------------------------------------------

void setESICLKFQ(unsigned char setting);
unsigned char getESICLKFQ();
unsigned char MeasureEsiosc_Oversampling(void);
unsigned char EsioscMeasure();

//--------------------------------------------------------------------------

unsigned char getESICLKFQ()
// return value: actual ESICLKFQ bits
{	unsigned int temp;

	temp = ESIOSC;                    					 // get ESICLKFQx bits
	temp = (temp>>8) & 0x3F;
	return temp;
}

void setESICLKFQ(unsigned char setting)
// setting: new ESICLKFQ setting is loaded into ESIOSC
{	unsigned int temp;

	temp = ESIOSC;                     					// get actual ESIOSC register content
	temp &= ~(0x3F00);
	temp = ((unsigned int) setting << 8) + temp; 		// and update ESICLKFQ bits
	ESIOSC = temp;
}

unsigned char MeasureEsiosc_Oversampling()
// this function does an averaging of 4 ESIOSC measurement results
{ unsigned int temp;

  temp = (unsigned int) EsioscMeasure();
  temp = temp + (unsigned int) EsioscMeasure();
  temp = temp + (unsigned int) EsioscMeasure();
  temp = temp + (unsigned int) EsioscMeasure();
  temp = temp / 4;
  return (unsigned char)temp;
}

//--------------------------------------------------------------------------
//---  ESI/ESIOSC Public Functions
//---

unsigned char EsioscMeasure()
//--- ensure that ACLK is stable running before calling this function.
// return parameter is measurement result (ESICNT3 value)
{	unsigned int temp;

	ESIOSC &= ~(ESICLKGON);     						// This and next instruction realizes
	                            						//  a clear->set ESICLKGON bit.
	ESIOSC |= ESICLKGON + ESIHFSEL;    					// This starts measurement.
	do{
		temp = ESICNT3;         						// get counter value
	} while(temp == 0x01);      						// when measurement has not finished
	                            						//  yet, ESICNT3 value is 0x01
	ESIOSC &= ~(ESICLKGON);     						// Stop ESIOSC oscillator
	return temp;                						// function returns counter value ESICNT3
}

void EsioscInit(unsigned char target)
//--- ensure that ACLK is stable running before calling this function.
// target: number of ESIOSC cycles within one ACLK period
//            (e.g. 4.8MHz/ACLK = 4.8MHz/32768Hz = 146.484375 ~ 146)
{
	unsigned char v_Measure;  							// storing the measurement result
	signed short v_Delta;     							// storing the delta between measurement and target
	unsigned char v_Setting;  							// storing actual ESIOSC bit setting
	signed short v_min;       							// storing minimum delta (v_Delta) within measurement sequence
	signed char v_adder;      							// defines if ESIOSC settings should be incremented or decremented
	unsigned char var;

	  // decide if actual ESIOSC frequency is too high or too low
	  v_Measure = MeasureEsiosc_Oversampling();
	  v_min = v_Measure - target;
	  v_Setting = getESICLKFQ();
	  if (v_Measure > target)
	  	v_adder = -1;         							// decrement should be used for following adjustment loop
	  else if (v_Measure < target)
		v_adder = 1;          							// increment should be used for following adjustment loop

	  if (v_Measure != target) 							// continue only if ESIOSC frequency is not target frequency
	  {  // following measurements: adjust ESIOSC frequency till it is closest to target
	    do
	    {
	      var = getESICLKFQ();
	      if ((var==0x00) || (var==0x3F))  				// check for under- or overflow
	      {	  break;
	      }
	      setESICLKFQ(var+v_adder);
	      v_Measure = MeasureEsiosc_Oversampling();
	      v_Delta = v_Measure - target;
	      if (abs(v_Delta) < abs(v_min))
	      {	v_min = v_Delta;
	      	v_Setting = getESICLKFQ();
	      }
	    } while (v_min == v_Delta);
	    setESICLKFQ(v_Setting);
	  }
}


unsigned char EsioscReCal(unsigned char target)
//--- ensure that ACLK is stable running before calling this function.
{  unsigned char val;
   unsigned char RetValue;
   signed short v_Delta;
   volatile signed short v_min;
   volatile unsigned char v_Setting;
   volatile signed char v_adder;

   val = EsioscMeasure();
   v_Delta= val - target;
   switch (v_status)
   { case 0:   //--- idle, no measurement is running. New sequence is started.
     		   if (val > target)
	    	  	 v_adder = -1;         					// decrement should be used for following adjustment loop
		       if (val < target)
   			     v_adder = 1;          					// increment should be used for following adjustment loop
	   	       if (val == target)
	   	       {  RetValue=0x00;       					// ESIOSC frequency and target frequency are already equal
	   	    	  break;
	   	       }
     		   v_min=v_Delta;
     		   v_Setting=getESICLKFQ();
     		   if ((v_Setting==0x00)||(v_Setting==0x3F))
     		   {   RetValue=0xFF;      					// limits for ESICLKFQx bit settings was reached: terminate Re-Calibration
     			   break;
     		   }
     		   setESICLKFQ(v_Setting+v_adder);
	   		   v_status=1;
	   	   	   RetValue=0x01;        					// re-calibration not yet completed. Another function call is needed.
	   	   	   break;

     case 1:   //--- measurement sequence was started.
    	 	   if (abs(v_Delta)<abs(v_min))
    	 	   {  v_min=v_Delta;
    	 	   	  v_Setting=getESICLKFQ();
        		  if ((v_Setting==0x00)||(v_Setting==0x3F))
        		   {   RetValue=0xFF;  					// limits for ESICLKFQx bit settings was reached: terminate Re-Calibration
        			   break;
        		   }
    	 	   	  setESICLKFQ(v_Setting+v_adder);
    	 	   	  RetValue=0x01;    					// re-calibration not yet completed. Another function call is needed.
    	 	   }
    	 	   else
    	 	   {  setESICLKFQ(v_Setting);
    	 		  v_status=0;
    	 		  RetValue=0x00;    					// re-calibration completed. Found new setting.
    	 	   }
 	   	       break;
   }

   return RetValue;
}
