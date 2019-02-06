/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.ti.msp.descriptortool.USBinterfaces;

public class PHDCInterface430 {
    
    public String name = "PHDC Interface";
    public int interfNum = -1;
    public String interfString = "PHDC Interface";
    
    // Getter and Setters
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getInterfNum() {
        return interfNum;
    }

    public void setInterfNum(int interfNum) {
        this.interfNum = interfNum;
    }

    public String getInterfString() {
        return interfString;
    }

    public void setInterfString(String interfString) {
        this.interfString = interfString;
    }
    
    /**
     * Constructor for PHDCInterface430 class
     */
    public PHDCInterface430() {
    }
 
    public String toString(){
        return name + "[" + interfNum + "]";
    }
    
}
//Released_Version_5_20_06_02
