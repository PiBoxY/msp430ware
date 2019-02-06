<FMCG_COPYRIGHT>
<FMCG_DATE>
<FMCG_OVERWRITE_DO_NOT_MODIFY>


#ifndef ESICALIBRATION_H_
#define ESICALIBRATION_H_

// Recalibration defines and gRecalFlag bitfields definitions
#define DELTA_LEVEL 					10
#define VALID_RECALIBRATION				BIT0
#define RECALIBRATION_TIMEOUT			BIT1
#define INITIAL_AFE2_RECALIBRATION		BIT5
#define PERIODIC_AFE2_RECALIBRATION		BIT6



//define for gESIStatusFlag
#define VALID_SEPARATION_CH0 						BIT0
#define VALID_SEPARATION_CH1 						BIT1
#define VALID_SEPARATION_CH2 						BIT2
#define DAC_CALIBRATION_COMPLETE					BIT4


<FMCG_ESICAL_H_CHANNEL_CONFIG>




typedef struct fastParams {
        uint8_t channel0Enabled;
        uint8_t channel1Enabled;
        uint8_t channel2Enabled;
        uint16_t channel0StartingPoint;
        uint16_t channel1StartingPoint;
        uint16_t channel2StartingPoint;
        uint16_t range;
} fastParams;

typedef struct findDACParams {
        uint8_t channel0Enabled;
        uint8_t channel1Enabled;
        uint8_t channel2Enabled;
} findDACParams;




extern void ReCalScanIF(void);
extern void TSM_Auto_cal(void);
extern void Find_Noise_level(void);
extern void Set_DAC(void);



#endif /* ESICALIBRATION_H_ */
