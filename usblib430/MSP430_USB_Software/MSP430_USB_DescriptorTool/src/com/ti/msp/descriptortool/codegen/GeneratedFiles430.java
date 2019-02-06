/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.ti.msp.descriptortool.codegen;

import com.ti.msp.common.Directory;
import com.ti.msp.common.StaticInfo;
import com.ti.msp.descriptortool.USB.USBDeviceConfig;
import com.ti.msp.descriptortool.USB.USBModuleConfiguration;
import com.ti.msp.descriptortool.USBinterfaces.CDCInterface430;
import com.ti.msp.descriptortool.USBinterfaces.HIDInterface430;
import com.ti.msp.descriptortool.USBinterfaces.MSCInterface430;
import com.ti.msp.descriptortool.USBinterfaces.PHDCInterface430;
import com.ti.msp.descriptortool.mspConfig.MSP430Config;
import java.awt.Component;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URL;
import java.util.ArrayList;
import java.util.Date;
import javax.swing.JTree;
import java.util.TreeMap;
import javax.swing.JOptionPane;

/**
 *
 * @author a0272979
 */
public class GeneratedFiles430 {
    private boolean commandline;
    private File dest;
    private File src;
    private String softTab = "    ";
    JTree configTree;
    Component parent;
    private BufferedWriter oStream;
    private TreeMap<String, Callback> callbacks;
    private short numOfCdcInterfaces;
    private short numOfHidInterfaces;
    private short numOfMscInterfaces;
    private short numOfPhdcInterfaces;
    private boolean isComposite;
    private int hasCDCIntf; 
    private int hasHIDIntf; 
    private int hasMSCIntf; 
    private int hasPHDCIntf;
    // USB 2.0 Standard Device Descriptor, page 262
    // No access modifiers used = package private members
    private final short bLength = 18; // Descriptor length in bytes
    private short bDescriptorType;
    private int bcdUSB;
    private short bDeviceClass;
    private short bDeviceSubClass;
    private short bDeviceProtocol;
    private short bMaxPacketSize0;
    private int idVendor;
    private int idProduct;
    private int bcdDevice; // Device Release Num
    private short iManufacturer; // Index of Manufacturer String Descriptor
    private short iProduct; // Index of Product String Descriptor
    private short iSerialNumber; // Index of Serial Number String Descriptor
    private short bNumConfigurations; // Number of Possible Configurations
    private int bmaxlun;
    private int intfindex;
    private ArrayList<CDCInterface430> cDCInterface430List;
    private ArrayList<HIDInterface430> hidInterface430List;
    private ArrayList<MSCInterface430> mscInterface430List;
    private ArrayList<PHDCInterface430> phdcInterface430List;
    private MSP430Config mspConfig;
    private USBDeviceConfig usbDevConfig;
    private USBModuleConfiguration usbModConfig;
    private StringDescriptor stringLanguageDescriptor;
    private StringDescriptor stringDescriptorVendorString;
    private StringDescriptor stringDescriptorProductString;
    private StringDescriptor stringDescriptor3;
    private StringDescriptor stringDescriptorConfiguration;
    private short crystalOscillator;
    private boolean crystalSuspend;
    private String crystalSuspendString;
    private String crystalFrequency;
    private String crystalFrequencyValue;
    private int crystalFrequencyIndex;
    private String dmaTXChannelString;
    private int dmaTXChannel;
    private int dmaRXChannel;
    private boolean usingSYSBIOS;
    private final String USBPLL_SETCLK_1_5[] =
    { "USBPLL_SETCLK_1_5", "1.5 MHz" }; /* USB - PLL Set for 1.5 MHz input clock */
    private final String USBPLL_SETCLK_1_6[] =
    { "USBPLL_SETCLK_1_6", "1.6 MHz" }; /* USB - PLL Set for 1.6 MHz input clock */
    private final String USBPLL_SETCLK_1_7778[] =
    { "USBPLL_SETCLK_1_7778", "1.7778 MHz" }; /*
                                               * USB - PLL Set for 1.7778 MHz
                                               * input clock
                                               */
    private final String USBPLL_SETCLK_1_8432[] =
    { "USBPLL_SETCLK_1_8432", "1.8432 MHz" }; /*
                                               * USB - PLL Set for 1.8432 MHz
                                               * input clock
                                               */
    private final String USBPLL_SETCLK_1_8461[] =
    { "USBPLL_SETCLK_1_8461", "1.8461 MHz" }; /*
                                               * USB - PLL Set for 1.8461 MHz
                                               * input clock
                                               */
    private final String USBPLL_SETCLK_1_92[] =
    { "USBPLL_SETCLK_1_92", "1.92 MHz" }; /*
                                           * USB - PLL Set for 1.92 MHz input
                                           * clock
                                           */
    private final String USBPLL_SETCLK_2_0[] =
    { "USBPLL_SETCLK_2_0", "2.0 MHz" }; /* USB - PLL Set for 2.0 MHz input clock */
    private final String USBPLL_SETCLK_2_4[] =
    { "USBPLL_SETCLK_2_4", "2.4 MHz" }; /* USB - PLL Set for 2.4 MHz input clock */
    private final String USBPLL_SETCLK_2_6667[] =
    { "USBPLL_SETCLK_2_6667", "2.6667 MHz" }; /*
                                               * USB - PLL Set for 2.6667 MHz
                                               * input clock
                                               */
    private final String USBPLL_SETCLK_3_0[] =
    { "USBPLL_SETCLK_3_0", "3.0 MHz" }; /* USB - PLL Set for 3.0 MHz input clock */
    private final String USBPLL_SETCLK_3_2[] =
    { "USBPLL_SETCLK_3_2", "3.2 MHz" }; /* USB - PLL Set for 3.2 MHz input clock */
    private final String USBPLL_SETCLK_3_5556[] =
    { "USBPLL_SETCLK_3_5556", "3.5556 MHz" }; /*
                                               * USB - PLL Set for 3.5556 MHz
                                               * input clock
                                               */
    private final String USBPLL_SETCLK_3_579545[] =
    { "USBPLL_SETCLK_3_579545", "3.579546 MHz" }; /*
                                                   * USB - PLL Set for 3.579546
                                                   * MHz input clock
                                                   */
    private final String USBPLL_SETCLK_3_84[] =
    { "USBPLL_SETCLK_3_84", "3.84 MHz" }; /*
                                           * USB - PLL Set for 3.84 MHz input
                                           * clock
                                           */
    private final String USBPLL_SETCLK_4_0[] =
    { "USBPLL_SETCLK_4_0", "4.0 MHz" }; /* USB - PLL Set for 4.0 MHz input clock */
    private final String USBPLL_SETCLK_4_1739[] =
    { "USBPLL_SETCLK_4_1739", "4.1739 MHz" }; /*
                                               * USB - PLL Set for 4.1739 MHz
                                               * input clock
                                               */
    private final String USBPLL_SETCLK_4_1943[] =
    { "USBPLL_SETCLK_4_1943", "4.1943 MHz" }; /*
                                               * USB - PLL Set for 4.1943 MHz
                                               * input clock
                                               */
    private final String USBPLL_SETCLK_4_332[] =
    { "USBPLL_SETCLK_4_332", "4.332 MHz" }; /*
                                             * USB - PLL Set for 4.332 MHz input
                                             * clock
                                             */
    private final String USBPLL_SETCLK_4_3636[] =
    { "USBPLL_SETCLK_4_3636", "4.3636 MHz" }; /*
                                               * USB - PLL Set for 4.3636 MHz
                                               * input clock
                                               */
    private final String USBPLL_SETCLK_4_5[] =
    { "USBPLL_SETCLK_4_5", "4.5 MHz" }; /* USB - PLL Set for 4.5 MHz input clock */
    private final String USBPLL_SETCLK_4_8[] =
    { "USBPLL_SETCLK_4_8", "4.8 MHz" }; /* USB - PLL Set for 4.8 MHz input clock */
    private final String USBPLL_SETCLK_5_33[] =
    { "USBPLL_SETCLK_5_33", "5.33 MHz" }; /*
                                           * USB - PLL Set for 5.33 MHz input
                                           * clock
                                           */
    private final String USBPLL_SETCLK_5_76[] =
    { "USBPLL_SETCLK_5_76", "5.76 MHz" }; /*
                                           * USB - PLL Set for 5.76 MHz input
                                           * clock
                                           */
    private final String USBPLL_SETCLK_6_0[] =
    { "USBPLL_SETCLK_6_0", "6.0 MHz" }; /* USB - PLL Set for 6.0 MHz input clock */
    private final String USBPLL_SETCLK_6_4[] =
    { "USBPLL_SETCLK_6_4", "6.4 MHz" }; /* USB - PLL Set for 6.4 MHz input clock */
    private final String USBPLL_SETCLK_7_2[] =
    { "USBPLL_SETCLK_7_2", "7.2 MHz" }; /* USB - PLL Set for 7.2 MHz input clock */
    private final String USBPLL_SETCLK_7_68[] =
    { "USBPLL_SETCLK_7_68", "7.68 MHz" }; /*
                                           * USB - PLL Set for 7.68 MHz input
                                           * clock
                                           */
    private final String USBPLL_SETCLK_8_0[] =
    { "USBPLL_SETCLK_8_0", "8.0 MHz" }; /* USB - PLL Set for 8.0 MHz input clock */
    private final String USBPLL_SETCLK_9_0[] =
    { "USBPLL_SETCLK_9_0", "9.0 MHz" }; /* USB - PLL Set for 9.0 MHz input clock */
    private final String USBPLL_SETCLK_9_6[] =
    { "USBPLL_SETCLK_9_6", "9.6 MHz" }; /* USB - PLL Set for 9.6 MHz input clock */
    private final String USBPLL_SETCLK_10_66[] =
    { "USBPLL_SETCLK_10_66", "10.66 MHz" }; /*
                                             * USB - PLL Set for 10.66 MHz input
                                             * clock
                                             */
    private final String USBPLL_SETCLK_12_0[] =
    { "USBPLL_SETCLK_12_0", "12.0 MHz" }; /*
                                           * USB - PLL Set for 12.0 MHz input
                                           * clock
                                           */
    private final String USBPLL_SETCLK_12_8[] =
    { "USBPLL_SETCLK_12_8", "12.8 MHz" }; /*
                                           * USB - PLL Set for 12.8 MHz input
                                           * clock
                                           */
    private final String USBPLL_SETCLK_14_4[] =
    { "USBPLL_SETCLK_14_4", "14.4 MHz" }; /*
                                           * USB - PLL Set for 14.4 MHz input
                                           * clock
                                           */
    private final String USBPLL_SETCLK_16_0[] =
    { "USBPLL_SETCLK_16_0", "16.0 MHz" }; /*
                                           * USB - PLL Set for 16.0 MHz input
                                           * clock
                                           */
    private final String USBPLL_SETCLK_16_9344[] =
    { "USBPLL_SETCLK_16_9344", "16.9344 MHz" }; /*
                                                 * USB - PLL Set for 16.9344 MHz
                                                 * input clock
                                                 */
    private final String USBPLL_SETCLK_16_94118[] =
    { "USBPLL_SETCLK_16_94118", "16.94118 MHz" };/*
                                                  * USB - PLL Set for 16.94118
                                                  * MHz input clock
                                                  */
    private final String USBPLL_SETCLK_18_0[] =
    { "USBPLL_SETCLK_18_0", "18.0 MHz" }; /*
                                           * USB - PLL Set for 18.0 MHz input
                                           * clock
                                           */
    private final String USBPLL_SETCLK_19_2[] =
    { "USBPLL_SETCLK_19_2", "19.2 MHz" }; /*
                                           * USB - PLL Set for 19.2 MHz input
                                           * clock
                                           */
    private final String USBPLL_SETCLK_24_0[] =
    { "USBPLL_SETCLK_24_0", "24.0 MHz" }; /*
                                           * USB - PLL Set for 24.0 MHz input
                                           * clock
                                           */
    private final String USBPLL_SETCLK_25_6[] =
    { "USBPLL_SETCLK_25_6", "25.6 MHz" }; /*
                                           * USB - PLL Set for 25.6 MHz input
                                           * clock
                                           */
    private final String USBPLL_SETCLK_26_0[] =
    { "USBPLL_SETCLK_26_0", "26.0 MHz" }; /*
                                           * USB - PLL Set for 26.0 MHz input
                                           * clock
                                           */
    private final String USBPLL_SETCLK_32_0[] =
    { "USBPLL_SETCLK_32_0", "32.0 MHz" }; /*
                                           * USB - PLL Set for 32.0 MHz input
                                           * clock
                                           */
    private final String[] usbPllList0 =
    {   new String(USBPLL_SETCLK_1_5[0]), 
        new String(USBPLL_SETCLK_1_6[0]),
        new String(USBPLL_SETCLK_1_7778[0]),
        new String(USBPLL_SETCLK_1_8432[0]),
        new String(USBPLL_SETCLK_1_8461[0]),
        new String(USBPLL_SETCLK_1_92[0]),
        new String(USBPLL_SETCLK_2_0[0]), 
        new String(USBPLL_SETCLK_2_4[0]),
        new String(USBPLL_SETCLK_2_6667[0]),
        new String(USBPLL_SETCLK_3_0[0]), 
        new String(USBPLL_SETCLK_3_2[0]),
        new String(USBPLL_SETCLK_3_5556[0]),
        new String(USBPLL_SETCLK_3_579545[0]),
        new String(USBPLL_SETCLK_3_84[0]),
        new String(USBPLL_SETCLK_4_0[0]),
        new String(USBPLL_SETCLK_4_1739[0]),
        new String(USBPLL_SETCLK_4_1943[0]),
        new String(USBPLL_SETCLK_4_332[0]),
        new String(USBPLL_SETCLK_4_3636[0]),
        new String(USBPLL_SETCLK_4_5[0]), 
        new String(USBPLL_SETCLK_4_8[0]),
        new String(USBPLL_SETCLK_5_33[0]),
        new String(USBPLL_SETCLK_5_76[0]),
        new String(USBPLL_SETCLK_6_0[0]), 
        new String(USBPLL_SETCLK_6_4[0]),
        new String(USBPLL_SETCLK_7_2[0]),
        new String(USBPLL_SETCLK_7_68[0]),
        new String(USBPLL_SETCLK_8_0[0]), 
        new String(USBPLL_SETCLK_9_0[0]),
        new String(USBPLL_SETCLK_9_6[0]),
        new String(USBPLL_SETCLK_10_66[0]),
        new String(USBPLL_SETCLK_12_0[0]),
        new String(USBPLL_SETCLK_12_8[0]),
        new String(USBPLL_SETCLK_14_4[0]),
        new String(USBPLL_SETCLK_16_0[0]),
        new String(USBPLL_SETCLK_16_9344[0]),
        new String(USBPLL_SETCLK_16_94118[0]),
        new String(USBPLL_SETCLK_18_0[0]),
        new String(USBPLL_SETCLK_19_2[0]),
        new String(USBPLL_SETCLK_24_0[0]),
        new String(USBPLL_SETCLK_25_6[0]),
        new String(USBPLL_SETCLK_26_0[0]),
        new String(USBPLL_SETCLK_32_0[0]) };

    private final String[] usbPllList1 =
    {   new String(USBPLL_SETCLK_1_5[1]), 
        new String(USBPLL_SETCLK_1_6[1]),
        new String(USBPLL_SETCLK_1_7778[1]),
        new String(USBPLL_SETCLK_1_8432[1]),
        new String(USBPLL_SETCLK_1_8461[1]),
        new String(USBPLL_SETCLK_1_92[1]),
        new String(USBPLL_SETCLK_2_0[1]), 
        new String(USBPLL_SETCLK_2_4[1]),
        new String(USBPLL_SETCLK_2_6667[1]),
        new String(USBPLL_SETCLK_3_0[1]), 
        new String(USBPLL_SETCLK_3_2[1]),
        new String(USBPLL_SETCLK_3_5556[1]),
        new String(USBPLL_SETCLK_3_579545[1]),
        new String(USBPLL_SETCLK_3_84[1]),
        new String(USBPLL_SETCLK_4_0[1]),
        new String(USBPLL_SETCLK_4_1739[1]),
        new String(USBPLL_SETCLK_4_1943[1]),
        new String(USBPLL_SETCLK_4_332[1]),
        new String(USBPLL_SETCLK_4_3636[1]),
        new String(USBPLL_SETCLK_4_5[1]), 
        new String(USBPLL_SETCLK_4_8[1]),
        new String(USBPLL_SETCLK_5_33[1]),
        new String(USBPLL_SETCLK_5_76[1]),
        new String(USBPLL_SETCLK_6_0[1]), 
        new String(USBPLL_SETCLK_6_4[1]),
        new String(USBPLL_SETCLK_7_2[1]),
        new String(USBPLL_SETCLK_7_68[1]),
        new String(USBPLL_SETCLK_8_0[1]), 
        new String(USBPLL_SETCLK_9_0[1]),
        new String(USBPLL_SETCLK_9_6[1]),
        new String(USBPLL_SETCLK_10_66[1]),
        new String(USBPLL_SETCLK_12_0[1]),
        new String(USBPLL_SETCLK_12_8[1]),
        new String(USBPLL_SETCLK_14_4[1]),
        new String(USBPLL_SETCLK_16_0[1]),
        new String(USBPLL_SETCLK_16_9344[1]),
        new String(USBPLL_SETCLK_16_94118[1]),
        new String(USBPLL_SETCLK_18_0[1]),
        new String(USBPLL_SETCLK_19_2[1]),
        new String(USBPLL_SETCLK_24_0[1]),
        new String(USBPLL_SETCLK_25_6[1]),
        new String(USBPLL_SETCLK_26_0[1]),
        new String(USBPLL_SETCLK_32_0[1]) };
    private int wTotalLength = 0x0035; // Total Length is dependent on CLASS type!
    private int bNumInterfaces; // Number of Interfaces
    private int bConfigurationvalue; // Value to use as an argument to select this
                               // configuration
    private int iConfiguration; // Index of String Descriptor describing this
                          // configuration
    private int bmAttributes; // bmAttributes, bus power, remote wakeup
    private int USB_SUPPORT_SELF_POWERED; // bus power
    private int USB_SUPPORT_REM_WAKE; // remote wakeup
    private int bMaxPower;
    private int[] hidType;
    private StringDescriptor stringConfigDescriptor;
    private ArrayList<Object> orderedInterfaces = new ArrayList<Object>();
    private int interfceCount = 0; 
    private int totalinterfaceCount = 0;
    private boolean previousCDC =false;
    private boolean previousPHDC =false;
    private int cdcCount =0;
    private int hidCount =0;
    private int mscCount =0;
    private int phdcCount =0;
    private int loopCount = 1;
    
    private String formattedVid;
    private String formattedPid;
    
    

    public GeneratedFiles430(String outputDir, JTree configTree, Component parent, 
            boolean commandline) {
        this.configTree = configTree;
        this.parent = parent;
        this.dest = new File(outputDir);
        this.commandline = commandline;
        this.src = new File("configFiles/templates/codeGen/descriptorFiles");
        
        processConfigTree();
        
        callbacks = new TreeMap<String, Callback>();
        callbacks.put("<MUDTCG_COPYRIGHT>", new MUDTCG_COPYRIGHT());
        callbacks.put("<MUDTCG_DATE>", new MUDTCG_DATE());
        callbacks.put("<MUDTCG_DESC_C_INCLUDES_FOR_CDC_OR_HID>", new MUDTCG_DESC_C_INCLUDES_FOR_CDC_OR_HID());
        callbacks.put("<MUDTCG_DESC_C_GLOBALS_FOR_CDC_OR_HID>", new MUDTCG_DESC_C_GLOBALS_FOR_CDC_OR_HID());
        callbacks.put("<MUDTCG_DESC_C_INCLUDES_FOR_MSC>", new MUDTCG_DESC_C_INCLUDES_FOR_MSC());
        callbacks.put("<MUDTCG_DESC_C_INCLUDES_FOR_PHDC>", new MUDTCG_DESC_C_INCLUDES_FOR_PHDC());
        callbacks.put("<MUDTCG_CDC_REQUEST_LIST>", new MUDTCG_CDC_REQUEST_LIST());
        callbacks.put("<MUDTCG_HID_REQUEST_LIST>", new MUDTCG_HID_REQUEST_LIST());
        callbacks.put("<MUDTCG_MSC_REQUEST_LIST>", new MUDTCG_MSC_REQUEST_LIST());
        callbacks.put("<MUDTCG_PHDC_REQUEST_LIST>", new MUDTCG_PHDC_REQUEST_LIST());
        callbacks.put("<MUDTCG_MSC_DESC_INFO>", new MUDTCG_MSC_DESC_INFO());
        callbacks.put("<MUDTCG_DESC_C_ABROMDEVICEDESC_STRUCT>", new MUDTCG_DESC_C_ABROMDEVICEDESC_STRUCT());
        callbacks.put("<MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_MSC_STRUCT>", new MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_MSC_STRUCT());
        callbacks.put("<MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_CDC_STRUCT>", new MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_CDC_STRUCT());
        callbacks.put("<MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_HID_STRUCT>", new MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_HID_STRUCT());
        callbacks.put("<MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_PHDC_STRUCT>", new MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_PHDC_STRUCT());
        callbacks.put("<MUDTCG_DESC_C_ABROMSTRINGDESC>", new MUDTCG_DESC_C_ABROMSTRINGDESC());
        callbacks.put("<MUDTCG_DESC_C_HID_REPORT_DESC>", new MUDTCG_DESC_C_HID_REPORT_DESC());
        callbacks.put("<MUDTCG_DESC_C_HID_REPORT_DESC_POINTERS>", new MUDTCG_DESC_C_HID_REPORT_DESC_POINTERS());
        callbacks.put("<MUDTCG_DESC_C_HID_USBHANDLE>", new MUDTCG_DESC_C_HID_USBHANDLE());
        callbacks.put("<MUDTCG_DESC_H_INCLUDES_FOR_PHDC>", new MUDTCG_DESC_H_INCLUDES_FOR_PHDC());
        callbacks.put("<MUDTCG_DESC_H_COMPOSITE_DEFINES>", new MUDTCG_DESC_H_COMPOSITE_DEFINES());
        callbacks.put("<MUDTCG_DESC_H_VID_PID_INFO>", new MUDTCG_DESC_H_VID_PID_INFO());
        callbacks.put("<MUDTCG_DESC_H_FW_VER_INFO>", new MUDTCG_DESC_H_FW_VER_INFO());
        callbacks.put("<MUDTCG_DESC_H_SERNUM_INFO>", new MUDTCG_DESC_H_SERNUM_INFO());
        callbacks.put("<MUDTCG_DESC_H_INTERFACE_INFO>", new MUDTCG_DESC_H_INTERFACE_INFO());
        callbacks.put("<MUDTCG_DESC_H_MSP430CONFIG_LDO_XTAL>", new MUDTCG_DESC_H_MSP430CONFIG_LDO_XTAL());
        callbacks.put("<MUDTCG_DESC_H_MSP430CONFIG_CLK_DMA>", new MUDTCG_DESC_H_MSP430CONFIG_CLK_DMA());
        callbacks.put("<MUDTCG_DESC_H_MSP430CONFIG_REM_WAKE>", new MUDTCG_DESC_H_MSP430CONFIG_REM_WAKE());
        callbacks.put("<MUDTCG_DESC_H_MSP430CONFIG_SELF_POWER>", new MUDTCG_DESC_H_MSP430CONFIG_SELF_POWER());
        callbacks.put("<MUDTCG_DESC_H_MSP430CONFIG_MAX_POWER>", new MUDTCG_DESC_H_MSP430CONFIG_MAX_POWER());
        callbacks.put("<MUDTCG_DESC_H_MAX_STRING_DESCRIPTOR_INDEX>", new MUDTCG_DESC_H_MAX_STRING_DESCRIPTOR_INDEX());
        callbacks.put("<MUDTCG_DESC_H_DESC_SIZE_HID>", new MUDTCG_DESC_H_DESC_SIZE_HID());
        callbacks.put("<MUDTCG_DESC_H_CONFIG_STRING_INDEX>", new MUDTCG_DESC_H_CONFIG_STRING_INDEX());
        callbacks.put("<MUDTCG_DESC_H_INTERFACE_ASSOCIATION_DESCRIPTOR>", new MUDTCG_DESC_H_INTERFACE_ASSOCIATION_DESCRIPTOR());
        callbacks.put("<MUDTCG_DESC_H_ABROMCONFIGURATIONDESCRIPTORPHDC>", new MUDTCG_DESC_H_ABROMCONFIGURATIONDESCRIPTORPHDC());
        callbacks.put("<MUDTCG_USBISR_C_INCLUDES>", new MUDTCG_USBISR_C_INCLUDES());
        callbacks.put("<MUDTCG_USBISR_C_IUSBINTERRUPTHANDLER>", new MUDTCG_USBISR_C_IUSBINTERRUPTHANDLER());
        callbacks.put("<MUDTCG_USBISR_C_USBVECINT_INPUT_ENDPOINTX>", new MUDTCG_USBISR_C_USBVECINT_INPUT_ENDPOINTX());
        callbacks.put("<MUDTCG_USBISR_C_USBVECINT_OUTPUT_ENDPOINTX>", new MUDTCG_USBISR_C_USBVECINT_OUTPUT_ENDPOINTX());
        callbacks.put("<MUDTCG_USBISR_C_EXIT_LPM>", new MUDTCG_USBISR_C_EXIT_LPM());
        callbacks.put("<MUDTCG_INF_FILE_DEVICE_LIST>", new MUDTCG_INF_FILE_DEVICE_LIST());
        callbacks.put("<MUDTCG_INF_FILE_DEVICE_LIST_64>", new MUDTCG_INF_FILE_DEVICE_LIST_64());
        callbacks.put("<MUDTCG_INF_STRING_DESCRIPTORS>", new MUDTCG_INF_STRING_DESCRIPTORS());
        
//        callbacks.put("<>", new ());
        
        
        
    }

    private boolean askUserToGenerateINF() {
        Object[] options = {"Yes","No"};
        
        int n = JOptionPane.showOptionDialog(parent, 
            "Do you want to generate \"MSP430_CDC.inf\"?", 
            "Generate MSP430_CDC.inf", 
            JOptionPane.YES_NO_OPTION, 
            JOptionPane.QUESTION_MESSAGE,
            null,
            options,
            options[1]);

        if(n == JOptionPane.YES_OPTION){
            return true;
        }
        return false;
    }
    
    public interface Callback {

        public void execute();
    }
    
    private void processConfigTree() {
        ConfigTreeProcessor processor  = new ConfigTreeProcessor(configTree);
        hasCDCIntf = 0;
        hasHIDIntf = 0;
        hasMSCIntf = 0;
        hasPHDCIntf = 0;
        isComposite = false;
        bDescriptorType = 0x01;
        bcdUSB = 0x0200; // USB 2.0
        bDeviceClass = 0xEF;
        bDeviceSubClass = 0x02;
        bDeviceProtocol = 0x01;
        bMaxPacketSize0 = 8;
        idVendor = 0x2047;
        bcdDevice = 0x0200;
        iManufacturer = 1;
        iProduct = 2;
        iSerialNumber = 3;
        bNumConfigurations = 1;
        bmaxlun = 1;
        intfindex = 0;
        
       
        mspConfig = processor.getMSP430Config();
        usbDevConfig = processor.getUSBDeviceConfig();
        usbModConfig = processor.getUSBModuleConfig();
        
        stringLanguageDescriptor = new StringDescriptor("");
        stringDescriptorVendorString = new StringDescriptor(usbDevConfig.getVendor());
        stringDescriptorProductString = new StringDescriptor(usbDevConfig.getProduct());
        
        if(usbDevConfig.useSerial){
            stringDescriptor3 = new StringDescriptor("0");
        }else{
            stringDescriptor3 = new StringDescriptor(usbDevConfig.getDevRelNum());
        }
        
        stringDescriptorConfiguration = new StringDescriptor(("MSP430 USB"));

        numOfMscInterfaces = processor.getNumberOfMSCinterfaces();
        if(numOfMscInterfaces > 0 ){
            hasMSCIntf = 1;
            mscInterface430List = processor.getMSCinterfaces();
            for(MSCInterface430 intf : mscInterface430List){
                orderedInterfaces.add(intf);
                for(Integer  key : intf.getLunMap().keySet()){
                   MSCInterface430.LogicalUnit lun = intf.getLunMap().get(key);
                   if(lun.isUseUSBVidinfo()==true){
                       lun.setT10Vendor(usbDevConfig.getVendor());
                       lun.setT10Product(usbDevConfig.getProduct());
                       lun.setT10Revision(usbDevConfig.getDevRelNum());
                   }
                }
                bmaxlun = intf.getLunMap().size();
                totalinterfaceCount++;
            }
        }
        
        numOfCdcInterfaces = processor.getNumberOfCDCinterfaces();
        if(numOfCdcInterfaces > 0 ){
            hasCDCIntf = 1;
            cDCInterface430List = processor.getCDCinterfaces();
            for(CDCInterface430 intf : cDCInterface430List){
                orderedInterfaces.add(intf);
                totalinterfaceCount++;
            }
        }
        
        numOfHidInterfaces = processor.getNumberOfHIDinterfaces();
        if(numOfHidInterfaces > 0 ){
            hasHIDIntf = 1;
            hidInterface430List = processor.getHIDinterfaces();
            for(HIDInterface430 intf : hidInterface430List){
                orderedInterfaces.add(intf);
                totalinterfaceCount++;
            }
        }
        
        numOfPhdcInterfaces = processor.getNumberOfPHDCinterfaces();
        if(numOfPhdcInterfaces > 0 ){
            hasPHDCIntf = 1;
            phdcInterface430List = processor.getPHDCinterfaces();
            for(PHDCInterface430 intf : phdcInterface430List){
                orderedInterfaces.add(intf);
                totalinterfaceCount++;
            }
        }

        if (((1 == numOfCdcInterfaces) && (0 == numOfHidInterfaces) && (0 == 
                numOfMscInterfaces)))
        {
            bDeviceClass = 0x02;
            bDeviceSubClass = 0;
            bDeviceProtocol = 0;
        } else if (0 == numOfCdcInterfaces)
        {
            bDeviceClass = 0;
            bDeviceSubClass = 0;
            bDeviceProtocol = 0;
        } else
        {
            bDeviceClass = 0xEF;
            bDeviceSubClass = 0x02;
            bDeviceProtocol = 0x01;
        }
        
        
        //MSP430configuration 
        crystalFrequencyIndex = mspConfig.getCrystOScSel();
        crystalOscillator = 2;
        crystalSuspend = mspConfig.isDisCrysOnSus();
        usingSYSBIOS = false;
        if (crystalSuspend)
        {
            crystalSuspendString = "1";
        } else
        {
            crystalSuspendString = "0";
        }
        crystalFrequency = usbPllList0[crystalFrequencyIndex];
        crystalFrequencyValue = usbPllList1[crystalFrequencyIndex].substring(0,
                usbPllList1[crystalFrequencyIndex].indexOf("M"));
        dmaTXChannel = mspConfig.getDamChannel()-1;
        dmaRXChannel = mspConfig.getDamChannel()-1;
        
        if (dmaTXChannel == 0){
            this.dmaTXChannelString = "DMA_CHANNEL_0";
        }else if(dmaTXChannel == 1){
            this.dmaTXChannelString = "DMA_CHANNEL_1";
        }else if(dmaTXChannel == 2){
            this.dmaTXChannelString = "DMA_CHANNEL_2";
        }else if(dmaTXChannel == 3){
            this.dmaTXChannelString = "DMA_CHANNEL_3";
        }else if(dmaTXChannel == 4){
            this.dmaTXChannelString = "DMA_CHANNEL_4";
        }else if(dmaTXChannel == 5){
            this.dmaTXChannelString = "DMA_CHANNEL_5";
        }else if(dmaTXChannel == 6){
            this.dmaTXChannelString = "DMA_CHANNEL_6";
        }else if(dmaTXChannel == 7){
            this.dmaTXChannelString = "DMA_CHANNEL_7";
        }else {
            this.dmaTXChannelString = "0xff";
        }
        
        hidType = new int[7];
        bNumInterfaces = 1; // Currently supports only 1
        bConfigurationvalue = 1;
        iConfiguration = 0;
        if(usbModConfig.isRemoteWakeUp()==false){
            USB_SUPPORT_REM_WAKE = 0x00;
        }else{
            USB_SUPPORT_REM_WAKE = 0x20;
        }
        if(usbModConfig.isSelfPowered()){
            USB_SUPPORT_SELF_POWERED = 0xc0;
        }else{
            USB_SUPPORT_SELF_POWERED = 0x80;
        }
        bmAttributes = (USB_SUPPORT_SELF_POWERED | USB_SUPPORT_REM_WAKE);
        bMaxPower = ((500 - 2*(usbModConfig.getMaxPowerDraw()))/2);
        stringConfigDescriptor = new StringDescriptor("MSP430 USB");
        
        String vid = usbDevConfig.getvID().toLowerCase().trim();
        String pid = usbDevConfig.getpID().toLowerCase().trim();
        
        StringBuilder temp = new StringBuilder();
        if(vid.contains("0x")){
            temp.append(vid.replace("0x", ""));
        }else{
            temp.append(Integer.toHexString(Integer.parseInt(vid)));
        }
        
        while(temp.length()<4){
            temp.insert(0, "0");
        }
        formattedVid = temp.toString();
        
        StringBuilder temp1 = new StringBuilder();
        if(pid.contains("0x")){
            temp1.append(pid.replace("0x", ""));
        }else{
            temp1.append(Integer.toHexString(Integer.parseInt(pid)));
        }
        
        while(temp1.length()<4){
            temp1.insert(0, "0");
        }
        formattedPid = temp1.toString();
       
        
    }
    
    
    public boolean generate(boolean infGen){
        
        if(orderedInterfaces.size()>0){
            copyTemplateSource(src, dest);

            if(numOfCdcInterfaces == 0 ){
                File inf = new File(dest+"/MSP430_CDC.inf");
                inf.delete();
            }
            else{
                
                if(commandline == false){
                    if(askUserToGenerateINF()==true){
                        copyTemplateSource(new File("configFiles/templates/codeGen/inf"), dest);
                        File inf = new File(dest.getAbsolutePath() + File.separator + "MSP430_CDC.inf");
                        processFile(inf);
                    }
                }else{
                    if(infGen == true){
                       copyTemplateSource(new File("configFiles/templates/codeGen/inf"), dest);
                       File inf = new File(dest.getAbsolutePath() + File.separator + "MSP430_CDC.inf");
                       processFile(inf);
                    }
                }
                
                
            }
            
            
            

            for(File file : dest.listFiles()){
                if(file.getAbsolutePath().contains("descriptors.c") ||
                           file.getAbsolutePath().contains("descriptors.h") ||
                           file.getAbsolutePath().contains("UsbIsr.c")){
                    processFile(file);
                }
            }
        }
        
        return true;
    }
    
    
    /**
     * Class that performs the substitution for the tag <FMCG_COPYRIGHT>
     */
    class MUDTCG_COPYRIGHT implements Callback {

        @Override
        public void execute() {
            try {
                URL url = GeneratedFiles430.class.getResource("Copyright.h");
                BufferedReader br = new BufferedReader(new InputStreamReader(url.openStream()));

                String strLine;
                while ((strLine = br.readLine()) != null) {
                    oStream.write(strLine);
                    oStream.newLine();
                }
                // Close the file streams
                br.close();
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }

    /**
     * Class that performs the substitution for the tag <FMCG_DATE>
     */
    class MUDTCG_DATE implements Callback {

        @Override
        public void execute() {
            try {
                Date timeStamp = new Date();
                oStream.newLine();
                oStream.write("// Generated by MSP USB Descriptor Tool: "+ timeStamp);
                oStream.newLine();
                oStream.newLine();

            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    } 
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_C_INCLUDES_FOR_CDC_OR_HID>
     */
    class MUDTCG_DESC_C_INCLUDES_FOR_CDC_OR_HID implements Callback {
        @Override
        public void execute() {
            try{
                if((numOfCdcInterfaces >0) || (numOfHidInterfaces >0)){
                    oStream.write("#include <USB_API/USB_CDC_API/UsbCdc.h>");
                    oStream.newLine();
                    oStream.write("#include <USB_API/USB_HID_API/UsbHidReq.h>");
                    oStream.newLine();
                } 
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_C_INCLUDES_FOR_MSC>
     */
    class MUDTCG_DESC_C_INCLUDES_FOR_MSC implements Callback {
        @Override
        public void execute() {
            try{
                if(numOfMscInterfaces > 0){
                    oStream.write("#include <USB_API/USB_MSC_API/UsbMscReq.h>");
                    oStream.newLine();
                    oStream.write("#include <USB_API/USB_MSC_API/UsbMscScsi.h>");
                    oStream.newLine();
                }   
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_C_INCLUDES_FOR_PHDC>
     */
    class MUDTCG_DESC_C_INCLUDES_FOR_PHDC implements Callback {
        @Override
        public void execute() {
            try{
                if(numOfPhdcInterfaces > 0){
                    oStream.write("#include <USB_API/USB_PHDC_API/UsbPHDC.h>");
                    oStream.newLine();
                }
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_CDC_REQUEST_LIST>
     */
    class MUDTCG_CDC_REQUEST_LIST implements Callback {
        @Override
        public void execute() {
            try{
                for (int i = 0; i < numOfCdcInterfaces; i++)
                {
                   oStream.write(softTab + "{");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "//---- CDC" + i + "Class Requests -----//");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "// GET LINE CODING");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "USB_REQ_TYPE_INPUT | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "USB_CDC_GET_LINE_CODING,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "0x00,0x00,                                 // always zero");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "CDC" + i + "_COMM_INTERFACE,0x00,                 // CDC interface is" + i);
                   oStream.newLine();
                   oStream.write(softTab + softTab + "0x07,0x00,                                 // Size of Structure (data length)");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "0xff,&usbGetLineCoding,");
                   oStream.newLine();
                   oStream.write(softTab + "},");
                   oStream.newLine();
                   oStream.write(softTab + "{");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "// SET LINE CODING");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "USB_REQ_TYPE_OUTPUT | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "USB_CDC_SET_LINE_CODING,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "0x00,0x00,                                 // always zero");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "CDC" + i + "_COMM_INTERFACE,0x00,                  // CDC interface is 0");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "0x07,0x00,                                 // Size of Structure (data length)");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "0xff,&usbSetLineCoding,");
                   oStream.newLine();
                   oStream.write(softTab + "},");
                   oStream.newLine();
                   oStream.write(softTab + "{");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "// SET CONTROL LINE STATE");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "USB_REQ_TYPE_OUTPUT | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "USB_CDC_SET_CONTROL_LINE_STATE,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "0xff,0xff,                                 // Contains data");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "CDC" + i + "_COMM_INTERFACE,0x00,                 // CDC interface is " + i);
                   oStream.newLine(); 
                   oStream.write(softTab + softTab + "0x00,0x00,                                 // No further data");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "0xcf,&usbSetControlLineState,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "},");
                   oStream.newLine();
                   oStream.newLine();
                }
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_HID_REQUEST_LIST>
     */
    class MUDTCG_HID_REQUEST_LIST implements Callback {
        @Override
        public void execute() {
            try{
                for (int i = 0; i < numOfHidInterfaces; i++)
                {
                   oStream.write(softTab + "{");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "//---- HID " + i + " Class Requests -----//");
                   oStream.newLine();
                   oStream.write(softTab + softTab + softTab + "USB_REQ_TYPE_INPUT | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,");
                   oStream.newLine();
                   
                   HIDInterface430 hidInt = hidInterface430List.get(i);
                   
                    if((hidInt.getHidReportType() == HIDInterface430.HID_REPORT_KEYBOARD) ||
                        (hidInt.getHidReportType() == HIDInterface430.HID_REPORT_MOUSE))
                    {
                        oStream.write(softTab + softTab + softTab + "USB_REQ_GET_IDLE,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "0xff,0xff,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "HID" + i + "_REPORT_INTERFACE,0x00,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "0xff,0xff,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "0xcc,&usbGetIdle,");
                        oStream.newLine();
                        oStream.write(softTab + "},");
                        oStream.newLine();
                        oStream.write(softTab + "{");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "USB_REQ_TYPE_OUTPUT | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "USB_REQ_SET_IDLE,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "0xff,0xFF,                          // bValueL is index and bValueH is type");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "HID" + i + "_REPORT_INTERFACE,0x00,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "0xff,0xff,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "0xcc,&usbSetIdle,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "},");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "{");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "USB_REQ_TYPE_INPUT | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "USB_REQ_GET_PROTOCOL,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "0xff,0xff,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "HID" + i + "_REPORT_INTERFACE,0x00,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "0xff,0xff,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "0xcc,&usbGetProtocol,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "},");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "{");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "USB_REQ_TYPE_OUTPUT | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "USB_REQ_SET_PROTOCOL,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "0xff,0xFF,                          // bValueL is index and bValueH is type");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "HID" + i + "_REPORT_INTERFACE,0x00,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "0xff,0xff,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "0xcc,&usbSetProtocol,");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "},");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "{");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "USB_REQ_TYPE_INPUT | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,");
                        oStream.newLine();
                     }
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "USB_REQ_GET_REPORT,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xff,0xff,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "HID" + i + "_REPORT_INTERFACE,0x00,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xff,0xff,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xcc,&usbGetReport,");
                    oStream.newLine();
                    oStream.write(softTab + "},");
                    oStream.newLine();
                    oStream.write(softTab + "{");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "// SET REPORT");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "USB_REQ_TYPE_OUTPUT | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "USB_REQ_SET_REPORT,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xff,0xFF,                          // bValueL is index and bValueH is type");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "HID" + i + "_REPORT_INTERFACE,0x00,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xff,0xff,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xcc,&usbSetReport,");
                    oStream.newLine();
                    oStream.write(softTab + "},");
                    oStream.newLine();
                    oStream.write(softTab + "{");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "// GET REPORT DESCRIPTOR");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "USB_REQ_TYPE_INPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_INTERFACE,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "USB_REQ_GET_DESCRIPTOR,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xff,DESC_TYPE_REPORT,              // bValueL is index and bValueH is type");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "HID" + i + "_REPORT_INTERFACE,0x00,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xff,0xff,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xdc,&usbGetReportDescriptor,");
                    oStream.newLine();
                    oStream.write(softTab + "},");
                    oStream.newLine();
                    oStream.write(softTab + "{");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "// GET HID DESCRIPTOR");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "USB_REQ_TYPE_INPUT | USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_INTERFACE,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "USB_REQ_GET_DESCRIPTOR,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xff,DESC_TYPE_HID,                 // bValueL is index and bValueH is type");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "HID" + i + "_REPORT_INTERFACE,0x00,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xff,0xff,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xdc,&usbGetHidDescriptor,");
                    oStream.newLine();
                    oStream.write(softTab + "},");
                    oStream.newLine();
                } 
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_MSC_REQUEST_LIST>
     */
    class MUDTCG_MSC_REQUEST_LIST implements Callback {
        @Override
        public void execute() {
            try{
                for (int i = 0; i < numOfMscInterfaces; i++){
                    oStream.write(softTab + "{");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//---- MSC Class Requests -----//");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "// Reset MSC");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "USB_REQ_TYPE_OUTPUT | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "USB_MSC_RESET_BULK,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x00,0x00,                    // always zero");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "MSC0_DATA_INTERFACE,0x00,     // MSC interface is 0");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x00,0x00,                    // Size of Structure (data length)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xff,&USBMSC_reset,");
                    oStream.newLine();
                    oStream.write(softTab + "},");
                    oStream.newLine();
                    oStream.write(softTab +"{");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "// Get Max Lun");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "USB_REQ_TYPE_INPUT | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "USB_MSC_GET_MAX_LUN,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x00,0x00,                    // always zero");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "MSC0_DATA_INTERFACE,0x00,     // MSC interface is 0");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x01,0x00,                    // Size of Structure (data length)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xff,&Get_MaxLUN,");
                    oStream.newLine();
                    oStream.write(softTab + "},");
                    oStream.newLine();
                }  
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_PHDC_REQUEST_LIST>
     */
    class MUDTCG_PHDC_REQUEST_LIST implements Callback {
        @Override
        public void execute() {
            try{
                for (int i = 0; i < numOfPhdcInterfaces; i++){
                    oStream.write(softTab + "{");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//---- PHDC " + i + " Class Requests -----//");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "// GET DATA STATUS REQUEST");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "USB_REQ_TYPE_INPUT | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "USB_PHDC_GET_STATUS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x00,0x00,                                 // always zero");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "PHDC" + i + "_DATA_INTERFACE,0x00,                 // PHDC interface is " +i);
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x02,0x00,                                 // Size of Structure (data length)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xff,&USBPHDC_GetDataStatusReq,");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + "},");
                    oStream.newLine();
                    
                } 
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_MSC_DESC_INFO>
     */
    class MUDTCG_MSC_DESC_INFO implements Callback {
        @Override
        public void execute() {
            try{
                if (numOfMscInterfaces > 0){
                    oStream.write("/*");
                    oStream.newLine();
                    oStream.write("* Every application using this MSC API must define an instance of this");
                    oStream.newLine();
                    oStream.write("* structure.  It establishes compile-time information about the storage media.");
                    oStream.newLine();
                    oStream.write("*/");
                    oStream.newLine();
                    oStream.write("struct config_struct USBMSC_config = {");
                    oStream.newLine();
                    oStream.write(softTab + "{");
                    oStream.newLine();
                    
                    
                    // Tool currently allows only one MSCInterface
                    MSCInterface430 mscIntf = mscInterface430List.get(0);
                    for (int i = 0; i < mscIntf.getNumOfLuns(); i++){
                        oStream.write(softTab + softTab + "{ // LUN" + i);
                        oStream.newLine();
                        
                        MSCInterface430.LogicalUnit lun = mscIntf.getLunMap().get(i);
                        
                        oStream.write(softTab + softTab + softTab + "0x" + Integer.toHexString(i) + ", // The number of this LUN.");
                        oStream.newLine();
                        
                        if((mscIntf.cdRomMode) && (i==0)){
                            oStream.write(softTab + softTab + softTab + "0x05, // PDT (Peripheral Device Type).");
                            oStream.newLine();
                        }else{
                            oStream.write(softTab + softTab + softTab + "0x00, // PDT (Peripheral Device Type).");
                            oStream.newLine();
                        }
                        
                        if(lun.isRemovableMedia()){
                            oStream.write(softTab + softTab + softTab + "0x80, // 0x00 = media not removable, 0x80 = media removable");
                            oStream.newLine();
                        }else{
                            oStream.write(softTab + softTab + softTab + "0x00, // 0x00 = media not removable, 0x80 = media removable");
                            oStream.newLine();
                        }
                        
                        oStream.write(softTab + softTab + softTab + "\"" + lun.getT10Vendor() + "\", // Vendor ID. Has no impact on most OSes.");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "\"" + lun.getT10Product() + "\", // Product ID. Has no impact on most OSes.");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "\"" + lun.getT10Revision() + "\", // Revision string. Has no impact on most OSes.");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "},");
                        oStream.newLine(); 
                    }
                    oStream.write(softTab +"}");
                    oStream.newLine();
                    oStream.write("};");
                    oStream.newLine();
                }
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_C_ABROMDEVICEDESC_STRUCT>
     */
    class MUDTCG_DESC_C_ABROMDEVICEDESC_STRUCT implements Callback {
        @Override
        public void execute() {
            try{
                oStream.write(softTab + "SIZEOF_DEVICE_DESCRIPTOR,               // Length of this descriptor");
                oStream.newLine();
                oStream.write(softTab + "DESC_TYPE_DEVICE,                       // Type code of this descriptor");
                oStream.newLine();          
                oStream.write(softTab + String.format("%0#4x, %0#4x,                             // Release of USB spec",(bcdUSB & 0xff), (bcdUSB >> 8)));
                oStream.newLine();
                oStream.write(softTab + String.format("%0#4x,                                   // Device's base class code", bDeviceClass));
                oStream.newLine();
                oStream.write(softTab + String.format("%0#4x,                                   // Device's sub class code",bDeviceSubClass));
                oStream.newLine();
                oStream.write(softTab + String.format("%0#4x,                                   // Device's protocol type code",bDeviceProtocol));
                oStream.newLine();
                oStream.write(softTab + "EP0_PACKET_SIZE,                        // End point 0's packet size");
                oStream.newLine();
                oStream.write(softTab + "USB_VID&0xFF, USB_VID>>8,               // Vendor ID for device, TI=0x0451");
                oStream.newLine();
                oStream.write(softTab + "                                        // You can order your own VID at www.usb.org\"");
                oStream.newLine();
                oStream.write(softTab + "USB_PID&0xFF, USB_PID>>8,               // Product ID for device,");
                oStream.newLine();
                oStream.write(softTab + "                                        // this ID is to only with this example");
                oStream.newLine();
                oStream.write(softTab + "VER_FW_L, VER_FW_H,                     // Revision level of device");
                oStream.newLine();
                oStream.write(softTab + String.format("%d,                                      // Index of manufacturer name string desc",iManufacturer));
                oStream.newLine();
                oStream.write(softTab + String.format("%d,                                      // Index of product name string desc",iProduct));
                oStream.newLine();
                oStream.write(softTab + "USB_STR_INDEX_SERNUM,                   // Index of serial number string desc");
                oStream.newLine();
                oStream.write(softTab + String.format("%d                                       //  Number of configurations supported%n",bNumConfigurations));
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }        
     
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_MSC_STRUCT>
     */
    class MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_MSC_STRUCT implements Callback {
        @Override
        public void execute() {
            try{
                if (0 != numOfMscInterfaces){
                    oStream.write(softTab + "/******************************************************* start of MSC*************************************/");
                    oStream.newLine();
                }
                for (int i = 0; i < numOfMscInterfaces; i++){
                    if (0 == i)
                    {
                        oStream.write(softTab + "{");
                        oStream.newLine();
                    }
                    oStream.write(softTab + " /*start MSC[" + i + "] Here */");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "{");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//-------- Descriptor for MSC class device -------------------------------------");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "// INTERFACE DESCRIPTOR (9 bytes) ");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "SIZEOF_INTERFACE_DESCRIPTOR,                // bLength");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "DESC_TYPE_INTERFACE,                        // bDescriptorType: 4 ");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "MSC" + i + "_DATA_INTERFACE,                        // bInterfaceNumber");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x00,                                       // bAlternateSetting");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x02,                                       // bNumEndpoints");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x08,                                       // bInterfaceClass: 3 = MSC Device");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x06,                                       // bInterfaceSubClass:");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x50,                                       // bInterfaceProtocol:");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "INTF_STRING_INDEX + " + intfindex + ",                      // iInterface:1");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "SIZEOF_ENDPOINT_DESCRIPTOR,                 // bLength");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "DESC_TYPE_ENDPOINT,                         // bDescriptorType");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "MSC" + i + "_INEP_ADDR,                             // bEndpointAddress; bit7=1 for IN, bits 3-0=1 for ep1");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "EP_DESC_ATTR_TYPE_BULK,                     // bmAttributes, interrupt transfers");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x40, 0x00,                                 // wMaxPacketSize, 64 bytes");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0X01,                                       // bInterval, ms");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "SIZEOF_ENDPOINT_DESCRIPTOR,                 // bLength");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "DESC_TYPE_ENDPOINT,                         // bDescriptorType");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "MSC" + i + "_OUTEP_ADDR,                            // bEndpointAddress; bit7=1 for IN, bits 3-0=1 for ep1");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "EP_DESC_ATTR_TYPE_BULK,                     // bmAttributes, interrupt transfers");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x40, 0x00,                                 // wMaxPacketSize, 64 bytes");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x01,                                       // bInterval, ms");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "/* end of MSC[" + i + "]*/");
                    oStream.newLine();
                    
                    if (i != (numOfMscInterfaces - 1)){
                       oStream.write(softTab + softTab + "},");
                       oStream.newLine(); 
                    }else{
                       oStream.write(softTab + softTab + "}");
                       oStream.newLine();
                    }
                    
                    if ((i + 1) == (numOfMscInterfaces)
                    && (numOfCdcInterfaces > 0 || numOfHidInterfaces > 0 || numOfPhdcInterfaces > 0)){
                        oStream.write(softTab + "},");
                        oStream.newLine();
                        oStream.write(softTab + "/******************************************************* end of MSC**************************************/");
                        oStream.newLine();
                    }else{
                        oStream.write(softTab + "}");
                        oStream.newLine();
                        oStream.write(softTab + "/******************************************************* end of MSC**************************************/");
                        oStream.newLine();
                    }
                    intfindex++;
                }
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_CDC_STRUCT>
     */
    class MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_CDC_STRUCT implements Callback {
        @Override
        public void execute() {
            try{
                for (int i = 0; i < numOfCdcInterfaces; i++){
                    if (0 == i){
                        oStream.write(softTab + "/******************************************************* start of CDC*************************************/");
                        oStream.newLine();
                        oStream.newLine();
                        oStream.write(softTab + "{");
                        oStream.newLine();
                    }
                    oStream.write(softTab + softTab + "/* start CDC[" + i + "] */");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "{");
                    oStream.newLine();
                    if ((1 == numOfCdcInterfaces) && (0 == numOfHidInterfaces)
                    && (0 == numOfMscInterfaces) && (0 == numOfPhdcInterfaces)){
                        oStream.newLine();
                    }else{
                        oStream.newLine();
                        oStream.write(softTab + softTab + "//Interface Association Descriptor");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "0x08,                              // bLength");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "DESC_TYPE_IAD,                     // bDescriptorType = 11");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "CDC" + i + "_COMM_INTERFACE,               // bFirstInterface");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "0x02,                              // bInterfaceCount");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "0x02,                              // bFunctionClass (Communication Class)");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "0x02,                              // bFunctionSubClass (Abstract Control Model)");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "0x01,                              // bFunctionProcotol (V.25ter, Common AT commands)");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "INTF_STRING_INDEX + " + intfindex + ",             // iInterface");
                        oStream.newLine();
                        oStream.newLine();
                    }
                    oStream.write(softTab + softTab + "//INTERFACE DESCRIPTOR (9 bytes)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x09,                              // bLength: Interface Descriptor size");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "DESC_TYPE_INTERFACE,               // bDescriptorType: Interface");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "CDC" + i + "_COMM_INTERFACE,               // bInterfaceNumber");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x00,                              // bAlternateSetting: Alternate setting");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x01,                              // bNumEndpoints: Three endpoints used");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x02,                              // bInterfaceClass: Communication Interface Class");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x02,                              // bInterfaceSubClass: Abstract Control Model");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x01,                              // bInterfaceProtocol: Common AT commands");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "INTF_STRING_INDEX + " + intfindex + ",             // iInterface:");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//Header Functional Descriptor");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x05,                                // bLength: Endpoint Descriptor size");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x24,                                // bDescriptorType: CS_INTERFACE");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x00,                                // bDescriptorSubtype: Header Func Desc");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x10,                                // bcdCDC: spec release number");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x01,");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//Call Managment Functional Descriptor");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x05,                                // bFunctionLength");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x24,                                // bDescriptorType: CS_INTERFACE");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x01,                                // bDescriptorSubtype: Call Management Func Desc");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x00,                                // bmCapabilities: D0+D1");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "CDC" + i + "_DATA_INTERFACE,                // bDataInterface: 0");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//ACM Functional Descriptor");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x04,                                // bFunctionLength");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x24,                                // bDescriptorType: CS_INTERFACE");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x02,                                // bDescriptorSubtype: Abstract Control Management desc");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x02,                                // bmCapabilities");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "// Union Functional Descriptor");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x05,                               // Size, in bytes");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x24,                               // bDescriptorType: CS_INTERFACE");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x06,                                // bDescriptorSubtype: Union Functional Desc");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "CDC" + i + "_COMM_INTERFACE,                // bMasterInterface -- the controlling intf for the union");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "CDC" + i + "_DATA_INTERFACE,                // bSlaveInterface -- the controlled intf for the union");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//EndPoint Descriptor for Interrupt endpoint");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "SIZEOF_ENDPOINT_DESCRIPTOR,         // bLength: Endpoint Descriptor size");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "DESC_TYPE_ENDPOINT,                 // bDescriptorType: Endpoint");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "CDC" + i + "_INTEP_ADDR,                    // bEndpointAddress: (IN2)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "EP_DESC_ATTR_TYPE_INT,                // bmAttributes: Interrupt");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x40, 0x00,                         // wMaxPacketSize, 64 bytes");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0xFF,                                // bInterval");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//DATA INTERFACE DESCRIPTOR (9 bytes)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x09,                                // bLength: Interface Descriptor size");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "DESC_TYPE_INTERFACE,                // bDescriptorType: Interface");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "CDC" + i + "_DATA_INTERFACE,                // bInterfaceNumber");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x00,                               // bAlternateSetting: Alternate setting");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x02,                               // bNumEndpoints: Three endpoints used");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x0A,                               // bInterfaceClass: Data Interface Class");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x00,                               // bInterfaceSubClass:");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x00,                               // bInterfaceProtocol: No class specific protocol required");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x00,                                // iInterface:");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//EndPoint Descriptor for Output endpoint");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "SIZEOF_ENDPOINT_DESCRIPTOR,         // bLength: Endpoint Descriptor size");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "DESC_TYPE_ENDPOINT,                    // bDescriptorType: Endpoint");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "CDC" + i + "_OUTEP_ADDR,                    // bEndpointAddress: (OUT3)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "EP_DESC_ATTR_TYPE_BULK,                // bmAttributes: Bulk");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x40, 0x00,                         // wMaxPacketSize, 64 bytes");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0xFF,                                 // bInterval: ignored for Bulk transfer");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//EndPoint Descriptor for Input endpoint");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "SIZEOF_ENDPOINT_DESCRIPTOR,         // bLength: Endpoint Descriptor size");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "DESC_TYPE_ENDPOINT,                    // bDescriptorType: Endpoint");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "CDC" + i + "_INEP_ADDR,                        // bEndpointAddress: (IN3)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "EP_DESC_ATTR_TYPE_BULK,                // bmAttributes: Bulk");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x40, 0x00,                         // wMaxPacketSize, 64 bytes");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0xFF                                // bInterval: ignored for bulk transfer");
                    oStream.newLine();
                    
                    if (i != (numOfCdcInterfaces - 1)){
                        oStream.write(softTab + softTab + "},");
                        oStream.newLine();
                        oStream.newLine();
                    }else{
                        oStream.write(softTab + softTab + "}");
                        oStream.newLine();
                        oStream.newLine();
                    }
                                       
                    oStream.write(softTab + softTab + "/* end CDC[" + i + "]*/");
                    oStream.newLine();
                    
                    if ((i + 1) == numOfCdcInterfaces){
                        if (numOfHidInterfaces != 0 || numOfPhdcInterfaces != 0){
                            oStream.write(softTab + "},");
                            oStream.write(softTab + "/******************************************************* end of CDC**************************************/");
                            oStream.newLine();
                            oStream.newLine();
                        }else{
                            oStream.write(softTab + "}");
                            oStream.write(softTab + "/******************************************************* end of CDC**************************************/");
                            oStream.newLine();
                            oStream.newLine();
                        }
                    }
                    intfindex++;
                }              
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_HID_STRUCT>
     */
    class MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_HID_STRUCT implements Callback {
        @Override
        public void execute() {
            try{
               if (0 != numOfHidInterfaces){
                    oStream.write(softTab + "/******************************************************* start of HID*************************************/");
                    oStream.newLine(); 
                    oStream.write(softTab + "{");
                    oStream.newLine();
               }
                
               for (int i = 0; i < numOfHidInterfaces; i++){
                    HIDInterface430 hidIntf = hidInterface430List.get(i);
                    oStream.write(softTab + "/*start HID[" + i + "] Here */");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "{");
                    oStream.newLine();
                    
                    oStream.write(softTab + softTab + softTab + "//-------- Descriptor for HID class device -------------------------------------");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "// INTERFACE DESCRIPTOR (9 bytes)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "SIZEOF_INTERFACE_DESCRIPTOR,        // bLength");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "DESC_TYPE_INTERFACE,                // bDescriptorType: 4");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "HID" + i + "_REPORT_INTERFACE,              // bInterfaceNumber");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x00,                               // bAlternateSetting");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "2,                                  // bNumEndpoints");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x03,                               // bInterfaceClass: 3 = HID Device");
                    oStream.newLine();
                    
                    if(hidIntf.getHidReportType() == HIDInterface430.HID_REPORT_KEYBOARD){
                        oStream.write(softTab + softTab + softTab + "1,                                  // bInterfaceSubClass:");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "1,                                  // bInterfaceProtocol:");
                        oStream.newLine();
                    }else if(hidIntf.getHidReportType() == HIDInterface430.HID_REPORT_MOUSE){
                        oStream.write(softTab + softTab + softTab + "1,                                  // bInterfaceSubClass:");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "2,                                  // bInterfaceProtocol:");
                        oStream.newLine();
                    }else {
                        oStream.write(softTab + softTab + softTab + "0,                                  // bInterfaceSubClass:");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab + "0,                                  // bInterfaceProtocol:");
                        oStream.newLine();
                    }

                    oStream.write(softTab + softTab + softTab + "INTF_STRING_INDEX + " + intfindex + ",              // iInterface:1");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "// HID DESCRIPTOR (9 bytes)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x09,                                 // bLength of HID descriptor");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x21,                                 // HID Descriptor Type: 0x21");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x01,0x01,                            // HID Revision number 1.01");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x00,                                // Target country, nothing specified (00h)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x01,                                // Number of HID classes to follow");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x22,                                // Report descriptor type");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "(report_desc_size_HID" + i + " & 0x0ff),  // Total length of report descriptor");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "(report_desc_size_HID" + i + "  >> 8),");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "SIZEOF_ENDPOINT_DESCRIPTOR,         // bLength");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "DESC_TYPE_ENDPOINT,                 // bDescriptorType");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "HID" + i + "_INEP_ADDR,                     // bEndpointAddress; bit7=1 for IN, bits 3-0=1 for ep1");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "EP_DESC_ATTR_TYPE_INT,              // bmAttributes, interrupt transfers");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x40, 0x00,                         // wMaxPacketSize, 64 bytes");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + hidIntf.getPollingInterv() + ",                                  // bInterval, ms");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "SIZEOF_ENDPOINT_DESCRIPTOR,         // bLength");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "DESC_TYPE_ENDPOINT,                 // bDescriptorType");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "HID" + i + "_OUTEP_ADDR,                    // bEndpointAddress; bit7=1 for IN, bits 3-0=1 for ep1");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "EP_DESC_ATTR_TYPE_INT,              // bmAttributes, interrupt transfers");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x40, 0x00,                         // wMaxPacketSize, 64 bytes");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + hidIntf.getPollingInterv() + ",                                  // bInterval, ms");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "/* end of HID[" + i + "]*/");
                    oStream.newLine();
                    
                    if (i != (numOfHidInterfaces - 1)){
                        oStream.write(softTab + softTab + "},");
                        oStream.newLine();
                        oStream.newLine();
                    }else{
                        oStream.write(softTab + softTab + "}");
                        oStream.newLine();
                        oStream.newLine();
                    }
                    
                    if ((i + 1) == (numOfHidInterfaces))
                    {
                        if (numOfPhdcInterfaces == 0)
                        {
                            oStream.write(softTab + "}");
                            oStream.write(softTab + "/******************************************************* end of HID**************************************/");
                            oStream.newLine();
                            oStream.newLine();
                        }else{
                            oStream.write(softTab + "},");
                            oStream.write(softTab + "/******************************************************* end of HID**************************************/");
                            oStream.newLine();
                            oStream.newLine();
                        }
                    }
                    intfindex++;
               }                
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_PHDC_STRUCT>
     */
    class MUDTCG_DESC_C_ABROMCONFIGDESCGROUP_PHDC_STRUCT implements Callback {
        @Override
        public void execute() {
            try{
                for (int i = 0; i < numOfPhdcInterfaces; i++){
                    if (0 == i){
                        oStream.write(softTab + "{");
                        oStream.newLine();
                    }
                    oStream.write(softTab + "/*start PHDC[" + i + "] Here */");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "{");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "//-------- Descriptor for PHDC class device -------------------------------------");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "// INTERFACE DESCRIPTOR (9 bytes)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "SIZEOF_INTERFACE_DESCRIPTOR,// bLength: Interface Descriptor size");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "DESC_TYPE_INTERFACE,        // bDescriptorType: Interface");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "PHDC" + i + "_DATA_INTERFACE,      // bInterfaceNumber");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x00,                       // bAlternateSetting: Alternate setting");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "PHDC_ENDPOINTS_NUMBER,      // bNumEndpoints (depends on the PHDC_USE_INT_ENDPOINT macro)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x0F,                       // bInterfaceClass: PHDC");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x00,                       // bInterfaceSubClass: Abstract Control Model");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x00,                       // bInterfaceProtocol: BOT");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "INTF_STRING_INDEX + " + intfindex + ",            // iInterface:");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "//PHDC Class Function descriptor");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x04,                        // bLength: Endpoint Descriptor size");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x20,                        // bDescriptorType: PHDC_CLASSFUNCTION_DESCRIPTOR");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x02,                        // bPHDCDataCode: PHDC_11073_20601");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x00,                        // bmCapability: spec release number");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "//PHDC Function Extension descriptor");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x06,                        // bLength");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x30,                        // bDescriptorType: CS_INTERFACE");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x00,                        // bReserved: Call Management Func Desc");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x01,                        // bNumDevSpecs: D0+D1");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "#if defined(IPORT_IEEE10415)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + softTab + " PUTWORD(4111),              // wDevSpecializations, weight scale");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "#elif defined(IPORT_IEEE10408)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + softTab + "PUTWORD(4104),              // wDevSpecializations, thermometer");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "#elif defined(IPORT_IEEE10407)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + softTab + "PUTWORD(4103),              // wDevSpecializations, blood pressure");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "#elif defined(IPORT_IEEE10417)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + softTab + "PUTWORD(4113),              // wDevSpecializations, glucosemeter");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "#elif defined(IPORT_IEEE10404)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + softTab + "PUTWORD(4100),              // wDevSpecializations, pulse oximeter");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "#endif");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "#ifdef PHDC_USE_INT_ENDPOINT");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "// ENDPOINT #1 INPUT DESCRIPTOR, (7 bytes)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "SIZEOF_ENDPOINT_DESCRIPTOR,     // bLength: Endpoint Descriptor size");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "DESC_TYPE_ENDPOINT,                // bDescriptorType: Endpoint");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "PHDC" + i + "_INTEP_ADDR,                // bEndpointAddress:");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "EP_DESC_ATTR_TYPE_INT,            // bmAttributes: Interrupt");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "PUTWORD(MAX_PACKET_SIZE),       // wMaxPacketSize, 64 bytes");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0xFF,                             // bInterval");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "//PHDC QoS");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x04,                           // bLength");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x21,                           // bDescriptorType: PHDC_QOS_DESCRIPTOR");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x01,                           // bQoSEncodingVersion");
                    oStream.newLine();
                    oStream.write(softTab + softTab + softTab + "0x01,                           // bmLatencyReliability: Low.Good");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "#endif");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "// ENDPOINT #2 OUTPUT DESCRIPTOR, (7 bytes)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "SIZEOF_ENDPOINT_DESCRIPTOR,     // bLength: Endpoint Descriptor size");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "DESC_TYPE_ENDPOINT,                // bDescriptorType: Endpoint");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "PHDC" + i + "_OUTEP_ADDR,                 // bEndpointAddress: (OUT3)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "EP_DESC_ATTR_TYPE_BULK,            // bmAttributes: Bulk");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "PUTWORD(MAX_PACKET_SIZE),       // wMaxPacketSize, 64 bytes");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0xFF,                             // bInterval: ignored for Bulk transfer");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "// PHDC QoS");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x04,                           // bLength");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x21,                           // bDescriptorType: PHDC_QOS_DESCRIPTOR");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x01,                           // bQoSEncodingVersion");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x08,                           // bmLatencyReliability: Medium.Best");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "// PHDC Meta Data");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x04,                           // bLength");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x22,                           // bDescriptorType: PHDC_METADATA_DESCRIPTOR");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0xCC,                           // bOpaqueData");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0xDD,");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "// ENDPOINT #3 INPUT DESCRIPTOR, (7 bytes)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "SIZEOF_ENDPOINT_DESCRIPTOR,     // bLength: Endpoint Descriptor size");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "DESC_TYPE_ENDPOINT,                // bDescriptorType: Endpoint");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "PHDC" + i + "_INEP_ADDR,                    // bEndpointAddress: (IN3)");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "EP_DESC_ATTR_TYPE_BULK,            // bmAttributes: Bulk");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "PUTWORD(MAX_PACKET_SIZE),       // wMaxPacketSize, 64 bytes");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0xFF,                            // bInterval: ignored for bulk transfer");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//PHDC QoS");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x04,                           // bLength");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x21,                           // bDescriptorType: PHDC_QOS_DESCRIPTOR");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x01,                           // bQoSEncodingVersion");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x08,                           // bmLatencyReliability: Medium.Best");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//PHDC Meta Data");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x07,                           // bLength");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x22,                           // bDescriptorType: PHDC_METADATA_DESCRIPTOR");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0xAB,                           // bOpaqueData");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0xCD,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0xEF,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x01,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0x02,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "/* end of PHDC[" + i + "]*/");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "");
                    oStream.newLine();
                    
                    
                    if (i != (numOfPhdcInterfaces - 1)){
                        oStream.write(softTab + softTab + "},");
                        oStream.newLine();
                    }else{
                        oStream.write(softTab + softTab + "}");
                        oStream.newLine();
                    }
                    
                    if ((i + 1) == (numOfPhdcInterfaces)){
                        oStream.write(softTab + "}");
                        oStream.newLine();
                        oStream.write(softTab + "/******************************************************* end of PHDC**************************************/");
                        oStream.newLine();
                    }
                          
                    intfindex++;
                    
                }
        
                
                oStream.write(softTab + "");
                oStream.newLine();
                oStream.write(softTab + softTab + "");
                oStream.newLine();
                oStream.write(softTab + softTab + softTab + "");
                oStream.newLine();
                               
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_C_ABROMSTRINGDESC>
     */
    class MUDTCG_DESC_C_ABROMSTRINGDESC implements Callback {

        @Override
        public void execute() {
            try {
                int stringIndexNum = 0;
                oStream.write(softTab + "// String index" + stringIndexNum++ + ", language support");
                oStream.newLine();
                oStream.newLine();
                oStream.write(stringLanguageDescriptor.generateLanguageOutput().toString());
                oStream.newLine();
                oStream.newLine();
                oStream.write(softTab + "// String index" + stringIndexNum++ + ", Manufacturer");
                oStream.newLine();
                oStream.newLine();
                oStream.write(stringDescriptorVendorString.generateOutput(false).toString());
                oStream.newLine();
                oStream.newLine();
                oStream.write(softTab + "// String index" + stringIndexNum++ + ", Product");
                oStream.newLine();
                oStream.newLine();
                oStream.write(stringDescriptorProductString.generateOutput(false).toString());
                oStream.newLine();
                oStream.newLine();
                oStream.write(softTab + "// String index" + stringIndexNum++ + ", Serial Number");
                oStream.newLine();
                oStream.newLine();
                oStream.write(stringDescriptor3.generateOutput(false).toString());
                oStream.newLine();
                oStream.newLine();
                oStream.write(softTab + "// String index" + stringIndexNum++ + ", Configuration String");
                oStream.newLine();
                oStream.newLine();
                oStream.write(stringDescriptorConfiguration.generateOutput(false).toString());
                oStream.newLine();
                oStream.newLine();
                
                StringBuilder temp = new StringBuilder();
                String newLine = System.getProperty("line.separator");
                for (int i = 0; i < numOfMscInterfaces; i++){
                    MSCInterface430 intf = mscInterface430List.get(i);
                    StringDescriptor intfDesc = new StringDescriptor((intf.getInterfString()));
                    
                    temp.append(newLine);
                    temp.append(softTab + "// String index" + stringIndexNum++ + ", Interface String");
                    temp.append(newLine);
                    temp.append(intfDesc.generateOutput(false).toString());
                    temp.append(newLine);
                }
                
                for (int i = 0; i < numOfCdcInterfaces; i++){
                    CDCInterface430 intf = cDCInterface430List.get(i);
                    StringDescriptor intfDesc = new StringDescriptor((intf.getInterfString()));
                    
                    temp.append(newLine);
                    temp.append(softTab + "// String index" + stringIndexNum++ + ", Interface String");
                    temp.append(newLine);
                    temp.append(intfDesc.generateOutput(false).toString());
                    temp.append(newLine);
                }
                
                for (int i = 0; i < numOfHidInterfaces; i++){
                    HIDInterface430 intf = hidInterface430List.get(i);
                    StringDescriptor intfDesc = new StringDescriptor((intf.getInterfString()));
                    
                    temp.append(newLine);
                    temp.append(softTab + "// String index" + stringIndexNum++ + ", Interface String");
                    temp.append(newLine);
                    temp.append(intfDesc.generateOutput(false).toString());
                    temp.append(newLine);
                }
                
                for (int i = 0; i < numOfPhdcInterfaces; i++){
                    PHDCInterface430 intf = phdcInterface430List.get(i);
                    StringDescriptor intfDesc = new StringDescriptor((intf.getInterfString()));
                    
                    temp.append(newLine);
                    temp.append(softTab + "// String index" + stringIndexNum++ + ", Interface String");
                    temp.append(newLine);
                    temp.append(intfDesc.generateOutput(false).toString());
                    temp.append(newLine);
                }
                
                if (',' == temp.charAt(temp.length() - 3)){
                    temp.deleteCharAt(temp.length() - 3);
                }
                
                oStream.write(temp.toString());

            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_C_HID_REPORT_DESC>
     */
    class MUDTCG_DESC_C_HID_REPORT_DESC implements Callback {
        @Override
        public void execute() {
            try{
                for (int i = 0; i < numOfHidInterfaces; i++){
                    HIDInterface430 hidInt = hidInterface430List.get(i);
                    
                    oStream.write("uint8_t const report_desc_HID"+ i + "[]=");
                    oStream.newLine();
                    oStream.write("{");
                    oStream.newLine();
                    oStream.write(hidInt.getHidReportText());
                    oStream.newLine();
                    oStream.write("};");
                    oStream.newLine();
                }
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
            
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_C_GLOBALS_FOR_CDC_OR_HID>
     */
    class MUDTCG_DESC_C_GLOBALS_FOR_CDC_OR_HID implements Callback {
        @Override
        public void execute() {
            try{
                if(numOfHidInterfaces >0){
                    oStream.write("uint16_t const report_desc_size[HID_NUM_INTERFACES] =");
                    oStream.newLine();
                    oStream.write("{");
                    oStream.newLine();
                    for(int i=0; i<numOfHidInterfaces;i++){
                        HIDInterface430 hidIntf = hidInterface430List.get(i);
                        if(i < (numOfHidInterfaces -1)){
                            oStream.write(softTab +hidIntf.getReportDescSize() + ",");
                            oStream.newLine();
                        }else{
                            oStream.write(softTab +hidIntf.getReportDescSize() + "");
                            oStream.newLine();
                        }
                    }
                    oStream.write("};");
                    oStream.newLine();
                    oStream.write("uint8_t const report_len_input[HID_NUM_INTERFACES] =");
                    oStream.newLine();
                    oStream.write("{");
                    oStream.newLine();
                    for(int i=0; i<numOfHidInterfaces;i++){
                        HIDInterface430 hidIntf = hidInterface430List.get(i);
                        if(i < (numOfHidInterfaces -1)){
                            oStream.write(softTab +hidIntf.getReportLenInput() + ",");
                            oStream.newLine();
                        }else{
                            oStream.write(softTab +hidIntf.getReportLenInput() + "");
                            oStream.newLine();
                        }
                    }
                    oStream.write("};");
                    oStream.newLine();
                  }
                
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_C_HID_REPORT_DESC_POINTERS>
     */
    class MUDTCG_DESC_C_HID_REPORT_DESC_POINTERS implements Callback {

        @Override
        public void execute() {
            try {
                if (numOfHidInterfaces > 0){
                    oStream.write("const uint8_t* report_desc[HID_NUM_INTERFACES] =");
                    oStream.newLine();
                    oStream.write("{");
                    oStream.newLine();
                    for (int i = 0; i < numOfHidInterfaces;  i++){
                        if (i < numOfHidInterfaces - 1){
                            oStream.write(softTab + "(uint8_t*)&report_desc_HID" + i + ",");
                            oStream.newLine();
                        }else{
                            oStream.write(softTab + "(uint8_t*)&report_desc_HID" + i);
                            oStream.newLine();
                        }
                    }

                    oStream.write("};");
                    oStream.newLine();
                    
                }

            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_C_HID_USBHANDLE>
     */
    class MUDTCG_DESC_C_HID_USBHANDLE implements Callback {
        @Override
        public void execute() {
            try{
               int i = 0;
               int j = 0;
               
               for (i = 0; i < numOfMscInterfaces; i++){
                   oStream.write(softTab + "{");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "MSC" + i + "_INEP_ADDR,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "MSC" + i + "_OUTEP_ADDR,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + i + ", ");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "MSC_CLASS,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "0,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "0,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "OEP" + (i + 1) + "_X_BUFFER_ADDRESS,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "OEP" + (i + 1) + "_Y_BUFFER_ADDRESS,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "IEP" + (i + 1) + "_X_BUFFER_ADDRESS,");
                   oStream.newLine();
                   oStream.write(softTab + softTab + "IEP" + (i + 1) + "_Y_BUFFER_ADDRESS");
                   oStream.newLine();
                    
                   if (i == numOfMscInterfaces - 1){
                        if(numOfCdcInterfaces > 0 || numOfHidInterfaces > 0 || numOfPhdcInterfaces > 0){
                            oStream.write(softTab + "},");
                            oStream.newLine(); 
                        }else{
                            oStream.write(softTab + "}");
                            oStream.newLine(); 
                        }
                    }else{
                        oStream.write(softTab + "},");
                        oStream.newLine();
                    } 
                }
               
                for (j = 0; j < numOfCdcInterfaces; j++, i++){
                    oStream.write(softTab + "{");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "CDC" + j + "_INEP_ADDR,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "CDC" + j + "_OUTEP_ADDR,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + (i + 1 + j) + ",");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "CDC_CLASS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "IEP" + (i + 1 + j) + "_X_BUFFER_ADDRESS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "IEP" + (i + 1 + j) + "_Y_BUFFER_ADDRESS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "OEP" + (i + 2 + j) + "_X_BUFFER_ADDRESS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "OEP" + (i + 2 + j) + "_Y_BUFFER_ADDRESS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "IEP" + (i + 2 + j) + "_X_BUFFER_ADDRESS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "IEP" + (i + 2 + j) + "_Y_BUFFER_ADDRESS");
                    oStream.newLine();
                    
                    if (j == numOfCdcInterfaces - 1){
                        if(numOfPhdcInterfaces > 0 || numOfHidInterfaces > 0){
                            oStream.write(softTab + "},");
                            oStream.newLine();
                        }else{
                            oStream.write(softTab + "}");
                            oStream.newLine();
                        }
                   }else{
                        oStream.write(softTab + "},");
                        oStream.newLine();
                    }
                }
                            
                             
               
                i = (numOfCdcInterfaces * 2) + numOfMscInterfaces;
                for (j = 0; j < numOfHidInterfaces; i++, j++){
                    oStream.write(softTab + "{");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "HID" + j + "_INEP_ADDR,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "HID" + j + "_OUTEP_ADDR,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + (i) + ",");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "HID_CLASS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "0,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "OEP" + (i + 1) + "_X_BUFFER_ADDRESS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "OEP" + (i + 1) + "_Y_BUFFER_ADDRESS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "IEP" + (i + 1) + "_X_BUFFER_ADDRESS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "IEP" + (i + 1) + "_Y_BUFFER_ADDRESS");
                    oStream.newLine();
                    
                    if (j == numOfHidInterfaces - 1){
                        if(numOfPhdcInterfaces > 0){
                            oStream.write(softTab + "},");
                            oStream.newLine(); 
                        }else{
                            oStream.write(softTab + "}");
                            oStream.newLine();
                        }
                    }else{
                        oStream.write(softTab + "},");
                        oStream.newLine();
                    }
                }
                
                for (j = 0; i < ((2 * numOfCdcInterfaces) + numOfHidInterfaces + 
                        numOfMscInterfaces + numOfPhdcInterfaces); i++, j++){
                    oStream.write(softTab + "{");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "PHDC" + j + "_INEP_ADDR,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "PHDC" + j + "_OUTEP_ADDR,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + (i + 1) + ",");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "PHDC_CLASS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "IEP" + (i + 1 ) + "_X_BUFFER_ADDRESS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "IEP" + (i + 1 ) + "_Y_BUFFER_ADDRESS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "OEP" + (i + 2 ) + "_X_BUFFER_ADDRESS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "OEP" + (i + 2 ) + "_Y_BUFFER_ADDRESS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "IEP" + (i + 2 ) + "_X_BUFFER_ADDRESS,");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "IEP" + (i + 2 ) + "_Y_BUFFER_ADDRESS");
                    oStream.newLine();
                    
                    if ((j == numOfPhdcInterfaces - 1)){
                        oStream.write(softTab + "}");
                        oStream.newLine();
                    } else{
                        oStream.write(softTab + "},");
                        oStream.newLine();
                    }
                    
                }
                
                
               oStream.write("");
               oStream.newLine();
               oStream.write(softTab + "");
               oStream.newLine();
               oStream.write(softTab + softTab + "");
               oStream.newLine();
               oStream.write(softTab + softTab + softTab +"");
               oStream.newLine();
            
               
               
            }catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_INCLUDES_FOR_PHDC>
     */
    class MUDTCG_DESC_H_INCLUDES_FOR_PHDC implements Callback {

        @Override
        public void execute() {
            try {
                if (numOfPhdcInterfaces > 0){
                    oStream.write("#include <EAS/Common/i3e_port.h>");
                    oStream.newLine();
                }  
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_COMPOSITE_DEFINES>
     */
    class MUDTCG_DESC_H_COMPOSITE_DEFINES implements Callback {

        @Override
        public void execute() {
            try {
                if (0 != numOfCdcInterfaces){
                    oStream.write("#define _CDC_          // Needed for CDC inteface");
                    oStream.newLine();
                }
                if (0 != numOfHidInterfaces){
                    oStream.write("#define _HID_          // Needed for HID interface");
                    oStream.newLine();
                }
                if (0 != numOfPhdcInterfaces){
                    oStream.write("#define _PHDC_          // Needed for PHDC interface");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write("/* Define specializations for descriptor if not defined by i3e stack */");
                    oStream.newLine();
                    oStream.write("#ifndef I3E_PORT_H");
                    oStream.newLine();
                    oStream.write("#define IPORT_IEEE10415         // weight scale");
                    oStream.newLine();
                    oStream.write(softTab + "//#define IPORT_IEEE10408       // thermometer");
                    oStream.newLine();
                    oStream.write(softTab + "//#define IPORT_IEEE10407       // blood pressure");
                    oStream.newLine();
                    oStream.write(softTab + "//#define IPORT_IEEE10417       // glucometer");
                    oStream.newLine();
                    oStream.write(softTab + "//#define IPORT_IEEE10404       // pulse oximeter");
                    oStream.newLine();
                    oStream.write("#endif");
                    oStream.newLine();
                    oStream.write("/**");
                    oStream.newLine();
                    oStream.write("* Enables (uncommented)/disables (commented) usage of the INTERRPUT ENDPOINT");
                    oStream.newLine();
                    oStream.write("*/");
                    oStream.newLine();
                    oStream.write("//#define PHDC_USE_INT_ENDPOINT");
                    oStream.newLine(); 
                    oStream.newLine();
                } 
                
                if (0 != numOfMscInterfaces){
                    oStream.write("#define _MSC_          // Needed for MSC interface");
                    oStream.newLine();
                    
                    if(mscInterface430List.get(0).isCdRomMode()){
                        oStream.write("#define CDROM_SUPPORT");
                        oStream.newLine();
                        oStream.newLine();
                    }
                    
                }
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_VID_PID_INFO>
     */
    class MUDTCG_DESC_H_VID_PID_INFO  implements Callback {

        @Override
        public void execute() {
            try {
                oStream.write("#define USB_VID               " + usbDevConfig.getvID() + "    // Vendor ID (VID)");
                oStream.newLine();
                oStream.write("#define USB_PID               " + usbDevConfig.getpID() + "        // Product ID (PID)");
                oStream.newLine();
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_FW_VER_INFO>
     */
    class MUDTCG_DESC_H_FW_VER_INFO implements Callback {

        @Override
        public void execute() {
            try {
                oStream.write(String.format("#define VER_FW_H              %0#4x          // Device release number, in binary-coded decimal",bcdDevice>>8));
                oStream.newLine();
                oStream.write(String.format("#define VER_FW_L              %0#4x          // Device release number, in binary-coded decimal",bcdDevice & 0xFF));
                oStream.newLine();
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_SERNUM_INFO>
     */
    class MUDTCG_DESC_H_SERNUM_INFO implements Callback {

        @Override
        public void execute() {
            try {
                oStream.write(String.format("#define USB_STR_INDEX_SERNUM  %d", iSerialNumber));
                oStream.newLine();
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_INTERFACE_INFO>
     */
    class MUDTCG_DESC_H_INTERFACE_INFO implements Callback {

        @Override
        public void execute() {
            try {
                int i = 0;
                int j=0;
                StringBuilder outepValue = new StringBuilder();
                StringBuilder inepValue = new StringBuilder();
                if ((1 == numOfCdcInterfaces) && (0 == numOfHidInterfaces && 0 == numOfMscInterfaces && 0 == numOfPhdcInterfaces)){
                    oStream.write("#define DESCRIPTOR_TOTAL_LENGTH            67            // wTotalLength, This is the sum of configuration descriptor length  + CDC descriptor length  + HID descriptor length");
                    oStream.newLine();
                }else{
                    oStream.write("#define DESCRIPTOR_TOTAL_LENGTH            " + ((66 * numOfCdcInterfaces) + (32 * numOfHidInterfaces) + (23 * numOfMscInterfaces) + (52 * numOfPhdcInterfaces) + 9) + "            // wTotalLength, This is the sum of configuration descriptor length  + CDC descriptor length  + HID descriptor length");
                    oStream.newLine();
                }
                
                oStream.write("#define USB_NUM_INTERFACES                  " + (2 * numOfCdcInterfaces + numOfHidInterfaces + numOfMscInterfaces + numOfPhdcInterfaces) + "            //Number of implemented interfaces.");
                oStream.newLine();
                oStream.newLine();
                
                outepValue.append("#define USB_OUTEP_INT_EN BIT0 ");
                inepValue.append("#define USB_INEP_INT_EN BIT0 ");
                
                if(numOfMscInterfaces>0){
                    j = 0;
                    for(MSCInterface430 mscIntf: mscInterface430List){
                        oStream.write("#define MSC" + j + "_DATA_INTERFACE                " + i + "              // Data interface number of MSC" + j);
                        oStream.newLine();
                        oStream.write("#define MSC" + j + "_OUTEP_ADDR                    0x0" + (i + 1) + "              // Output Endpoint number of MSC" + j);
                        oStream.newLine();
                        outepValue.append("| BIT" + (i + 1) + " ");
                        inepValue.append("| BIT" + (i + 1) + " ");
                        oStream.write("#define MSC" + j + "_INEP_ADDR                     0x" + (i + 81) + "              // Input Endpoint number of MSC" + j);
                        oStream.newLine();
                        oStream.newLine();
                        i++;
                        j++;
                    }
                }
                
                if(numOfCdcInterfaces >0 ){
                    j = 0;
                    for(CDCInterface430 cdcIntf: cDCInterface430List){
                        oStream.write("#define CDC" + j + "_COMM_INTERFACE                " + i + "              // Comm interface number of CDC" + j);
                        oStream.newLine();
                        i++;
                        oStream.write("#define CDC" + j + "_DATA_INTERFACE                " + i + "              // Data interface number of CDC" + j);
                        oStream.newLine();
                        oStream.write("#define CDC" + j + "_INTEP_ADDR                    0x" + (i + 80) + "              // Interrupt Endpoint Address of CDC" + j);
                        oStream.newLine();
                        oStream.write("#define CDC" + j + "_OUTEP_ADDR                    0x0" + (i + 1) + "              // Output Endpoint Address of CDC" + j);
                        oStream.newLine();
                        outepValue.append("| BIT" + (i + 1) + " ");
                        inepValue.append("| BIT" + i + " ");
                        inepValue.append("| BIT" + (i + 1) + " ");
                        oStream.write("#define CDC" + j + "_INEP_ADDR                    0x" + (i + 81) + "              // Input Endpoint Address of CDC" + j);
                        oStream.newLine();
                        oStream.newLine();

                        i++;
                        j++;
                    }
                }
                
                if(numOfHidInterfaces > 0){
                    j = 0;
                    for(HIDInterface430 hidIntf: hidInterface430List){
                        oStream.write("#define HID" + j + "_REPORT_INTERFACE                " + i + "              // Report interface number of HID" + j);
                        oStream.newLine();
                        oStream.write("#define HID" + j + "_OUTEP_ADDR                    0x0" + (i +1) + "           // Output Endpoint number of HID" + j);
                        oStream.newLine();
                        oStream.write("#define HID" + j + "_INEP_ADDR                     0x" + (i + 81) + "           // Input Endpoint number of HID" + j);
                        outepValue.append("| BIT" + (i + 1) + " ");
                        inepValue.append("| BIT" + (i + 1) + " ");
                        oStream.newLine();
                        oStream.newLine();
                        j++;
                        i++;
                    }
                }
                
                if(numOfPhdcInterfaces > 0){
                    j = 0;
                    for(PHDCInterface430 phdcint: phdcInterface430List){
                        oStream.write("#define PHDC" + j + "_DATA_INTERFACE                " + i + "           // Data interface number of PHDC" + j);
                        oStream.newLine();
                        oStream.write("#define PHDC" + j + "_INTEP_ADDR                    0x" + (i + 80 + 1) + "           // Interrupt Endpoint Address of PHDC" + j);
                        oStream.newLine();
                        oStream.write("#define PHDC" + j + "_OUTEP_ADDR                    0x0" + (i + 1 + 1) + "           // Output Endpoint number of PHDC" + j);
                        oStream.newLine();
                        oStream.write("#define PHDC" + j + "_INEP_ADDR                     0x" + (i + 81 + 1) + "           // Input Endpoint number of PHDC" + j);
                        oStream.newLine();
                        outepValue.append("| BIT" + (i + 1 + 1) + " ");
                        inepValue.append("| BIT" + (i + 1) + " ");
                        inepValue.append("| BIT" + (i + 1 + 1) + " ");
                        oStream.newLine();
                        oStream.newLine();
                        j++;
                        i++;
                    }
                }
                
                oStream.write("#define CDC_NUM_INTERFACES                   " + numOfCdcInterfaces + "           //  Total Number of CDCs implemented. should set to 0 if there are no CDCs implemented.");
                oStream.newLine();
                oStream.write("#define HID_NUM_INTERFACES                   " + numOfHidInterfaces + "           //  Total Number of HIDs implemented. should set to 0 if there are no HIDs implemented.");
                oStream.newLine();
                oStream.write("#define MSC_NUM_INTERFACES                   " + numOfMscInterfaces + "           //  Total Number of MSCs implemented. should set to 0 if there are no MSCs implemented.");
                oStream.newLine();
                oStream.write("#define PHDC_NUM_INTERFACES                  " + numOfPhdcInterfaces + "           //  Total Number of PHDCs implemented. should set to 0 if there are no PHDCs implemented.");
                oStream.newLine();
                
                oStream.write("// Interface numbers for the implemented CDSs and HIDs, This is to use in the Application(main.c) and in the interupt file(UsbIsr.c).");
                oStream.newLine();
                
                if(numOfMscInterfaces > 0){
                    int k=0;
                    
                    for(MSCInterface430 mscIntf : mscInterface430List){
                        oStream.write("#define MSC" + k + "_INTFNUM                "+ mscIntf.getIntfNum());
                        oStream.newLine();
                        k++;
                    }
                }
                
                if(numOfCdcInterfaces > 0){
                    int k=0;
                    
                    for(CDCInterface430 cdcIntf : cDCInterface430List){
                        oStream.write("#define CDC" + k + "_INTFNUM                " + cdcIntf.getIntfNum());
                        oStream.newLine();
                        k++;  
                    }
                }
                
                if(numOfHidInterfaces > 0){
                    int k=0;
                    
                    for(HIDInterface430 hidIntf : hidInterface430List){
                        oStream.write("#define HID" + k + "_INTFNUM                " + hidIntf.getInterfNum());
                        oStream.newLine();
                        k++; 
                    }
                    
                }
                
                if(numOfPhdcInterfaces > 0){
                    int k=0;
                    
                    for(PHDCInterface430 phdcIntf : phdcInterface430List){
                        oStream.write("#define PHDC" + k + "_INTFNUM                " + phdcIntf.getInterfNum());
                        oStream.newLine();
                        k++;
                    }
                    
                }
                
                oStream.write("#define MSC_MAX_LUN_NUMBER                   "  + bmaxlun + "           // Maximum number of LUNs supported");
                oStream.newLine();
                oStream.newLine();
                
                oStream.write("#define PUTWORD(x)      ((x)&0xFF),((x)>>8)");
                oStream.newLine();
                
                oStream.write(outepValue.toString());
                oStream.newLine();
                oStream.write(inepValue.toString());
                oStream.newLine();
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_MSP430CONFIG_LDO_XTAL>
     */
    class MUDTCG_DESC_H_MSP430CONFIG_LDO_XTAL implements Callback {

        @Override
        public void execute() {
            try {
                if(mspConfig.isDriveVBUSExt()== true){
                    oStream.write("#define USB_USE_INTERNAL_3V3LDO FALSE");
                    oStream.newLine();
                    oStream.newLine();
                }else{
                    oStream.write("#define USB_USE_INTERNAL_3V3LDO TRUE");
                    oStream.newLine();
                }
                
                if(mspConfig.isBypass()==false){
                    oStream.write("#define USB_XT2_BYPASS_MODE FALSE");
                    oStream.newLine();
                    oStream.newLine();
                }else{
                    oStream.write("#define USB_XT2_BYPASS_MODE TRUE");
                    oStream.newLine();
                }
                        
                
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_MSP430CONFIG_CLK_DMA>
     */
    class MUDTCG_DESC_H_MSP430CONFIG_CLK_DMA implements Callback {

        @Override
        public void execute() {
            try {
               oStream.write("#define USB_PLL_XT        " + crystalOscillator + "                  // Defines which XT is used by the PLL (1=XT1, 2=XT2)");
               oStream.newLine();
               oStream.write("#define USB_DISABLE_XT_SUSPEND " + crystalSuspendString + "             // If non-zero, then USB_suspend() will disable the oscillator");
               oStream.newLine();
               oStream.write("                                             // that is designated by USB_PLL_XT; if zero, USB_suspend won't");
               oStream.newLine();
               oStream.write("                                             // affect the oscillator");
               oStream.newLine();
               oStream.write("#define USB_DMA_CHAN           " + dmaTXChannelString + "   // Set to 0xFF if no DMA channel will be used 0..7 for selected DMA channel");
               oStream.newLine();

               
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_MSP430CONFIG_REM_WAKE>
     */
    class MUDTCG_DESC_H_MSP430CONFIG_REM_WAKE implements Callback {

        @Override
        public void execute() {
            try {
                oStream.write(String.format("#define USB_SUPPORT_REM_WAKE %0#4x", USB_SUPPORT_REM_WAKE));                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_MSP430CONFIG_SELF_POWER>
     */
    class MUDTCG_DESC_H_MSP430CONFIG_SELF_POWER implements Callback {

        @Override
        public void execute() {
            try {
                oStream.write(String.format("#define USB_SUPPORT_SELF_POWERED %0#4x%n", USB_SUPPORT_SELF_POWERED));
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_MSP430CONFIG_MAX_POWER>
     */
    class MUDTCG_DESC_H_MSP430CONFIG_MAX_POWER implements Callback {

        @Override
        public void execute() {
            try {
               oStream.write(String.format("#define USB_MAX_POWER %0#4x",bMaxPower)); 
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_MAX_STRING_DESCRIPTOR_INDEX>
     */
    class MUDTCG_DESC_H_MAX_STRING_DESCRIPTOR_INDEX implements Callback {

        @Override
        public void execute() {
            try {
                int indexCount = 4 + numOfCdcInterfaces + numOfHidInterfaces +
                + numOfMscInterfaces + numOfPhdcInterfaces;
                oStream.write(String.format("#define MAX_STRING_DESCRIPTOR_INDEX " + indexCount));
                oStream.newLine();
                
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_DESC_SIZE_HID>
     */
    class MUDTCG_DESC_H_DESC_SIZE_HID implements Callback {

        @Override
        public void execute() {
            try {
                for (int i = 0; i < numOfHidInterfaces; i++){
                    HIDInterface430 hidint = hidInterface430List.get(i);
                    oStream.write("#define report_desc_size_HID" + i + " " + hidint.getReportDescSize());
                    oStream.newLine();
                }
                
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_CONFIG_STRING_INDEX>
     */
    class MUDTCG_DESC_H_CONFIG_STRING_INDEX implements Callback {

        @Override
        public void execute() {
            try {
                if(usbDevConfig.isUseSerial()){
                    oStream.write("#define CONFIG_STRING_INDEX       4");
                    oStream.newLine();
                    oStream.write("#define INTF_STRING_INDEX         5");
                    oStream.newLine();
                }else{
                    oStream.write("#define CONFIG_STRING_INDEX       3");
                    oStream.newLine();
                    oStream.write("#define INTF_STRING_INDEX         4");
                    oStream.newLine();
                }
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_INTERFACE_ASSOCIATION_DESCRIPTOR>
     */
    class MUDTCG_DESC_H_INTERFACE_ASSOCIATION_DESCRIPTOR implements Callback {

        @Override
        public void execute() {
            try {
                int count = numOfCdcInterfaces + numOfHidInterfaces + numOfMscInterfaces +
                    numOfPhdcInterfaces;
            
                if (count > 1 && (numOfCdcInterfaces >= 1 || numOfPhdcInterfaces >= 1)){
                    oStream.write("//Interface Association Descriptor");
                    oStream.newLine();
                    oStream.write(softTab + "uint8_t bLength;                             // Size of this Descriptor in Bytes");
                    oStream.newLine();
                    oStream.write(softTab + "uint8_t bDescriptorType;                     // Descriptor Type (=11)");
                    oStream.newLine();
                    oStream.write(softTab + "uint8_t bFirstInterface;                     // Interface number of the first one associated with this function");
                    oStream.newLine();
                    oStream.write(softTab + "uint8_t bInterfaceCount;                     // Numver of contiguous interface associated with this function");
                    oStream.newLine();
                    oStream.write(softTab + "uint8_t bFunctionClass;                      // The class triad of this interface,");
                    oStream.newLine();
                    oStream.write(softTab + "uint8_t bFunctionSubClass;                   // usually same as the triad of the first interface");
                    oStream.newLine();
                    oStream.write(softTab + "uint8_t bFunctionProcotol;");
                    oStream.newLine();
                    oStream.write(softTab + "uint8_t iInterface;                          // Index of String Desc for this function");
                    oStream.newLine();
                }
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_DESC_H_ABROMCONFIGURATIONDESCRIPTORPHDC>
     */
    class MUDTCG_DESC_H_ABROMCONFIGURATIONDESCRIPTORPHDC implements Callback {

        @Override
        public void execute() {
            try {
                if (numOfPhdcInterfaces > 0){
                    oStream.write("struct abromConfigurationDescriptorPhdc");
                    oStream.newLine();
                    oStream.write(softTab + "{");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "uint8_t bLength;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bDescriptorType;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bInterfaceNumber;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bAlternateSetting;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bNumEndpoints;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bInterfaceClass;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bInterfaceSubClass;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bInterfaceProtocol;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t iInterface;");
                    oStream.newLine(); 
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//PHDC Class Function descriptor");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bLength_Class;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bDescriptorType_Class;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bPHDCDataCode;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bmCapability;");
                    oStream.newLine(); 
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//PHDC Function Extension descriptor");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bLength_Ext;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bDescriptorType_Ext;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bReserved;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bNumDevSpecs;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bDevSpecializationsL;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bDevSpecializationsH;");
                    oStream.newLine(); 
                    oStream.newLine();
                    oStream.write(softTab + softTab + "#ifdef PHDC_USE_INT_ENDPOINT");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "// ENDPOINT #1 INPUT DESCRIPTOR, (7 bytes)");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bLength_Intp;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bDescriptorType_intp;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bEndpointAddress_intp;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bmAttributes_intp;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bMaxPacketSize_intpL;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bMaxPacketSize_intpH;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bInterval_intp;");
                    oStream.newLine();
                    oStream.newLine();
                    oStream.write(softTab + softTab + "//PHDC QoS");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bLength_QoS;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bDescriptorType_QoS;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bQoSEncodingVersion;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bmLatencyReliability;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "#endif");
                    oStream.newLine(); 
                    oStream.newLine();
                    oStream.write(softTab + softTab + "// ENDPOINT #2 OUTPUT DESCRIPTOR, (7 bytes)");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bLength_Out;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bDescriptorType_Out;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bEndpointAddress_Out;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bmAttributes_Out;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bMaxPacketSize_OutL;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bMaxPacketSize_OutH;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bInterval_Out;");
                    oStream.newLine(); 
                    oStream.newLine();
                    oStream.write(softTab + softTab + "// PHDC QoS");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bLength_QoSout;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bDescriptorType_QoSout;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bQoSEncodingVersion_QoSout;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bmLatencyReliability_QoSout;");
                    oStream.newLine(); 
                    oStream.newLine();
                    oStream.write(softTab + softTab + "// PHDC Meta Data");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bLength_Metaout;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bDescriptorType_Metaout;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bOpaqueData_Metaout;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bOpaqueData1_Metaout;");
                    oStream.newLine(); 
                    oStream.newLine();
                    oStream.write(softTab + softTab + "// ENDPOINT #3 INPUT DESCRIPTOR, (7 bytes)");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bLength_In;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bDescriptorType_In;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bEndpointAddress_In;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bmAttributes_In;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bMaxPacketSize_InL;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bMaxPacketSize_InH;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bInterval_In;");
                    oStream.newLine(); 
                    oStream.newLine();
                    oStream.write(softTab + softTab + "// PHDC QoS");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bLength_QoSin;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bDescriptorType_QoSin;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bQoSEncodingVersion_QoSin;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bmLatencyReliability_QoSin;");
                    oStream.newLine(); 
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "//PHDC Meta Data");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bLength_Metain;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bDescriptorType_Metain;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bOpaqueData1_Metain1;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bOpaqueData2_Metain2;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bOpaqueData3_Metain3;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bOpaqueData4_Metain4;");
                    oStream.newLine(); 
                    oStream.write(softTab + softTab + "uint8_t bOpaqueData5_Metain5;");
                    oStream.newLine(); 
                    oStream.write(softTab + "};");
                    oStream.newLine(); 
                    oStream.newLine(); 
                    
                }
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_USBISR_C_INCLUDES>
     */
    class MUDTCG_USBISR_C_INCLUDES implements Callback {

        @Override
        public void execute() {
            try {
                if(numOfCdcInterfaces > 0){
                    oStream.write("#include <USB_API/USB_CDC_API/UsbCdc.h>");
                    oStream.newLine(); 
                    
                          
                }
                
                if(numOfHidInterfaces > 0){
                    oStream.write("#include <USB_API/USB_HID_API/UsbHid.h>");
                    oStream.newLine();
                    oStream.write("#include <USB_API/USB_HID_API/UsbHidReq.h>");
                    oStream.newLine();
                }
                
                if(numOfMscInterfaces > 0){
                    oStream.write("#include <USB_API/USB_MSC_API/UsbMscStateMachine.h>");
                    oStream.newLine();
                    oStream.write("#include <USB_API/USB_MSC_API/UsbMscScsi.h>");
                    oStream.newLine();
                    oStream.write("#include <USB_API/USB_MSC_API/UsbMsc.h>");
                    oStream.newLine();
                }
                
                if(numOfPhdcInterfaces > 0){
                    oStream.write("#include <USB_API/USB_PHDC_API/UsbPHDC.h>");
                    oStream.newLine();
                    if ((numOfHidInterfaces > 0)==false){
                        oStream.write("#include <USB_API/USB_HID_API/UsbHid.h>");
                        oStream.newLine();
                    }
                }
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_USBISR_C_IUSBINTERRUPTHANDLER>
     */
    class MUDTCG_USBISR_C_IUSBINTERRUPTHANDLER implements Callback {

        @Override
        public void execute() {
            try {
                if(mspConfig.isSysBIOS()){
                    oStream.write("#ifdef __TI_COMPILER_VERSION__");
                    oStream.newLine();
                    oStream.write("void iUsbInterruptHandler(void)");
                    oStream.newLine();
                    oStream.write("#elif __IAR_SYSTEMS_ICC__");
                    oStream.newLine();
                    oStream.write("void iUsbInterruptHandler(unsigned long arg)");
                    oStream.newLine();
                    oStream.write("#endif");
                    oStream.newLine();
                    oStream.write("{");
                    oStream.newLine();
                    oStream.write(softTab + "volatile uint8_t bWakeUp = FALSE;");
                    oStream.newLine();
                }else{
                   oStream.write("#if defined(__TI_COMPILER_VERSION__) || (__IAR_SYSTEMS_ICC__) ");
                    oStream.newLine();
                    oStream.write("#pragma vector=USB_UBM_VECTOR");
                    oStream.newLine();
                    oStream.write("__interrupt void iUsbInterruptHandler(void)");
                    oStream.newLine();
                    oStream.write("#elif defined(__GNUC__) && (__MSP430__)");
                    oStream.newLine();
                    oStream.write("void __attribute__ ((interrupt(USB_UBM_VECTOR))) iUsbInterruptHandler(void)");
                    oStream.newLine();
                    oStream.write("#endif");
                    oStream.newLine();
                    oStream.write("{");
                    oStream.newLine();
                    oStream.write(softTab + "uint8_t bWakeUp = FALSE;");
                    oStream.newLine(); 
                }
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_USBISR_C_USBVECINT_INPUT_ENDPOINTX>
     */
    class MUDTCG_USBISR_C_USBVECINT_INPUT_ENDPOINTX implements Callback {

        @Override
        public void execute() {
            try {
                if(interfceCount < totalinterfaceCount){
                    String name = orderedInterfaces.get(interfceCount).getClass().getSimpleName();
                    if(name.contentEquals("HIDInterface430")){
                        oStream.write(softTab + softTab + "//send saved bytes from buffer...");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "bWakeUp = HidToHostFromBuffer(HID" + hidCount + "_INTFNUM);");
                        oStream.newLine();
                        interfceCount++;
                        hidCount++;
                    }else if (name.contentEquals("MSCInterface430")){
                        oStream.write(softTab + softTab + "bWakeUp = MSCToHostFromBuffer();");
                        oStream.newLine();
                        interfceCount++;
                        mscCount++;
                    }else if(name.contentEquals("CDCInterface430")){
                        if(previousCDC == false){
                            previousCDC = true;
                        }else{
                            oStream.write(softTab + softTab + "//send saved bytes from buffer...");
                            oStream.newLine();
                            oStream.write(softTab + softTab + "bWakeUp = CdcToHostFromBuffer(CDC" + cdcCount + "_INTFNUM);");
                            oStream.newLine();
                            interfceCount++;
                            cdcCount++;
                            previousCDC = false;
                        }
                    }
                    else if(name.contentEquals("PHDCInterface430")){
                        if(previousPHDC == false){
                            previousPHDC = true;
                        }else{
                            oStream.write(softTab + softTab + "//send saved bytes from buffer...");
                            oStream.newLine();
                            oStream.write(softTab + softTab + "bWakeUp = PHDCToHostFromBuffer(PHDC" + phdcCount + "_INTFNUM);");
                            oStream.newLine();
                            interfceCount++;
                            phdcCount++;
                            previousPHDC = false;
                        }
                    }
                }
                loopCount++;
                
                if(loopCount == 8){
                    hidCount = 0;
                    cdcCount = 0;
                    mscCount = 0;
                    phdcCount = 0;
                    interfceCount = 0;
                }
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_USBISR_C_USBVECINT_OUTPUT_ENDPOINTX>
     */
    class MUDTCG_USBISR_C_USBVECINT_OUTPUT_ENDPOINTX implements Callback {

        @Override
        public void execute() {
            try {
                if(interfceCount < totalinterfaceCount){
                    String name = orderedInterfaces.get(interfceCount).getClass().getSimpleName();
                    if(name.contentEquals("HIDInterface430")){
                        oStream.write(softTab + softTab + "//call callback function if no receive operation is underway");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "if (!HidIsReceiveInProgress(HID" + hidCount + "_INTFNUM))");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "{");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab  + "if (wUsbEventMask & USB_DATA_RECEIVED_EVENT)");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab  + "{");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab  + softTab + "bWakeUp = USBHID_handleDataReceived(HID" + hidCount + "_INTFNUM); ");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab  + "}");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "}");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "else");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "{");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab  + softTab + "//complete receive opereation - copy data to user buffer");
                        oStream.newLine();
                        oStream.write(softTab + softTab + softTab  + softTab + "bWakeUp = HidToBufferFromHost(HID" + hidCount + "_INTFNUM);");
                        oStream.newLine();
                        oStream.write(softTab + softTab + "}");
                        oStream.newLine();
                        interfceCount++;
                        hidCount++;
                    }else if (name.contentEquals("MSCInterface430")){
                        oStream.write(softTab + softTab + "bWakeUp = MSCFromHostToBuffer();");
                        oStream.newLine();
                        interfceCount++;
                        mscCount++;
                    }else if(name.contentEquals("CDCInterface430")){
                        if(previousCDC == false){
                            previousCDC = true;
                        }else{
                            oStream.write(softTab + softTab + "//call callback function if no receive operation is underway");
                            oStream.newLine();
                            oStream.write(softTab + softTab + "if (!CdcIsReceiveInProgress(CDC" + cdcCount + "_INTFNUM) && USBCDC_getBytesInUSBBuffer(CDC" + cdcCount + "_INTFNUM))");
                            oStream.newLine();
                            oStream.write(softTab + softTab + "{");
                            oStream.newLine();
                            oStream.write(softTab + softTab + softTab + "if (wUsbEventMask & USB_DATA_RECEIVED_EVENT)");
                            oStream.newLine();
                            oStream.write(softTab + softTab + softTab + "{");
                            oStream.newLine();
                            oStream.write(softTab + softTab + softTab + softTab  + "bWakeUp = USBCDC_handleDataReceived(CDC" + cdcCount + "_INTFNUM);");
                            oStream.newLine();
                            oStream.write(softTab + softTab + softTab + "}");
                            oStream.newLine();
                            oStream.write(softTab + softTab + "}");
                            oStream.newLine();
                            oStream.write(softTab + softTab + "else");
                            oStream.newLine();
                            oStream.write(softTab + softTab + "{");
                            oStream.newLine();
                            oStream.write(softTab + softTab + softTab + "//complete receive opereation - copy data to user buffer");
                            oStream.newLine();
                            oStream.write(softTab + softTab + softTab + "bWakeUp = CdcToBufferFromHost(CDC" + cdcCount + "_INTFNUM);");
                            oStream.newLine();
                            oStream.write(softTab + softTab +  "}");
                            oStream.newLine();
                            interfceCount++;
                            cdcCount++;
                            previousCDC = false;
                        }
                    }
                    else if(name.contentEquals("PHDCInterface430")){
                        if(previousPHDC == false){
                            previousPHDC = true;
                        }else{
                            oStream.write(softTab + softTab + "//call callback function if no receive operation is underway");
                            oStream.newLine();
                            oStream.write(softTab + softTab + "if (!PHDCIsReceiveInProgress(PHDC0_INTFNUM))");
                            oStream.newLine();
                            oStream.write(softTab + softTab + "{");
                            oStream.newLine();
                            oStream.write(softTab + softTab + softTab  + "if (wUsbEventMask & USB_DATA_RECEIVED_EVENT)");
                            oStream.newLine();
                            oStream.write(softTab + softTab + softTab  + "{");
                            oStream.newLine();
                            oStream.write(softTab + softTab + softTab  + softTab + "bWakeUp = USBPHDC_handleDataReceived(PHDC0_INTFNUM);");
                            oStream.newLine();
                            oStream.write(softTab + softTab + softTab  + "}");
                            oStream.newLine();
                            oStream.write(softTab + softTab + "}");
                            oStream.newLine();
                            oStream.write(softTab + softTab + "else");
                            oStream.newLine();
                            oStream.write(softTab + softTab + "{");
                            oStream.newLine();
                            oStream.write(softTab + softTab + softTab  + softTab + "//complete receive opereation - copy data to user buffer");
                            oStream.newLine();
                            oStream.write(softTab + softTab + softTab  + softTab + "bWakeUp = PHDCToBufferFromHost(PHDC0_INTFNUM); ");
                            oStream.newLine();
                            oStream.write(softTab + softTab + "}");
                            oStream.newLine();
                            
                            interfceCount++;
                            phdcCount++;
                            previousPHDC = false;
                        }
                    }
                }
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_USBISR_C_EXIT_LPM>
     */
    class MUDTCG_USBISR_C_EXIT_LPM implements Callback {

        @Override
        public void execute() {
            try {
                if(mspConfig.isSysBIOS() == false){
                    oStream.write(softTab + "if (bWakeUp)");
                    oStream.newLine();
                    oStream.write(softTab + "{");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "__bic_SR_register_on_exit(LPM3_bits);" + "   // Exit LPM0-3");
                    oStream.newLine();
                    oStream.write(softTab + softTab + "__no_operation();" + "  // Required for debugger");
                    oStream.newLine();
                    oStream.write(softTab +"}");
                    oStream.newLine();
                }
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_INF_FILE_DEVICE_LIST>
     */
    class MUDTCG_INF_FILE_DEVICE_LIST implements Callback {

        @Override
        public void execute() {
            try {
                int i=0;
                if (numOfCdcInterfaces>0){
                    for(CDCInterface430 cdcIntf : cDCInterface430List){
                        if((numOfCdcInterfaces == 1) && (numOfHidInterfaces ==0 ) && (numOfMscInterfaces ==0) && (numOfPhdcInterfaces ==0)){
                            oStream.write("%DESCRIPTION" + i + "%=TIUSB, USB\\Vid_" + formattedVid + "&Pid_" + formattedPid);
                            oStream.newLine();
                        }else{
                            oStream.write("%DESCRIPTION" + i + "%=TIUSB, USB\\Vid_" + formattedVid + "&Pid_" + formattedPid + "&MI_0" + i);
                            oStream.newLine();
                        }
                        i+=2;
                    }
                }
                
            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_INF_FILE_DEVICE_LIST_64>
     */
    class MUDTCG_INF_FILE_DEVICE_LIST_64 implements Callback {

        @Override
        public void execute() {
            try {
                if(numOfCdcInterfaces > 0){
                    int i=0;
                    for(CDCInterface430 cdcIntf : cDCInterface430List){
                        if((numOfCdcInterfaces == 1) && (numOfHidInterfaces ==0 ) && (numOfMscInterfaces ==0) && (numOfPhdcInterfaces ==0)){
                            oStream.write("%DESCRIPTION" + i + "%=TIUSB.NTamd64, USB\\Vid_" + formattedVid + "&Pid_" + formattedPid);
                            oStream.newLine();
                        }else{
                            oStream.write("%DESCRIPTION" + i + "%=TIUSB.NTamd64, USB\\Vid_" + formattedVid + "&Pid_" + formattedPid + "&MI_0" + i);
                            oStream.newLine();
                        }
                        i+=2;
                    }
                }

            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    }  
    
    /**
     * Class that performs the substitution for the tag <MUDTCG_INF_STRING_DESCRIPTORS>
     */
    class MUDTCG_INF_STRING_DESCRIPTORS implements Callback {

        @Override
        public void execute() {
            try {
                if(numOfCdcInterfaces > 0){
                    int i=0;
                    oStream.write("DESCRIPTION=\"" + usbDevConfig.getProduct() + "\"");
                    oStream.newLine();
                    for(CDCInterface430 cdcIntf : cDCInterface430List){
                        oStream.write("DESCRIPTION" + i + "=\"" + cdcIntf.getInterfString() + "\"");
                        oStream.newLine();
                        i++;
                    }
                }

            } catch (IOException ex) {
                StaticInfo.debug(ex.getMessage(), 0);
            }
        }
    } 
    
    
    
    /**
     * Copy the source directory to destination
     *
     * @param src
     * @param dest
     * @return
     */
    private boolean copyTemplateSource(File src, File dest) {

        if (!src.exists()) {
            StaticInfo.debug("Directory does not exist: " + src.getAbsolutePath());
        } else {
            try {
                Directory.copy(src, dest);
            } catch (IOException e) {
                StaticInfo.error("Could not copy: " + src.getAbsolutePath());
                StaticInfo.debug(e.getMessage());
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * Perform all required template substitutions on the file
     *
     * @param f
     */
    private boolean processFile(File f) {

        StaticInfo.debug("Processing: " + f.getAbsolutePath(), 0);

        boolean ok = true;

        // Open the output file buffer 
        FileWriter fw;
        try {
            fw = new FileWriter(f.getAbsoluteFile() + "_new");
            oStream = new BufferedWriter(fw);

            // Read the template file (XmlConfig/template.c) 
            FileInputStream fStream = new FileInputStream(f.getAbsolutePath());
            DataInputStream in = new DataInputStream(fStream);
            BufferedReader br = new BufferedReader(new InputStreamReader(in));

            // for each line 
            //     if line matches a token, 
            //         call func to process that token 
            //     else 
            //         write the line unmodified to outfile 
            String strLine;
            while ((strLine = br.readLine()) != null) {
                // Split the line by spaces, if the first token matches one in our 
                // tree, call the appropriate handler. Otherwise, write the unmodified 
                // line to the output file 
                String[] tokens = strLine.trim().split("\\s+");


                String upCase = tokens[0].toUpperCase();
                if (upCase.startsWith("<MUDTCG")) {
                    Callback handler = callbacks.get(upCase);
                    if (handler != null) {
                        handler.execute();
                    } else {
                        StaticInfo.debug("   ERROR: Unrecognized token '" + upCase + "'", 0);
                    }
                } // Echo any line that is not a CDC specific comment
                else if (!upCase.startsWith("//MUDTCG")) {
                    oStream.write(strLine);
                    oStream.newLine();
                }
            }
            // Close the file streams 
            in.close();
            oStream.close();
        } catch (IOException ex) {
            StaticInfo.debug(ex.getMessage(), 0);
        }

        // Delete the original file and rename the _new version
        try {
            if (f.delete()) {
                File newF = new File(f.getAbsoluteFile() + "_new");
                if (!newF.renameTo(f)) {
                    StaticInfo.debug("Rename failed" + newF.getAbsolutePath(), 0);
                }
            } else {
                StaticInfo.debug("Delete failed" + f.getAbsolutePath(), 0);
            }
        } catch (Exception ex) {
            StaticInfo.debug(ex.getMessage(), 0);
        }

        return ok;
    }    
    
    
}
//Released_Version_5_20_06_02
