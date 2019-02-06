//*****************************************************************************
//
// loader.h
//
// This module contains definitions for the MSP430x09x loader code APIs stored
// in ROM for both assembly and ANSI-C
//
//  Caleb Overbay
//  Texas Instruments Inc.
//  April 2018
//*****************************************************************************

#if (defined(__ASM_HEADER__) || defined(__A430__))

// Assembly style definitions

#define Loader              0x0F880
#define ApiCall             0x0F882
#define SWID                0x0F884
#define LedOn               0x0F886
#define LedOff              0x0F888
#define XOVL                0x0F88A
#define LOVL                0x0F88C
#define COVL                0x0F88E
#define ROVL                0x0F890
#define SpiReadByte         0x0F892
#define SpiReadWord         0x0F894
#define SpiReadStream       0x0F896
#define SpiStreamEnd        0x0F898
#define SpiWriteByte        0x0F89A
#define SpiReadWrite        0x0F89C
#define CpyToSpi            0x0F89E
#define CpyTo430            0x0F8A0
#define SpiGenerateImage    0x0F8A2
#define CRC                 0x0F8A4

/* API Software Registers  */
#define CurOvlSpiH          0x01C50               // current Overlay SPIadr high word
#define CurOvlSpiL          0x01C52               // current Overlay SPIadr low word
#define CurOvlAdrH          0x01C54               // current Overlay prog address high
#define CurOvlAdrL          0x01C56               // current Overlay prog address low
#define Status_Reg          0x01C58               // status REG
#define LedOffPtr           0x01C5A               // socket for cust LedOff
#define LedOnPtr            0x01C5C               // socket for cust LedOn
#define UnexpCnt            0x01C5E               // count of unexpected interrupts

/* MSP430 Secondary Interrupt vectors  */
#define RESERVED_00_IV2     0x01C60               // Reserved vector
#define RESERVED_01_IV2     0x01C62               // Reserved vector
#define RESERVED_02_IV2     0x01C64               // Reserved vector
#define RESERVED_03_IV2     0x01C66               // Reserved vector
#define RESERVED_04_IV2     0x01C68               // Reserved vector
#define PORT2_IV2           0x01C6A               // Port2 secondary vector
#define TIMER0_A1_IV2       0x01C6C               // TA0CCR1 CCIFG1 secondary vector
#define TIMER0_A0_IV2       0x01C6E               // TA0CCR0 CCIFG0 secondary vector
#define PORT1_IV2           0x01C70               // Port1 secondary vector
#define APOOL_IV2           0x01C72               // APOOL secondary vector
#define WDT_IV2             0x01C74               // WDT secondary vector
#define TIMER1_A1_IV2       0x01C76               // TA1CCR1 CCIFG1 secondary vector
#define TIMER1_A0_IV2       0x01C78               // TA1CCR0 CCIFG0 secondary vector
#define UNMI_IV2            0x01C7A               // UNMI secondary vector
#define SYSNMI_IV2          0x01C7C               // SYSNMI secondary vector
#define RST_IV2             0x01C7E               // Reset secondary vector

#else

// ANSI-C style definitions

//********************************************************************
// Name        :Loader                                               *
// Function    :Loads image from SPI to MSP430, does not return      *
//********************************************************************
typedef void loader_t (void);
#define Loader(void) ((loader_t *)(*(int *)0x0F880))(void)

//*********************************************************************
//* Name        :ApiCall                                              *
//* Function    :Represents a low convention API function             *
//* Arguments   :R12.A pointer to structure of parameters             *
//* Structure   :ushort Fnum     ;function number                     *
//*             :ushort R15Val   ;parameter to/from R15 of target fkt *
//*             :ushort R14Val   ;parameter to/from R14 of target fkt *
//*             :ushort R13Val   ;parameter to/from R13 of target fkt *
//*             :ushort R12Val   ;parameter to/from R12 of target fkt *
//*             :ushort R11Val   ;parameter to/from R11 of target fkt *
//*             :ushort R10Val   ;parameter to/from R10 of target fkt *
//* Returns     :-                                                    *
//* Stack       :10 words                                             *
//* Calls       :addressed function                                   *
//*********************************************************************
typedef void apicall_t (void * args);
#define ApiCall(args) ((apicall_t *)(*(int *)0x0F882))(args)

//********************************************************************
// Name        :SWID                                                 *
// Function    :calculates own code integrity and returns            *
//              software ID number                                   *
// Arguments   :-                                                    *
// Returns     :2843:AALLh                                           *
//              IEEE convention                                      *
//              28: IEEE-TI-ID   43: MSP430-SW-ID                    *
//              LL: 'Lo92 loader version AA: API version             *
// Stack       :-                                                    *
//********************************************************************
typedef unsigned long swid_t (void);
#define SWID(void) ((swid_t *)(*(int *)0x0F884))(void)

typedef struct
{
    unsigned short SWIDNum; // Function number (here #0)
    unsigned short IdLow;   // Reserved for returned low byte
    unsigned short IdHigh;  // Reserved for returned high byte
}swid_api_args;

//********************************************************************
// Name        :LedOn                                                *
// Function    :turns 250kHz clock P1.2 on for LED and SPI power     *
// Arguments   :-                                                    *
// Returns     :-                                                    *
// Stack       :-                                                    *
//********************************************************************
typedef void ledon_t (void);
#define LedOn(void) ((ledon_t *)(*(int *)0x0F886))(void)

typedef struct
{
    unsigned short LedOnNum;  // Function number (here #1)
}ledon_api_args;

//*********************************************************************
//* Name        :LedOff                                               *
//* Function    :turns 250kHz clock P1.2 off                          *
//* Arguments   :-                                                    *
//* Returns     :-                                                    *
//* Stack       :-                                                    *
//*********************************************************************
typedef void ledoff_t (void);
#define LedOff(void) ((ledoff_t *)(*(int *)0x0F888))(void)

typedef struct
{
    unsigned short LedOffNum;  // Function number (here #2)
}ledoff_api_args;

//********************************************************************
// Name        :XOVL                                                 *
// Function    :executes overlay as defined with parameters          *
//              blink LED 3X & 1Hz when error and enter LPM4         *
// Arguments   :R12.W Spi source address high bits                   *
//              R13.W Spi source address low bits                    *
//              R14.W alternate 430 load address when !=0            *
//              R15.W alternate start address when !=0               *
// Returns     :-                                                    *
// Stack       :-                                                    *
//********************************************************************
typedef void xovl_t (unsigned short SpiAdrHigh, unsigned short SpiAdrLow, unsigned short ladr, unsigned short sadr);
#define XOVL(SpiAdrHigh, SpiAdrLow, ladr, sadr) ((xovl_t *)(*(int *)0x0F88A))(SpiAdrHigh, SpiAdrLow, ladr, sadr)

typedef struct
{
    unsigned short XOVLNum;    // Function number (here #3)
    unsigned short SpiAdrHigh; // SPI address high word
    unsigned short SpiAdrLow;  // SPI address low word
    unsigned short ladr;       // Alternate 430 load address when != 0
    unsigned short sadr;       // Alternate 430 start address when != 0
}xovl_api_args;

//********************************************************************
// Name        :LOVL                                                 *
// Function    :Loads an overlay into memory and returns to calling  *
//              function without executing it                        *
//              blink LED 3X & 1Hz when error and enter LPM4         *
// Arguments   :R12.W Spi source address high word                   *
//              R13.W Spi source address low word                    *
//              R14.W MSP430 target address word !0 = override       *
// Returns     :-                                                    *
// Stack       :-                                                    *
//********************************************************************
typedef void lovl_t (unsigned short SpiAdrHigh, unsigned short SpiAdrLow, unsigned short ladr);
#define LOVL(SpiAdrHigh, SpiAdrLow, ladr) ((lovl_t *)(*(int *)0x0F88C))(SpiAdrHigh, SpiAdrLow, ladr)

typedef struct
{
    unsigned short LOVLNum;     // Function number (here #4)
    unsigned short SpiAdrHigh;  // SPI address high word
    unsigned short SpiAdrLow;   // SPI address low word
    unsigned short ladr;        // Alternate 430 load address when != 0
}lovl_api_args;

//********************************************************************
// Name        :COVL                                                 *
// Function    :Loads an overlay into memory, executes it as a call  *
//              blink LED 3X & 1Hz when error and LPM4               *
// Arguments   :R12.W Spi source address high word                   *
//              R13.W Spi source address low word                    *
//              R14.W MSP430 target address word !0 = override       *
// Returns     :-                                                    *
// Stack       :-                                                    *
//********************************************************************
typedef void covl_t (unsigned short SpiAdrHigh, unsigned short SpiAdrLow, unsigned short ladr);
#define COVL(SpiAdrHigh, SpiAdrLow, ladr) ((covl_t *)(*(int *)0x0F88E))(SpiAdrHigh, SpiAdrLow, ladr)

typedef struct
{
    unsigned short COVLNum;    // Function number (here #5)
    unsigned short SpiAdrHigh; // SPI address high word
    unsigned short SpiAdrLow;  // SPI address low word
    unsigned short ladr;       // Alternate 430 load address when != 0
}covl_api_args;

//********************************************************************
// Name        :ROVL                                                 *
// Function    :Returns from current overlay, to previous            *
//              blink LED 3X & 1Hz when error and LPM4               *
// Arguments   :-                                                    *
// Returns     :-                                                    *
// Stack       :-                                                    *
//********************************************************************
typedef void rovl_t (void);
#define ROVL(void) ((rovl_t *)(*(int *)0x0F890))(void)

typedef struct
{
    unsigned short ROVLNum; // Function number (here #6)
}rovl_api_args;

//********************************************************************
// Name        :SpiReadByte                                          *
// Function    :reads byte from SPI memory...,                       *
// Arguments   :R12.W Spi source address high word                   *
//              R13.W Spi source address low word                    *
// Returns     :R12.B  data read                                     *
// Stack       :-                                                    *
//********************************************************************
typedef unsigned char spireadbyte_t (unsigned short SpiAdrHigh, unsigned short SpiAdrLow);
#define SpiReadByte(SpiAdrHigh, SpiAdrLow) ((spireadbyte_t *)(*(int *)0x0F892))(SpiAdrHigh, SpiAdrLow)

typedef struct
{
    unsigned short SpiReadByteNum;   // Function number (here #7)

    union
    {
        unsigned short SpiAdrHigh;   // SPI address high word
        unsigned char SpiRxData;     // Reserved for returned byte
    };

    unsigned short SpiAdrLow;        // SPI address low word
}spireadbyte_api_args;

//********************************************************************
// Name        :SpiReadWord                                          *
// Function    :reads word from SPI memory...,                       *
// Arguments   :R12.W Spi source address high word                   *
//              R13.W Spi source address low word                    *
// Returns     :R12.W  data read                                     *
// Stack       :1 word                                               *
//********************************************************************
typedef unsigned short spireadword_t (unsigned short SpiAdrHigh, unsigned short SpiAdrLow);
#define SpiReadWord(SpiAdrHigh, SpiAdrLow) ((spireadword_t *)(*(int *)0x0F894))(SpiAdrHigh, SpiAdrLow)

typedef struct
{
    unsigned short SpiReadWordNum;  // Function number (here #8)

    union
    {
        unsigned short SpiAdrHigh;  // SPI address high word
        unsigned short SpiRxData;   // Reserved for returned word
    };

    unsigned short SpiAdrLow;       // SPI address low word
}spireadword_api_args;

//********************************************************************
// Name        :SpiReadStream                                        *
// Function    :reads first element of stream at "SpiAdr"            *
//              continue with SpiReadWrite and terminate             *
//              with SpiStreamEnd                                    *
// Arguments   :R12.W Spi source address high word                   *
//              R13.W Spi source address low word                    *
// Returns     :R12.B  data read                                     *
// Stack       :2 Words                                              *
//********************************************************************
typedef unsigned char spireadstream_t (unsigned short SpiAdrHigh, unsigned short SpiAdrLow);
#define SpiReadStream(SpiAdrHigh, SpiAdrLow) ((spireadstream_t *)(*(int *)0x0F896))(SpiAdrHigh, SpiAdrLow)

typedef struct
{
    unsigned short SpiReadStreamNum;  // Function number (here #9)

    union
    {
        unsigned short SpiAdrHigh;    // SPI address high word
        unsigned char SpiRxData;      // Reserved for returned byte
    };

    unsigned short SpiAdrLow;         // SPI address low word
}spireadstream_api_args;

//********************************************************************
// Name        :SpiStreamEnd                                         *
// Function    :terminates an SpiReadStream operation                *
// Arguments   :-                                                    *
// Returns     :-                                                    *
// Stack       :-                                                    *
//********************************************************************
typedef void spistreamend_t (void);
#define SpiStreamEnd(void) ((spistreamend_t *)(*(int *)0x0F898))(void)

typedef struct
{
    unsigned short SpiStreamEndNum;  // Function number (here #10)
}spistreamend_api_args;

//********************************************************************
// Name        :SpiWriteByte                                         *
// Function    :writes single "data" byte to memory location "SpiAdr"*
// Arguments   :R12.W Spi source address high word                   *
//              R13.W Spi source address low word                    *
//             :R14.B  data to be written                            *
// Returns     :-                                                    *
// Stack       :3 Words                                              *
//********************************************************************
typedef void spiwritebyte_t (unsigned short SpiAdrHigh, unsigned short SpiAdrLow, unsigned char data);
#define SpiWriteByte(SpiAdrHigh, SpiAdrLow, data) ((spiwritebyte_t *)(*(int *)0x0F89A))(SpiAdrHigh, SpiAdrLow, data)

typedef struct
{
    unsigned short SpiWriteByteNum;  // Function number (here #11)
    unsigned short SpiAdrHigh;       // SPI address high word
    unsigned short SpiAdrLow;        // SPI address low word
    unsigned char SpiData;           // Byte to write to SPI address
}spiwritebyte_api_args;

//********************************************************************
// Name        :SpiReadWrite                                         *
// Function    :send data via SPI to EEPROM/FLASH while receiving    *
//              bytewide data from EEPROM/FLASH (without CS change)  *
// Arguments   :R12.B transmit data as unsigned char                 *
// Returns     :R12.B receive data as unsigned char                  *
// Stack       :1 word used                                          *
//********************************************************************
typedef unsigned char spireadwrite_t (unsigned char TxData);
#define SpiReadWrite(TxData) ((spireadwrite_t *)(*(int *)0x0F89C))(TxData)

typedef struct
{
    unsigned short SpiReadWriteNum;   // Function number (here #12)
    union
    {
        unsigned char SpiTxData;      // Byte to send to SPI device
        unsigned char SpiRxData;      // Reserved for byte received from SPI device
    };
}spireadwrite_api_args;

//********************************************************************
// Name        :CpyToSpi                                             *
// Function    :copies data block from 430 memory to SPI device      *
// Arguments   :R12.W MSP430 source address                          *
//             :R13.W Spi source address high word                   *
//              R14.W Spi source address low word                    *
//             :R15.W length in bytes                                *
// Returns     :-                                                    *
// Stack       :3 words                                              *
//********************************************************************
typedef void cpytospi_t (unsigned short SrcAdr, unsigned short SpiAdrHigh, unsigned short SpiAdrLow, unsigned short Count);
#define CpyToSpi(SrcAdr, SpiAdrHigh, SpiAdrLow, Count)((cpytospi_t *)(*(int *)0x0F89E))(SrcAdr, SpiAdrHigh, SpiAdrLow, Count)

typedef struct
{
    unsigned short CpyToSpiNum;  // Function number (here #13)
    unsigned short SrcAdr;       // Source address for MSP430
    unsigned short SpiAdrHigh;   // SPI address high word
    unsigned short SpiAdrLow;    // SPI address low word
    unsigned char Count;         // Length in bytes
}cpytospi_api_args;

//********************************************************************
// Name        :CpyTo430                                             *
// Function    :copies data block from SPI to 430 memory             *
// Arguments   :R12.W Spi source address high word                   *
//              R13.W Spi source address low word                    *
//              R14.W MSP430 target address word                     *
//              R15.W length in bytes                                *
// Returns     :-                                                    *
// Stack       :-                                                    *
//********************************************************************
typedef void cpyto430_t (unsigned short SpiAdrHigh, unsigned short SpiAdrLow, unsigned short DstAdr, unsigned short Count);
#define CpyTo430(SpiAdrHigh, SpiAdrLow, DstAdr, Count) ((cpyto430_t *)(*(int *)0x0F8A0))(SpiAdrHigh, SpiAdrLow, DstAdr, Count)

typedef struct
{
    unsigned short CpyTo430Num;  // Function number (here #14)
    unsigned short SpiAdrHigh;   // SPI address high word
    unsigned short SpiAdrLow;    // SPI address low word
    unsigned short DstAdr;       // Destination address for MSP430
    unsigned char Count;         // Length in bytes
}cpyto430_api_args;

//********************************************************************
// Name        :SpiGenerateImage                                     *
// Function    :generates a bootable EEPROM/FLASH image via SPI..    *
//              the routine stops in LPM4                            *
// Arguments   :R12.W program address in MSP430 space (1C00h)        *
//             :R13.W Spi source address high word                   *
//              R14.W Spi source address low word                    *
//              R15.W length of data in bytes (2...1984)             *
//              R11:W start address of code (1C00h...2300h)          *
// Returns     :- => LPM4                                            *
// Destroys    :R8,R9,R10,R11 (meant as utility function)            *
// Stack       :1 Words                                              *
//********************************************************************
typedef void spigenerateimage_t (unsigned short SrcAdr, unsigned short SpiAdrHigh, unsigned short SpiAdrLow, unsigned short Count, unsigned short sadr);
#define SpiGenerateImage(SrcAdr, SpiAdrHigh, SpiAdrLow, Count, sadr) ((spigenerateimage_t *)(*(int *)0x0F8A2))(SrcAdr, SpiAdrHigh, SpiAdrLow, Count, sadr)

typedef struct
{
    unsigned short SpiGenerateImageNum; // Function number (here #15)
    unsigned short SrcAdr;              // Program address in MSP430 space
    unsigned short SpiAdrHigh;          // SPI address high word
    unsigned short SpiAdrLow;           // SPI address low word
    unsigned char Count;                // Length in bytes
    unsigned short StartAdr;            // Start address of code
}spigenerateimage_api_args;

//********************************************************************
// Name        :CRC                                                  *
// Function    :calculates a CRC over the given parameters           *
//* Arguments  :R12.W initial seed for CRC                           *
//             :R13.W CRC start addr                                 *
//             :R14.W CRC end addr                                   *
// Returns     :R12.W CRC value                                      *
// Stack       :-                                                    *
//********************************************************************
typedef unsigned short crc_t (unsigned short Seed, unsigned short StartAdr, unsigned short EndAdr);
#define CRC(Seed, StartAdr, EndAdr) ((crc_t *)(*(int *)0x0F8A4))(Seed, StartAdr, EndAdr)

typedef struct
{
    unsigned short CRCNum;      // Function number (here #16)

    union
    {
        unsigned short Seed;
        unsigned short CRCVal;
    };

    unsigned short StartAdr;
    unsigned short EndAdr;
}crc_api_args;

/* API Software Registers  */
#define CurOvlSpiH          *(int *)0x01C50               // current Overlay SPIadr high word
#define CurOvlSpiL          *(int *)0x01C52               // current Overlay SPIadr low word
#define CurOvlAdrH          *(int *)0x01C54               // current Overlay prog address high
#define CurOvlAdrL          *(int *)0x01C56               // current Overlay prog address low
#define Status_Reg          *(int *)0x01C58               // status REG
#define LedOffPtr           *(int *)0x01C5A               // socket for cust LedOff
#define LedOnPtr            *(int *)0x01C5C               // socket for cust LedOn
#define UnexpCnt            *(int *)0x01C5E               // count of unexpected interrupts

/* MSP430 Secondary Interrupt vectors  */
#define RESERVED_00_IV2     *(int *)0x01C60               // Reserved vector
#define RESERVED_01_IV2     *(int *)0x01C62               // Reserved vector
#define RESERVED_02_IV2     *(int *)0x01C64               // Reserved vector
#define RESERVED_03_IV2     *(int *)0x01C66               // Reserved vector
#define RESERVED_04_IV2     *(int *)0x01C68               // Reserved vector
#define PORT2_IV2           *(int *)0x01C6A               // Port2 secondary vector
#define TIMER0_A1_IV2       *(int *)0x01C6C               // TA0CCR1 CCIFG1 secondary vector
#define TIMER0_A0_IV2       *(int *)0x01C6E               // TA0CCR0 CCIFG0 secondary vector
#define PORT1_IV2           *(int *)0x01C70               // Port1 secondary vector
#define APOOL_IV2           *(int *)0x01C72               // APOOL secondary vector
#define WDT_IV2             *(int *)0x01C74               // WDT secondary vector
#define TIMER1_A1_IV2       *(int *)0x01C76               // TA1CCR1 CCIFG1 secondary vector
#define TIMER1_A0_IV2       *(int *)0x01C78               // TA1CCR0 CCIFG0 secondary vector
#define UNMI_IV2            *(int *)0x01C7A               // UNMI secondary vector
#define SYSNMI_IV2          *(int *)0x01C7C               // SYSNMI secondary vector
#define RST_IV2             *(int *)0x01C7E               // Reset secondary vector

#endif
