/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.ti.msp.descriptortool.USBinterfaces;

/**
 *
 * @author a0272979
 */
public class CDCInterface430 {
    
    public String name = "CDC Interface";
    public int intfNum = -1;
    public String interfString = "Virtual COM Port (CDC)";

    
    // Getter and Setters
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getIntfNum() {
        return intfNum;
    }

    public void setIntfNum(int intfNum) {
        this.intfNum = intfNum;
    }

    public String getInterfString() {
        return interfString;
    }

    public void setInterfString(String interfString) {
        this.interfString = interfString;
    }

    /**
     * Constructor for CDCInterface430 class
     */
    public CDCInterface430() {
    }

    public String toString() {
            return name + "[" + intfNum + "]";
    }
  
}
//Released_Version_5_20_06_02
