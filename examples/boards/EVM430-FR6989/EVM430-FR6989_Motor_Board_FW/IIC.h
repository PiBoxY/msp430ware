/* IIC.h
 *
*/

#ifndef IIC_
#define IIC_

// IIC Setting
#define Master_Add 0x01        // Master Address
#define Slave_Add  0x02        // Slave Address

#define Write  0x80
#define Read   0x00

volatile unsigned int rotate_counter;

unsigned char IIC_Data[8];
unsigned char* IIC_Data_pt;


void IIC_TX(void);
void Set_IIC(void);
void IIC(unsigned char Add, unsigned char RW, unsigned char* IIC_Data_pt);



volatile unsigned char RXData;

void Setup_IIC_Slave(void);

#endif
