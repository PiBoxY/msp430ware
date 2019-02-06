/*For MSP430FR2311 */
#define PxSEL0 P1SEL0                         // Port selection
#define PxSEL1 P1SEL1                         // Port selection
#define PxDIR P1DIR                         // Port direction
#define PxOUT P1OUT                         // Port output
#define PxIN  P1IN                          // Port input

#define SDA   BIT2                          // Controls SDA line
#define SCL   BIT3                          // Controls SCL line

#define MSP430CLOCKFREQHZ 8000000           // USER CONFIG: Existing Sys Freq
#define I2CDELAYUSEC         30             // USER CONFIG: GPIO change delay
#define GPIODELAYCYCLES 	((MSP430CLOCKFREQHZ/8000000)*I2CDELAYUSEC)

#define timeout_counter 8000                // I2C Time out ~1ms at least

#define I2CDELAY MSP430_SWI2CMST_delay()    // Macro for GPIO change delay
#define SDA_1 PxDIR&=~(SDA)                 // Set to input [SDA = 1 via pullup]
#define SDA_0 PxDIR|=(SDA)                  // Set to output [SDA = 0]
#define SCL_1 PxDIR&=~(SCL)                 // Set to input [SCL = 1 via pullup]
#define SCL_0 PxDIR|=(SCL)                  // Set to output [SCL = 0]

#define NACK 0
#define ACK  1

#define FALSE_t 0
#define TRUE_t 1

typedef unsigned char tBool;

void MSP430_SWI2CMST_init(void);

void MSP430_SWI2CMST_delay(void);

void MSP430_SWI2CMST_start(void);

void MSP430_SWI2CMST_stop(void);

unsigned char MSP430_SWI2CMST_txByte(unsigned char data);

unsigned char MSP430_SWI2CMST_rxByte(char ack, char* timeout);

tBool MSP430_SWI2CMST_writeBlock(unsigned char SlaveAddress, void* reg,
                                unsigned int numBytes,
                                unsigned char multi, void* I2CTxData);

tBool MSP430_SWI2CMST_readBlock(unsigned char SlaveAddress, void* reg,
                               unsigned int numBytes, void* I2CRxData);
