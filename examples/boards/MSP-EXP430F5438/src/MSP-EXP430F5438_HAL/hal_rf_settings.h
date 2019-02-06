/*******************************************************************************
 *
 *  hal_rf_settings.h
 *
 *  Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

#ifndef HAL_RF_SETTINGS_H
#define HAL_RF_SETTINGS_H
/* Chipcon
 * Product = CC2500
 * Chip version = E
 * Crystal accuracy = 10 ppm
 * X-tal frequency = 26 MHz
 * RF output power = 0 dBm
 * RX filterbandwidth = 541.666667 kHz
 * Phase = 1
 * Datarate = 249.938965 kbps
 * Modulation = (7) MSK
 * Manchester enable = (0) Manchester disabled
 * RF Frequency = 2432.999908 MHz
 * Channel spacing = 199.951172 kHz
 * Channel number = 0
 * Optimization = Sensitivity
 * Sync mode = (3) 30/32 sync word bits detected
 * Format of RX/TX data = (0) Normal mode, use FIFOs for RX and TX
 * CRC operation = (1) CRC calculation in TX and CRC check in RX enabled
 * Forward Error Correction = (0) FEC disabled
 * Length configuration = (1) Variable length packets, packet length configured by the first
 *received byte after sync word.
 * Packetlength = 255
 * Preamble count = (2)  4 bytes
 * Append status = 1
 * Address check = (0) No address check
 * FIFO autoflush = 0
 * Device address = 0
 * GDO0 signal selection = ( 6) Asserts when sync word has been sent / received, and de-asserts at
 *the end of the packet
 * GDO2 signal selection = (11) Serial Clock */
const HAL_RF_CONFIG myRfConfig = {

    0x0B,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x5D,   // FREQ2     Frequency control word, high byte.
    0x93,   // FREQ1     Frequency control word, middle byte.
    0xB1,   // FREQ0     Frequency control word, low byte.
    0x86,   // MDMCFG4   Modem configuration.
    0x83,   // MDMCFG3   Modem configuration.
    0x83,   // MDMCFG2   Modem configuration.
    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.
    0x00,   // CHANNR    Channel number.
    0x44,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0x56,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end RX configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x16,   // FOCCFG    Frequency Offset Compensation Configuration.
    0x6C,   // BSCFG     Bit synchronization Configuration.
    0x03,   // AGCCTRL2  AGC control.
    0x40,   // AGCCTRL1  AGC control.
    0x91,   // AGCCTRL0  AGC control.
    0xA9,   // FSCAL3    Frequency synthesizer calibration.
    0x0A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x11,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,   // TEST2     Various test settings.
    0x35,   // TEST1     Various test settings.
    0x0B,   // TEST0     Various test settings.
    0x0B,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0D   GDO0 output pin configuration. Refer to SmartRF® Studio User Manual for
            // detailed pseudo register explanation.
    0x04,   // PKTCTRL1  Packet automation control.
    0x05,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0xFF    // PKTLEN    Packet length.

};

const uint8 myPaTable[] = {0xFB};
const uint8 myPaTableLen = 1;

#endif /* HAL_RF_SETTINGS_H */
