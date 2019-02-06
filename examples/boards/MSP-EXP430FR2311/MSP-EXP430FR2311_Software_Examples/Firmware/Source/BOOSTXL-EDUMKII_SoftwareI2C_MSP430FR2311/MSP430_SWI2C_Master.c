#include "MSP430_SWI2C_Master.h"

#include "msp430.h"

void MSP430_SWI2CMST_init(void)
{
  PxOUT &= ~(SCL | SDA);                    // Output settings always LOW
  PxSEL0 &= ~(SCL | SDA);                    // Set GPIO functions
  PxSEL1 &= ~(SCL | SDA);                    // Set GPIO functions
}

void MSP430_SWI2CMST_delay(void)
{
  __delay_cycles(GPIODELAYCYCLES);          // Quick delay
}

void MSP430_SWI2CMST_start(void)
{
  SDA_1;                                    // SDA = 1
  I2CDELAY;                                 // Quick delay
  SCL_1;                                    // SCL = 1
  I2CDELAY;                                 // Quick delay
  SDA_0;                                    // SDA = 0
  I2CDELAY;                                 // Quick delay
  SCL_0;                                    // SCL = 0
  I2CDELAY;                                 // Quick delay
}

void MSP430_SWI2CMST_stop(void)
{
  SDA_0;                                    // SDA = 0
  I2CDELAY;                                 // Quick delay
  SCL_1;                                    // SCL = 1
  I2CDELAY;                                 // Quick delay
  SDA_1;                                    // SDA = 1
  I2CDELAY;                                 // Quick delay
}

unsigned char MSP430_SWI2CMST_txByte(unsigned char data)
{
  unsigned char bits, temp, ack;
  unsigned int time_count = 0;
  
  SCL_0;                                    // SCL = 0
  temp = data;                              // Initialize temp variable
  bits = 0x08;                              // Load I2C bit counter
  while (bits != 0x00)                      // Loop until all bits are shifted
  {
    if (temp & BIT7)                        // Test data bit
      SDA_1;                                // SDA = 1
    else
      SDA_0;                                // SDA = 0
    I2CDELAY;                               // Quick delay
    SCL_1;                                  // SCL = 1
    time_count = 0;
    do
    {
      time_count ++;
    }while (((PxIN & SCL) == 0)&&(time_count != timeout_counter));              // Wait for any SCL clock stretching
    I2CDELAY;                               // Quick delay
    temp = (temp << 1);                     // Shift bits 1 place to the left
    SCL_0;                                  // SCL = 0
    bits = (bits - 1);                      // Loop until 8 bits are sent
  }
  I2CDELAY;
  SDA_1;                                    // SDA = 1
  SCL_1;                                    // SCL = 1
  time_count = 0;
  do
  {
    time_count ++;
  }while (((PxIN & SCL) == 0)&&(time_count != timeout_counter));              // Wait for any SCL clock stretching
  I2CDELAY;                                 // Quick delay
  ack = (PxIN & SDA);                       // Read ACK state from Slave
  SCL_0;                                    // SCL = 0
  if (ack)                                  // Return ACK state to calling app
    return (1);                             // NACK
  else
    return (0);                             // ACK
}

unsigned char MSP430_SWI2CMST_rxByte(char ack, char* timeout)
{
  unsigned char bits, data = 0;
  unsigned int time_count = 0;
  *timeout = 0;

  SDA_1;                                    // SDA = 1
  bits = 0x08;                              // Load I2C bit counter
  while (bits > 0)                          // Loop until all bits are read
  {
    SCL_1;                                  // SCL = 1
    time_count = 0;
    do
    {
      time_count ++;
    }while (((PxIN & SCL) == 0)&&(time_count < timeout_counter));              // Wait for any SCL clock stretching
    if(time_count >= timeout_counter)
        *timeout = 1;
    I2CDELAY;                               // Quick delay
    data = (data << 1);                     // Shift bits 1 place to the left
    if (PxIN & SDA)                         // Check digital input
      data = (data + 1);                    // If input is high, store a '1'
    SCL_0;                                  // SCL = 0
    I2CDELAY;                               // Quick delay
    bits = (bits - 1);                      // Decrement I2C bit counter
  }
  if (ack)                                  // Need to send ACK to Slave?
    SDA_0;                                  // Yes, so pull SDA low
  else
    SDA_1;                                  // No, so keep SDA high
  SCL_1;                                    // SCL = 1
  I2CDELAY;                                 // Equivalent to sending N(ACK)
  SCL_0;                                    // SCL = 0
  SDA_1;                                    // SDA = 1

  return (data);                            // Return 8-bit data byte
}   

tBool MSP430_SWI2CMST_writeBlock(unsigned char SlaveAddress, void* reg,
                                unsigned int numBytes, unsigned char multi,
                                void* I2CTxData)
{
  unsigned int  i;
  unsigned char *temp;
  unsigned char *temp_reg;

  temp = (unsigned char *)I2CTxData;           // Initialize array pointer
  temp_reg = (unsigned char *)reg;           // Initialize array pointer
  MSP430_SWI2CMST_start();                  // Send Start condition
  if(MSP430_SWI2CMST_txByte((SlaveAddress << 1) & ~BIT0)) // [ADDR] + R/W bit = 0 and check ACK state
  {
      MSP430_SWI2CMST_stop();                 // NACK, send STOP condition

      return FALSE_t;
  }
  else
  {

  }

  if(MSP430_SWI2CMST_txByte(*(temp_reg)))     // Send data and check ack state, return 0 is received ack
   {
          MSP430_SWI2CMST_stop();               // NACK, send STOP condition
          return FALSE_t;
   }

  for (i = 0; i < numBytes; i++)
  {
    if(MSP430_SWI2CMST_txByte(*(temp)))     // Send data and check ack state, return 0 is received ack
    {
        MSP430_SWI2CMST_stop();               // NACK, send STOP condition
        return FALSE_t;
    }
    temp++;                                 // Increment pointer to next element
  }
  if (multi == 0)                           // Need STOP condition?
  {
    MSP430_SWI2CMST_stop();                 // Yes, send STOP condition
  }
  I2CDELAY;
  return TRUE_t;
}

tBool MSP430_SWI2CMST_readBlock(unsigned char SlaveAddress, void* reg,
                               unsigned int numBytes, void* I2CRxData)
{
  unsigned int  i;
  unsigned char* temp;
  unsigned char* temp_reg;
  unsigned char check_timeout = 0;

  temp_reg = (unsigned char *)reg;           // Initialize array pointer
  MSP430_SWI2CMST_start();                  // Send Start condition
  if(MSP430_SWI2CMST_txByte((SlaveAddress << 1) & ~BIT0)) // [ADDR] + R/W bit = 0 and check ACK state
  {
      MSP430_SWI2CMST_stop();                 // NACK, send STOP condition

      return FALSE_t;
  }
  else
  {

  }

  if(MSP430_SWI2CMST_txByte(*(temp_reg)))     // Send data and check ack state, return 0 is received ack
 {
		MSP430_SWI2CMST_stop();               // NACK, send STOP condition
		return FALSE_t;
 }

  temp = (unsigned char *)I2CRxData;           // Initialize array pointer
  MSP430_SWI2CMST_start();                  // Send Start condition
  MSP430_SWI2CMST_txByte((SlaveAddress << 1) | BIT0); // [ADDR] + R/W bit = 1
  for (i = 0; i < numBytes; i++)
  {
    if (i == (numBytes - 1))
      *(temp) = MSP430_SWI2CMST_rxByte(NACK, &check_timeout);// Read last 8-bit data with no ACK
    else
      *(temp) = MSP430_SWI2CMST_rxByte(ACK, &check_timeout);// Read 8-bit data & then send ACK
    temp++;                                 // Increment pointer to next element
    if(check_timeout)
    {
        MSP430_SWI2CMST_stop();                   // Send Stop condition
        return FALSE_t;
    }
  }
  MSP430_SWI2CMST_stop();                   // Send Stop condition
  I2CDELAY;
  return TRUE_t;
}
