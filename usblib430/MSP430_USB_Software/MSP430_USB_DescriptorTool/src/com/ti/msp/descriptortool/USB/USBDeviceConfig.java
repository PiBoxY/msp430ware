/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.ti.msp.descriptortool.USB;

import java.io.Serializable;


public class USBDeviceConfig implements Serializable{
    private static final long serialVersionUID = 42L;
    
    public String name = "USB Device";
    public String vID = "0x2047";
    public String pID = "";
    public String vendor = "Texas Instruments";
    public String product = "MSP430-USB Example";
    public boolean useSerial = true;
    public String devRelNum = "0x200";

    /*
     * Getter and Seters
     */
    public String getvID() {
        return vID;
    }

    public void setvID(String vID) {
        this.vID = vID;
    }

    public String getpID() {
        return pID;
    }

    public void setpID(String pID) {
        this.pID = pID;
    }

    public String getVendor() {
        return vendor;
    }

    public void setVendor(String vendor) {
        this.vendor = vendor;
    }

    public String getProduct() {
        return product;
    }

    public void setProduct(String product) {
        this.product = product;
    }

    public boolean isUseSerial() {
        return useSerial;
    }

    public void setUseSerial(boolean useSerial) {
        this.useSerial = useSerial;
    }

    public String getDevRelNum() {
        return devRelNum;
    }

    public void setDevRelNum(String devRelNum) {
        this.devRelNum = devRelNum;
    }

    /**
     * Constructor for USBDevice
     */
    public USBDeviceConfig() {
    }
    
    public String toString(){
        return name;
    }
    
}
//Released_Version_5_20_06_02
