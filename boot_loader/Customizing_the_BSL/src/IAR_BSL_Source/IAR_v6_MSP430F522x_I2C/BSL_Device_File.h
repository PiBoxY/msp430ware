#include "msp430F5229.h"

//Device Specific Definitions
#define MASS_ERASE_DELAY 0x8000
#define INTERRUPT_VECTOR_START 0xFFE0
#define INTERRUPT_VECTOR_END   0xFFFF
#define SECURE_RAM_START 0x2400


#define TX_PORT_SEL P4SEL
#define TX_PORT_DIR P4DIR
#define RX_PORT_SEL P4SEL
#define RX_PORT_DIR P4DIR
#define RXD       BIT1                      // RXD on P4.1
#define TXD       BIT2                      // TXD on P4.2
#define DCO_SPEED 8000000
#define ACLK_SPEED 32768

#define UCZNCTL1  UCB1CTL1
#define UCZNCTL0  UCB1CTL0
#define UCZNI2COA UCB1I2COA
#define UCZNIE    UCB1IE
#define UCZIV     UCB1IV
#define UCZRXBUF  UCB1RXBUF
#define UCZTXBUF  UCB1TXBUF


//Device Specific Definitions for commands and bugs
#define FULL_FLASH_BSL
#define DO_NOT_CHECK_VPE
#define CLEAR_LOCKIO    

// standard command includes
#ifdef RAM_WRITE_ONLY_BSL
#    define SUPPORTS_RX_DATA_BLOCK_FAST
#    define SUPPORTS_RX_PASSWORD
#    define SUPPORTS_LOAD_PC
#endif

#ifdef FULL_FLASH_BSL
#    define FULL_GENERIC_COMMANDS
#    define FLASH_COMMANDS
#endif

#ifdef FULL_FRAM_BSL
#    define FULL_GENERIC_COMMANDS
#endif

#ifdef FULL_GENERIC_COMMANDS
#    define SUPPORTS_RX_DATA_BLOCK_FAST
#    define SUPPORTS_RX_PASSWORD
#    define SUPPORTS_LOAD_PC
#    define SUPPORTS_RX_DATA_BLOCK
#    define SUPPORTS_MASS_ERASE
#    define SUPPORTS_CRC_CHECK
#    define SUPPORTS_TX_DATA_BLOCK
#    define SUPPORTS_TX_BSL_VERSION
#endif

#ifdef FLASH_COMMANDS
#    define SUPPORTS_ERASE_SEGMENT
#    define SUPPORTS_TOGGLE_INFO
#endif
