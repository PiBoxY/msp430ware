/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.ti.msp.descriptortool.USBinterfaces;

public class HIDInterface430 {
    
    public static int HID_REPORT_DATAPIPE = 0;
    public static int HID_REPORT_KEYBOARD = 1;
    public static int HID_REPORT_MOUSE    = 2;
    public static int HID_REPORT_CUSTOM   = 3;
    
    final private String eol = System.getProperty("line.separator");
    final private String softTab = "    ";
    public String customReportText = 
            "0x06, 0x00, 0xff,                  // Usage Page (Vendor Defined)\n" +
            "0x09, 0x01,                        // Usage Page (Vendor Defined)\n" +
            "0xa1, 0x01,                        // COLLECTION (Application)\n" +
            "0x85, 0x3f,                        // Report ID (Vendor Defined)\n" +
            "0x95, MAX_PACKET_SIZE-1,           // Report Count\n" +
            "0x75, 0x08,                        // Report Size\n" +
            "0x25, 0x01,                        // Usage Maximum\n" +
            "0x15, 0x01,                        // Usage Minimum\n" +
            "0x09, 0x01,                        // Vendor Usage\n" +
            "0x81, 0x02,                        // Input (Data,Var,Abs)\n" +
            "0x85, 0x3f,                        // Report ID (Vendor Defined)\n" +
            "0x95, MAX_PACKET_SIZE-1,           // Report Count\n" +
            "0x75, 0x08,                        // Report Size\n" +
            "0x25, 0x01,                        // Usage Maximum\n" +
            "0x15, 0x01,                        // Usage Minimum\n" +
            "0x09, 0x01,                        // Vendor Usage\n" +
            "0x91 ,0x02,                        // Output (Data,Var,Abs)\n" +
            "0xc0                                // end Application Collection";
    final private String keyboardReportString = 
            softTab + "0x05, 0x01,                        // Usage Page (Generic Desktop)" + eol +                      
            softTab + "0x09, 0x06,                        // Usage (Keyboard)" + eol +
            softTab + "0xA1, 0x01,                        // Collection (Application)" + eol +
            softTab + "0x05, 0x07,                        // Usage Page (Key Codes)" + eol +
            softTab + "0x19, 0xE0,                        // Usage Minimum (224)" + eol +
            softTab + "0x29, 0xE7,                        // Usage Maximum (231)" + eol +
            softTab + "0x15, 0x00,                        // Logical Minimum (0)" + eol +
            softTab + "0x25, 0x01,                        // Logical Maximum (1)" + eol +
            softTab + "0x75, 0x01,                        // Report Size (1)" + eol +
            softTab + "0x95, 0x08,                        // Report Count (8)" + eol +
            softTab + "0x81, 0x02,                        // Input (Data, Variable, Absolute) -- Modifier byte" + eol +
            softTab + "0x95, 0x01,                        // Report Count (1)" + eol +
            softTab + "0x75, 0x08,                        // Report Size (8)" + eol +
            softTab + "0x81, 0x03,                        // (81 01) Input (Constant) -- Reserved byte" + eol +
            softTab + "0x95, 0x05,                        // Report Count (5)" + eol +
            softTab + "0x75, 0x01,                        // Report Size (1)" + eol +
            softTab + "0x05, 0x08,                        // Usage Page (Page# for LEDs)" + eol +
            softTab + "0x19, 0x01,                        // Usage Minimum (1)" + eol +
            softTab + "0x29, 0x05,                        // Usage Maximum (5)" + eol +
            softTab + "0x91, 0x02,                        // Output (Data, Variable, Absolute) -- LED report" + eol +
            softTab + "0x95, 0x01,                        // Report Count (1)" + eol +
            softTab + "0x75, 0x03,                        // Report Size (3)" + eol +
            softTab + "0x91, 0x03,                        // (91 03) Output (Constant) -- LED report padding" + eol +
            softTab + "0x95, 0x06,                        // Report Count (6)" + eol +
            softTab + "0x75, 0x08,                        // Report Size (8)" + eol +
            softTab + "0x15, 0x00,                        // Logical Minimum (0)" + eol +
            softTab + "0x25, 0x66,                        // Logical Maximum(102)  // was 0x65" + eol +
            softTab + "0x05, 0x07,                        // Usage Page (Key Codes)" + eol +
            softTab + "0x19, 0x00,                        // Usage Minimum (0)" + eol +
            softTab + "0x29, 0x66,                        // Usage Maximum (102) // was 0x65" + eol +
            softTab + "0x81, 0x00,                        // Input (Data, Array) -- Key arrays (6 bytes)" + eol +
            softTab + "0xC0                               // End Collection" +eol;
    
    final private String mouseReportString = 
            softTab + "0x05, 0x01,                // Usage Pg (Generic Desktop)" + eol +
            softTab + "0x09, 0x02,                // Usage (Mouse)" + eol +
            softTab + "0xA1, 0x01,                // Collection: (Application)" + eol +
            softTab + "0x09, 0x01,                // Usage Page (Vendor Defined)" + eol +
            softTab + "0xA1, 0x00,                // Collection (Linked)" + eol +
            softTab + "0x05, 0x09,                // Usage (Button)" + eol +
            softTab + "0x19, 0x01,                // Usage Min (#)" + eol +
            softTab + "0x29, 0x05,                // Usage Max (#)" + eol +
            softTab + "0x15, 0x00,                // Log Min (0)" + eol +
            softTab + "0x25, 0x01,                // Usage Maximum" + eol +
            softTab + "0x95, 0x05,                // Report count (5)" + eol +
            softTab + "0x75, 0x01,                // Report Size (1)" + eol +
            softTab + "0x81, 0x02,                // Input (Data,Var,Abs)" + eol +
            softTab + "0x95, 0x01,                // Report Count (1)" + eol +
            softTab + "0x75, 0x03,                // Report Size (3)" + eol +
            softTab + "0x81, 0x01,                // Input: (Constant)" + eol +
            softTab + "0x05, 0x01,                // Usage Pg (Generic Desktop)" + eol +
            softTab + "0x09, 0x30,                // Usage (X)" + eol +
            softTab + "0x09, 0x31,                // Usage (Y)" + eol +
            softTab + "0x09, 0x38,                // Usage (Wheel)" + eol +
            softTab + "0x15, 0x81,                // Log Min (-127)" + eol +
            softTab + "0x25, 0x7F,                // Log Max (127)" + eol +
            softTab + "0x75, 0x08,                // Report Size" + eol +
            softTab + "0x95, 0x03,                // Report Count (3)" + eol +
            softTab + "0x81, 0x06,                // Input: (Data, Variable, Relative)" + eol +
            softTab + "0xC0,                      // End Collection" + eol +
            softTab + "0xC0                       // End Collection" + eol;
    
    final private String dataPipeReportString =
            softTab + "0x06, 0x00, 0xff,    // Usage Page (Vendor Defined)" + eol +
            softTab + "0x09, 0x01,    // Usage Page (Vendor Defined)" + eol +
            softTab + "0xa1, 0x01,    // COLLECTION (Application)" + eol +
            softTab + "0x85, 0x3f,    // Report ID (Vendor Defined)" + eol +
            softTab + "0x95, MAX_PACKET_SIZE-1,    // Report Count" + eol +
            softTab + "0x75, 0x08,    // Report Size" + eol +
            softTab + "0x25, 0x01,    // Usage Maximum" + eol +
            softTab + "0x15, 0x01,    // Usage Minimum" + eol +
            softTab + "0x09, 0x01,    // Vendor Usage" + eol +
            softTab + "0x81, 0x02,    // Input (Data,Var,Abs)" + eol +
            softTab + "0x85, 0x3f,    // Report ID (Vendor Defined)" + eol +
            softTab + "0x95, MAX_PACKET_SIZE-1,    //Report Count" + eol +
            softTab + "0x75, 0x08,    // Report Size" + eol +
            softTab + "0x25, 0x01,    // Usage Maximum" + eol +
            softTab + "0x15, 0x01,    // Usage Minimum" + eol +
            softTab + "0x09, 0x01,    // Vendor Usage" + eol +
            softTab + "0x91 ,0x02,    // Ouput (Data,Var,Abs)" + eol +
            softTab + "0xc0    // end Application Collection" + eol;
    
    
    public String name = "HID Interface";
    public int interfNum = -1;
    public String interfString = "HID Interface";
    public int pollingInterv = 1;
    public int hidReportType = HID_REPORT_DATAPIPE;
    public String hidReportText = dataPipeReportString;
    public String reportDescSize = "36";
    public String reportLenInput = "64";
    
            
            
    // Getter and Setters
    public int getPollingInterv() {
        return pollingInterv;
    }

    public void setPollingInterv(int pollingInterv) {
        this.pollingInterv = pollingInterv;
    }

    public int getHidReportType() {
        return hidReportType;
    }

    public void setHidReportType(int hidReportType) {
        this.hidReportType = hidReportType;
        setSizeAndLength();
        switch(hidReportType){
            case 0:
                hidReportText = dataPipeReportString;
                break;
            case 1:
                hidReportText =keyboardReportString;
                break;
            case 2:
                hidReportText =mouseReportString;
                break;
            case 3:
                hidReportText =customReportText;
                break;
            default:
                break;
        }
    }
    
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
    

    public String getReportDescSize() {
        return reportDescSize;
    }

    public void setReportDescSize(String reportDescSize) {
        this.reportDescSize = reportDescSize;
    }

    public String getReportLenInput() {
        return reportLenInput;
    }

    public void setReportLenInput(String reportLenInput) {
        this.reportLenInput = reportLenInput;
    }

    public String getCustomReportText() {
        return customReportText;
    }

    public void setCustomReportText(String customReportText) {
        this.customReportText = customReportText;
        this.hidReportText = customReportText;
    }

    public String getHidReportText() {
        return hidReportText;
    }

    public void setHidReportText(String hidReportText) {
        this.hidReportText = hidReportText;
    }
    
    
    
    
 
    /**
     * Constructor for PHDCInterface class
     */
    public HIDInterface430() {
    }
 
    public String toString(){
        return name + "[" + interfNum + "]";
    }

    private void setSizeAndLength() {
        switch(hidReportType){
            case 0:
                setReportDescSize("36");
                setReportLenInput("64");
                break;
            case 1:
                setReportDescSize("63");
                setReportLenInput("8");
                break;
            case 2:
                setReportDescSize("52");
                setReportLenInput("4");
                break;
            case 3:
                // Size and length for custom HID report,
                // is taken care by HIDCustomReportDialog
                break;
            default:
                break;
        }
    }
    
}
//Released_Version_5_20_06_02
