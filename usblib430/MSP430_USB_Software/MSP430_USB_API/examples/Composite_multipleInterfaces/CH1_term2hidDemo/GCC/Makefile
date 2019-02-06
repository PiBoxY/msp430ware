DEVICE = MSP430F5529
EXAMPLE_NAME = CH1_term2hidDemo
USB_BASE = ./../../../..
REDHAT_GCC = 
GCC_BIN = $(REDHAT_GCC)/bin
CC = $(GCC_BIN)/msp430-elf-gcc
GDB = $(GCC_BIN)/msp430-elf-gdb
SUPPORT_FILE_DIRECTORY = $(REDHAT_GCC)/include
DRIVERLIB_DIR = $(USB_BASE)/driverlib/MSP430F5xx_6xx
MSP430_FILE = $(USB_BASE)/USB_API/msp430USB.ld
PROJECT_ROOT = $(USB_BASE)/examples/Composite_multipleInterfaces/$(EXAMPLE_NAME)
USB_CONFIG = $(PROJECT_ROOT)/USB_config
USB_API = $(USB_BASE)/USB_API


CFLAGS = -I $(SUPPORT_FILE_DIRECTORY) -I $(USB_BASE) -I $(DRIVERLIB_DIR) -I $(PROJECT_ROOT) -I $(USB_CONFIG) -I $(USB_API) -D__$(DEVICE)__ -DDEPRECATED -mmcu=msp430f5529 -O3 -Os -g -fdata-sections -w
LFLAGS = -T $(MSP430_FILE) -T $(SUPPORT_FILE_DIRECTORY)/msp430f5529.ld -Wl,--gc-sections

SRC_FILES = \
../system_pre_init.c \
../hal.c \
../main.c \
../USB_config/UsbIsr.c \
../USB_config/descriptors.c \
../USB_app/usbConstructs.c \
../USB_app/usbEventHandling.c \
../../../../USB_API/USB_PHDC_API/UsbPHDC.c \
../../../../USB_API/USB_HID_API/UsbHid.c \
../../../../USB_API/USB_HID_API/UsbHidReq.c \
../../../../USB_API/USB_CDC_API/UsbCdc.c \
../../../../USB_API/USB_Common/usb.c \
../../../../USB_API/USB_Common/usbdma.c \
../../../../USB_API/USB_MSC_API/UsbMscScsi.c \
../../../../USB_API/USB_MSC_API/UsbMscStateMachine.c \
../../../../USB_API/USB_MSC_API/UsbMscReq.c \
../../../../driverlib/MSP430F5xx_6xx/dac12_a.c \
../../../../driverlib/MSP430F5xx_6xx/usci_b_spi.c \
../../../../driverlib/MSP430F5xx_6xx/usci_b_i2c.c \
../../../../driverlib/MSP430F5xx_6xx/ctsd16.c \
../../../../driverlib/MSP430F5xx_6xx/rtc_a.c \
../../../../driverlib/MSP430F5xx_6xx/tec.c \
../../../../driverlib/MSP430F5xx_6xx/ucs.c \
../../../../driverlib/MSP430F5xx_6xx/ldopwr.c \
../../../../driverlib/MSP430F5xx_6xx/battbak.c \
../../../../driverlib/MSP430F5xx_6xx/sd24_b.c \
../../../../driverlib/MSP430F5xx_6xx/gpio.c \
../../../../driverlib/MSP430F5xx_6xx/dma.c \
../../../../driverlib/MSP430F5xx_6xx/wdt_a.c \
../../../../driverlib/MSP430F5xx_6xx/adc10_a.c \
../../../../driverlib/MSP430F5xx_6xx/pmm.c \
../../../../driverlib/MSP430F5xx_6xx/usci_a_uart.c \
../../../../driverlib/MSP430F5xx_6xx/comp_b.c \
../../../../driverlib/MSP430F5xx_6xx/sysctl.c \
../../../../driverlib/MSP430F5xx_6xx/mpy32.c \
../../../../driverlib/MSP430F5xx_6xx/flashctl.c \
../../../../driverlib/MSP430F5xx_6xx/oa.c \
../../../../driverlib/MSP430F5xx_6xx/lcd_b.c \
../../../../driverlib/MSP430F5xx_6xx/tlv.c \
../../../../driverlib/MSP430F5xx_6xx/eusci_a_uart.c \
../../../../driverlib/MSP430F5xx_6xx/aes.c \
../../../../driverlib/MSP430F5xx_6xx/timer_a.c \
../../../../driverlib/MSP430F5xx_6xx/rtc_c.c \
../../../../driverlib/MSP430F5xx_6xx/timer_b.c \
../../../../driverlib/MSP430F5xx_6xx/ram.c \
../../../../driverlib/MSP430F5xx_6xx/eusci_b_i2c.c \
../../../../driverlib/MSP430F5xx_6xx/ref.c \
../../../../driverlib/MSP430F5xx_6xx/timer_d.c \
../../../../driverlib/MSP430F5xx_6xx/pmap.c \
../../../../driverlib/MSP430F5xx_6xx/eusci_b_spi.c \
../../../../driverlib/MSP430F5xx_6xx/adc12_a.c \
../../../../driverlib/MSP430F5xx_6xx/rtc_b.c \
../../../../driverlib/MSP430F5xx_6xx/eusci_a_spi.c \
../../../../driverlib/MSP430F5xx_6xx/usci_a_spi.c \
../../../../driverlib/MSP430F5xx_6xx/sfr.c \
../../../../driverlib/MSP430F5xx_6xx/crc.c \


CH1_term2hidDemo: $(SRC_FILES)
	$(CC) $(CFLAGS) $(LFLAGS) $? -o $(EXAMPLE_NAME).out

debug: CH1_term2hidDemo
	$(GDB) $(EXAMPLE_NAME).out