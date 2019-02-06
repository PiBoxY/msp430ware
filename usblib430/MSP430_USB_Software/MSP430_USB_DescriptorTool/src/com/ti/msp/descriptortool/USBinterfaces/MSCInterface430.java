/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.ti.msp.descriptortool.USBinterfaces;

import java.io.Serializable;
import java.util.HashMap;
import javax.swing.DefaultComboBoxModel;

/**
 *
 * @author a0272979
 */
public class MSCInterface430 {
   
    public final static int NUMBER_OF_LUN_1 =1;
    public final static int NUMBER_OF_LUN_2 =2;
    public final static int NUMBER_OF_LUN_3 =3;
    public final static int NUMBER_OF_LUN_4 =4; 
    
    public String name;
    public int intfNum;
    public String interfString;
    public int numOfLuns;
    public boolean hardDriveMode;
    public boolean cdRomMode;
    public HashMap<Integer,LogicalUnit> lunMap;

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

    public int getNumOfLuns() {
        return numOfLuns;
    }

    public void setNumOfLuns(int numOfLuns) {
        this.numOfLuns = numOfLuns;
    }

    public boolean isHardDriveMode() {
        return hardDriveMode;
    }

    public void setHardDriveMode(boolean hardDriveMode) {
        this.hardDriveMode = hardDriveMode;
    }

    public boolean isCdRomMode() {
        return cdRomMode;
    }

    public void setCdRomMode(boolean cdRomMode) {
        this.cdRomMode = cdRomMode;
    }

    public HashMap<Integer, LogicalUnit> getLunMap() {
        return lunMap;
    }

    public void setLunMap(HashMap<Integer, LogicalUnit> lunMap) {
        this.lunMap = lunMap;
    }
    
    public class LogicalUnit implements Serializable{
        private static final long serialVersionUID = 231L;
        
        public boolean enabled;
        public int lunNumber;
        public boolean removableMedia;
        public String t10Vendor;
        public String t10Product;
        public String t10Revision;
        public boolean useUSBVidinfo;

        public boolean isEnabled() {
            return enabled;
        }

        public void setEnabled(boolean enabled) {
            this.enabled = enabled;
        }
        
        public int getLunNumber() {
            return lunNumber;
        }

        public void setLunNumber(int lunNumber) {
            this.lunNumber = lunNumber;
        }

        public boolean isRemovableMedia() {
            return removableMedia;
        }

        public void setRemovableMedia(boolean removableMedia) {
            this.removableMedia = removableMedia;
        }

        public String getT10Vendor() {
            return t10Vendor;
        }

        public void setT10Vendor(String t10Vendor) {
            this.t10Vendor = t10Vendor;
        }

        public String getT10Product() {
            return t10Product;
        }

        public void setT10Product(String t10Product) {
            this.t10Product = t10Product;
        }

        public String getT10Revision() {
            return t10Revision;
        }

        public void setT10Revision(String t10Revision) {
            this.t10Revision = t10Revision;
        }

        public boolean isUseUSBVidinfo() {
            return useUSBVidinfo;
        }

        public void setUseUSBVidinfo(boolean useUSBVidinfo) {
            this.useUSBVidinfo = useUSBVidinfo;
        }
        
        public LogicalUnit(int lunNumber){
            this.lunNumber = lunNumber;
            if(lunNumber==NUMBER_OF_LUN_1){
                this.enabled = true;
            }else{
                this.enabled = false;
            }
            this.removableMedia = false;
            this.t10Vendor = "TI MSC";
            this.t10Product = "LUN" + lunNumber;
            this.t10Revision = "0";
            this.useUSBVidinfo = false; 
        } 
    }

    /**
     * Constructor for CDCInterface class
     */
    public MSCInterface430() {
        this.intfNum = -1;
        name ="MSC Interface";
        interfString = "MSC Interface";
        numOfLuns = NUMBER_OF_LUN_1;
        hardDriveMode = true;
        cdRomMode = false;
        lunMap = new HashMap<Integer, LogicalUnit>();
        lunMap.put(0, new LogicalUnit(0));
        
    }

    public String toString() {
            return name + "[" + intfNum + "]";
    }
  
}
//Released_Version_5_20_06_02
