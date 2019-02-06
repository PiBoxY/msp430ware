/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.ti.msp.descriptortool.mspConfig;

import com.ti.msp.descriptortool.*;
import java.io.Serializable;

/**
 *
 * @author a0272979
 */
public class MSP430Config implements Serializable{
    
    final public int XT2_1_5MHZ_MHZ =   0;
    final public int XT2_1_6MHZ_MHZ =   1;
    final public int XT2_1_7778_MHZ =   2;
    final public int XT2_1_8432_MHZ =   3;
    final public int XT2_1_8461_MHZ =   4;
    final public int XT2_1_92_MHZ =     5;
    final public int XT2_2_0_MHZ =      6;
    final public int XT2_2_4_MHZ =      7;
    final public int XT2_2_66667_MHZ =  8;
    final public int XT2_3_0_MHZ =      9;
    final public int XT2_3_2_MHZ =     10;
    final public int XT2_3_5556_MHZ =  11;
    final public int XT2_3_579546_MHZ =12;
    final public int XT2_3_84_MHZ =    13;
    final public int XT2_4_0_MHZ =     14;
    final public int XT2_4_1739_MHZ =  15;
    final public int XT2_4_1943_MHZ =  16;
    final public int XT2_4_332_MHZ =   17;
    final public int XT2_4_3636_MHZ =  18;
    final public int XT2_4_5_MHZ =     19;
    final public int XT2_4_8_MHZ =     20;
    final public int XT2_5_3_MHZ =     21;
    final public int XT2_5_76_MHZ =    22;
    final public int XT2_6_0_MHZ =     23;
    final public int XT2_6_4_MHZ =     24;
    final public int XT2_7_2_MHZ =     25;
    final public int XT2_7_68_MHZ =    26;
    final public int XT2_8_0_MHZ =     27;
    final public int XT2_9_0_MHZ =     28;
    final public int XT2_9_6_MHZ =     29;
    final public int XT2_10_66_MHZ =   30;
    final public int XT2_12_0_MHZ =    31;
    final public int XT2_12_8_MHZ =    32;
    final public int XT2_14_4_MHZ =    33;
    final public int XT2_16_0_MHZ =    34;
    final public int XT2_16_9344_MHZ = 35;
    final public int XT2_16_94118_MHZ =36;
    final public int XT2_18_0_MHZ =    37;
    final public int XT2_19_2_MHZ =    39;
    final public int XT2_24_0_MHZ =    40;
    final public int XT2_25_6_MHZ =    41;
    final public int XT2_26_0_MHZ =    42;
    final public int XT2_32_MHZ   =    43;
    
    
    
    final public int DMA_CHANNEL_NA =   0;
    final public int DMA_CHANNEL_0 =   1;
    final public int DMA_CHANNEL_1 =   2;
    final public int DMA_CHANNEL_2 =   3;
    final public int DMA_CHANNEL_3 =   4;
    final public int DMA_CHANNEL_4 =   5;
    final public int DMA_CHANNEL_5 =   6;
    final public int DMA_CHANNEL_6 =   7;
    final public int DMA_CHANNEL_7 =   8;
    final public int MSPCONFIG_MSP430F5xx_F6xx = 0;
    final public int MSPCONFIG_MSP432 = 1;   
    
    public String name = "MSP";
    public boolean disCrysOnSus = true;
    public int crystOScSel= XT2_4_0_MHZ;
    public int damChannel = DMA_CHANNEL_0 ;
    public boolean sysBIOS = false;
    public boolean driveVBUSExt = false;
    public boolean bypass = false;
  
    
    public boolean isBypass() {
        return bypass;
    }
    /*
     * Getter and Seters
     */
    public void setBypass(boolean bypass) {
        this.bypass = bypass;
    }

    public boolean isDisCrysOnSus() {
        return disCrysOnSus;
    }

    public void setDisCrysOnSus(boolean disCrysOnSus) {
        this.disCrysOnSus = disCrysOnSus;
    }

    public int getCrystOScSel() {
        return crystOScSel;
    }

    public void setCrystOScSel(int crystOScSel) {
        this.crystOScSel = crystOScSel;
    }

    public int getDamChannel() {
        return damChannel;
    }

    public void setDamChannel(int damChannel) {
        this.damChannel = damChannel;
    }

    public boolean isSysBIOS() {
        return sysBIOS;
    }

    public void setSysBIOS(boolean sysBIOS) {
        this.sysBIOS = sysBIOS;
    }

    public boolean isDriveVBUSExt() {
        return driveVBUSExt;
    }

    public void setDriveVBUSExt(boolean driveVBUSExt) {
        this.driveVBUSExt = driveVBUSExt;
    }
    
    /**
     * Constructor for MSP430Config class
     */
    public MSP430Config() {
    }
  
    public String toString(){
        return name;
    }
}
//Released_Version_5_20_06_02
