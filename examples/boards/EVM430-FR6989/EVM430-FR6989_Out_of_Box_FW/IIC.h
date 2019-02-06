/* IIC.h
*/

#ifndef IIC_
#define IIC_

// IIC Setting
#define Master_Add 0x01        // Master Address
#define Slave_Add  0x02        // Slave Address


void IIC_TX(unsigned char );
void IIC_RX(void);
void Set_IIC(void);
void Set_IIC_Timeout(void);


volatile unsigned char Master_RXData[5];

// void Setup_IIC_Slave(void);


#endif
