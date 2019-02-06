/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.ti.msp.descriptortool;

import com.ti.msp.common.StaticInfo;
import com.ti.msp.descriptortool.USB.USBDeviceConfig;
import com.ti.msp.descriptortool.USB.USBModuleConfiguration;
import com.ti.msp.descriptortool.USBinterfaces.CDCInterface430;
import com.ti.msp.descriptortool.USBinterfaces.HIDInterface430;
import com.ti.msp.descriptortool.USBinterfaces.MSCInterface430;
import com.ti.msp.descriptortool.USBinterfaces.PHDCInterface430;
import com.ti.msp.descriptortool.codegen.ConfigTreeProcessor;
import com.ti.msp.descriptortool.mspConfig.MSP430Config;
import java.awt.Component;
import java.io.File;
import java.util.ArrayList;
import javax.swing.JFileChooser;
import javax.swing.JTree;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

/**
 *
 * @author a0272979
 */
public class DescriptorSaveFile {
    Component parent;
    String outputLoc;
    String generation;
    
    public DescriptorSaveFile(Component parent,String outputLoc) {
        this.parent = parent;
        this.outputLoc = outputLoc;
    }
    
    
    
    
    public boolean setSaveLocation(){
        JFileChooser fc = new JFileChooser(outputLoc + System.getProperty("file.separator")  + "MSP430USBDesc.xml");
        int returnVal = fc.showSaveDialog(parent);
        
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            File file = fc.getSelectedFile();
            this.outputLoc = file.getAbsolutePath();
            this.generation  = file.getAbsolutePath();
            return true;
        }
        
        return false;
    }
    
     public void setSaveLocation(String outputLoc){
         this.outputLoc = outputLoc;
     }
     
     public void setGeneration(String outputLoc){
         this.generation = outputLoc;
     };
     
     
     
    
    public void saveDesign(JTree tree){
        ConfigTreeProcessor processor  = new ConfigTreeProcessor(tree);
        ArrayList<CDCInterface430> cDCInterface430List= new ArrayList<CDCInterface430>();
        ArrayList<HIDInterface430> hidInterface430List = new ArrayList<HIDInterface430>();
        ArrayList<MSCInterface430> mscInterface430List = new ArrayList<MSCInterface430>();
        ArrayList<PHDCInterface430> phdcInterface430List = new ArrayList<PHDCInterface430>();
        
        
        MSP430Config mspConfig = processor.getMSP430Config();
        USBDeviceConfig usbDevConfig = processor.getUSBDeviceConfig();
        USBModuleConfiguration usbModConfig = processor.getUSBModuleConfig();
        
        int numOfMscInterfaces = processor.getNumberOfMSCinterfaces();
        if(numOfMscInterfaces > 0 ){
            mscInterface430List = processor.getMSCinterfaces();
        }
        
        int numOfCdcInterfaces = processor.getNumberOfCDCinterfaces();
        if(numOfCdcInterfaces > 0 ){
            cDCInterface430List = processor.getCDCinterfaces();
        }
        
        int numOfHidInterfaces = processor.getNumberOfHIDinterfaces();
        if(numOfHidInterfaces > 0 ){
            hidInterface430List = processor.getHIDinterfaces();
        }
        
        int numOfPhdcInterfaces = processor.getNumberOfPHDCinterfaces();
        if(numOfPhdcInterfaces > 0 ){
            phdcInterface430List = processor.getPHDCinterfaces();
        }
        
        
        try {
 
		DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
		DocumentBuilder docBuilder = docFactory.newDocumentBuilder();
 
		// descTool element
		Document doc = docBuilder.newDocument();
		Element descToolElement = doc.createElement("descTool");
                doc.appendChild(descToolElement);

                // toolOptions element
                Element toolOptionsElement = doc.createElement("toolOptions");
                descToolElement.appendChild(toolOptionsElement);
                
                Element runGUIElement = doc.createElement("runGUI");
                runGUIElement.appendChild(doc.createTextNode("1"));
                toolOptionsElement.appendChild(runGUIElement);
                
                
                Element workspaceElement = doc.createElement("workspace");
                workspaceElement.appendChild(doc.createTextNode(new File(generation).getAbsolutePath()));
                toolOptionsElement.appendChild(workspaceElement);
                
                Element projectNameElement = doc.createElement("projectName");
                projectNameElement.appendChild(doc.createTextNode("n/a"));
                toolOptionsElement.appendChild(projectNameElement);
                
                // toolConfig element
                Element toolConfigElement = doc.createElement("toolConfig");
                descToolElement.appendChild(toolConfigElement);
                
                Element treeConfigElement = doc.createElement("treeConfig");
                toolConfigElement.appendChild(treeConfigElement);
                
                Element MSPElement = doc.createElement("MSP");
                MSPElement.setAttribute("mcuFam", "0");
                treeConfigElement.appendChild(MSPElement);

                Element mspConfigElement = doc.createElement("mspConfig");
                MSPElement.appendChild(mspConfigElement);
                
                
                Element disableXtalOnSusElement = doc.createElement("disableXtalOnSus");
                disableXtalOnSusElement.appendChild(doc.createTextNode(mspConfig.isDisCrysOnSus() ? "1" : "0"));
                mspConfigElement.appendChild(disableXtalOnSusElement);
                Element XT2OscFreqElement = doc.createElement("XT2OscFreq");
                XT2OscFreqElement.appendChild(doc.createTextNode(String.valueOf(mspConfig.getCrystOScSel())));
                mspConfigElement.appendChild(XT2OscFreqElement);
                Element DMAChannelElement = doc.createElement("DMAChannel");
                DMAChannelElement.appendChild(doc.createTextNode(String.valueOf(mspConfig.getDamChannel())));
                mspConfigElement.appendChild(DMAChannelElement);
                Element bypassXT2Element = doc.createElement("bypassXT2");
                bypassXT2Element.appendChild(doc.createTextNode(mspConfig.isBypass() ? "1" : "0"));
                mspConfigElement.appendChild(bypassXT2Element);
                Element useSYSBIOSElement = doc.createElement("useSYSBIOS");
                useSYSBIOSElement.appendChild(doc.createTextNode(mspConfig.isSysBIOS() ? "1" : "0"));
                mspConfigElement.appendChild(useSYSBIOSElement);
                Element driveVBUSfromExtSuppElement = doc.createElement("driveVBUSfromExtSupp");
                driveVBUSfromExtSuppElement.appendChild(doc.createTextNode(mspConfig.isDriveVBUSExt() ? "1" : "0"));
                mspConfigElement.appendChild(driveVBUSfromExtSuppElement);
                
     
                
                
                Element USBDeviceElement = doc.createElement("USBDevice");
                MSPElement.appendChild(USBDeviceElement);
                
                Element devicePropElement = doc.createElement("deviceProp");
                USBDeviceElement.appendChild(devicePropElement);
                
                Element vidElement = doc.createElement("vid");
                vidElement.appendChild(doc.createTextNode(usbDevConfig.getvID()));
                devicePropElement.appendChild(vidElement);
                Element pidElement = doc.createElement("pid");
                pidElement.appendChild(doc.createTextNode(usbDevConfig.getpID()));
                devicePropElement.appendChild(pidElement);
                Element vndStrElement = doc.createElement("vndStr");
                vndStrElement.appendChild(doc.createTextNode(usbDevConfig.getVendor()));
                devicePropElement.appendChild(vndStrElement);
                Element prodStrElement = doc.createElement("prodStr");
                prodStrElement.appendChild(doc.createTextNode(usbDevConfig.getProduct()));
                devicePropElement.appendChild(prodStrElement);
                Element devRelNumElement = doc.createElement("devRelNum");
                devRelNumElement.appendChild(doc.createTextNode(usbDevConfig.getDevRelNum()));
                devicePropElement.appendChild(devRelNumElement);
                Element useSerNumElement = doc.createElement("useSerNum");
                useSerNumElement.appendChild(doc.createTextNode(usbDevConfig.isUseSerial() ? "1" : "0"));
                devicePropElement.appendChild(useSerNumElement);
                
                
                
                
                Element configElement = doc.createElement("config");
                USBDeviceElement.appendChild(configElement);
                
                Element configParamElement = doc.createElement("configParam");
                configElement.appendChild(configParamElement);
                Element selfPoweredElement = doc.createElement("selfPowered");
                selfPoweredElement.appendChild(doc.createTextNode(usbModConfig.isSelfPowered() ? "1" : "0"));
                configParamElement.appendChild(selfPoweredElement);
                Element remWkupSupElement = doc.createElement("remWkupSup");
                remWkupSupElement.appendChild(doc.createTextNode(usbModConfig.isRemoteWakeUp() ? "1" : "0"));
                configParamElement.appendChild(remWkupSupElement);
                Element maxPowDrainElement = doc.createElement("maxPowDrain");
                maxPowDrainElement.appendChild(doc.createTextNode(String.valueOf(usbModConfig.getMaxPowerDraw())));
                configParamElement.appendChild(maxPowDrainElement);
                Element configStrElement = doc.createElement("configStr");
                configStrElement.appendChild(doc.createTextNode(usbModConfig.getConfigurationString()));
                configParamElement.appendChild(configStrElement);
                Element interfacesElement = doc.createElement("interfaces");
                configElement.appendChild(interfacesElement);
                
                
                for(MSCInterface430 intf : mscInterface430List){
                    addMSCInterfaces(doc,interfacesElement, intf);
                }
                
                for(CDCInterface430 intf : cDCInterface430List){
                    addCDCInterfaces(doc,interfacesElement, intf);
                }
                
                for(HIDInterface430 intf : hidInterface430List){
                    addHIDInterfaces(doc,interfacesElement, intf);
                }
                
                for(PHDCInterface430 intf : phdcInterface430List){
                    addPHDCInterfaces(doc,interfacesElement, intf);
                }
                
                
                
                Element codeGenerationElement = doc.createElement("codeGeneration");
                codeGenerationElement.setAttribute("generate", "0");
                codeGenerationElement.setAttribute("generateINF", "0");
                descToolElement.appendChild(codeGenerationElement);

                // write the content into xml file
		TransformerFactory transformerFactory = TransformerFactory.newInstance();
		Transformer transformer = transformerFactory.newTransformer();
                transformer.setOutputProperty(OutputKeys.INDENT, "yes");
                transformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "4");
		DOMSource source = new DOMSource(doc);
		StreamResult result = new StreamResult(new File(outputLoc));
 
		// Output to console for testing
		// StreamResult result = new StreamResult(System.out);
 
		transformer.transform(source, result);
                StaticInfo.debug("File saved!");
 
	  } catch (ParserConfigurationException pce) {
		pce.printStackTrace();
	  } catch (TransformerException tfe) {
		tfe.printStackTrace();
	  }
    }

    private void addMSCInterfaces(Document doc, Element interfacesElement, MSCInterface430 intf) {
        Element mscInterfaceElement = doc.createElement("mscInterface");
        mscInterfaceElement.setAttribute("intString", intf.getInterfString());
        interfacesElement.appendChild(mscInterfaceElement);
        
        Element LUNElement = doc.createElement("LUN");
        LUNElement.setAttribute("hdMode", String.valueOf((intf.isHardDriveMode()) ? 1 : 0));
        LUNElement.setAttribute("cdRomMode", String.valueOf((intf.isCdRomMode()) ? 1 : 0));
        LUNElement.setAttribute("numberOfLUN", String.valueOf(intf.getNumOfLuns()));
        mscInterfaceElement.appendChild(LUNElement);
        
        
        for(Integer lunNum: intf.getLunMap().keySet()){
            MSCInterface430.LogicalUnit lun = intf.getLunMap().get(lunNum);
            Element lunConfigElement = doc.createElement("LUNConfig");
            LUNElement.appendChild(lunConfigElement);
            
            Element removableMeadiaElement = doc.createElement("removableMedia");
            removableMeadiaElement.appendChild(doc.createTextNode(String.valueOf((lun.isRemovableMedia()) ? 1 : 0)));
            lunConfigElement.appendChild(removableMeadiaElement);
            
            Element t10StrElement = doc.createElement("t10Str");
            lunConfigElement.appendChild(t10StrElement);
            
            Element vendorElement = doc.createElement("vendor");
            vendorElement.appendChild(doc.createTextNode(lun.getT10Vendor()));
            t10StrElement.appendChild(vendorElement);
            Element productElement = doc.createElement("product");
            productElement.appendChild(doc.createTextNode(lun.getT10Product()));
            t10StrElement.appendChild(productElement);
            Element revisionElement = doc.createElement("revision");
            revisionElement.appendChild(doc.createTextNode(lun.getT10Revision()));
            t10StrElement.appendChild(revisionElement);
            
            Element useDevforT10Element = doc.createElement("useDevforT10");
            useDevforT10Element.appendChild(doc.createTextNode(String.valueOf((lun.isUseUSBVidinfo()) ? 1 : 0)));
            lunConfigElement.appendChild(useDevforT10Element);
            
        }
        
        
        
        
        
    }
    
    
    private void addCDCInterfaces(Document doc, Element interfacesElement, CDCInterface430 intf) {
        Element cdcInterfaceElement = doc.createElement("cdcInterface");
        cdcInterfaceElement.setAttribute("intString", intf.getInterfString());
        interfacesElement.appendChild(cdcInterfaceElement);
        
    }
    
    private void addHIDInterfaces(Document doc, Element interfacesElement, HIDInterface430 intf) {
        Element hidInterfaceElement = doc.createElement("hidInterface");
        hidInterfaceElement.setAttribute("intString", intf.getInterfString());
        interfacesElement.appendChild(hidInterfaceElement);
        
        Element hidParamsElement = doc.createElement("hidParams");
        hidInterfaceElement.appendChild(hidParamsElement);
        
        Element pollIntvElement = doc.createElement("pollIntv");
        pollIntvElement.appendChild(doc.createTextNode(String.valueOf(intf.getPollingInterv())));
        hidParamsElement.appendChild(pollIntvElement);
        
        Element hidRepTypeElement = doc.createElement("hidRepType");
        int reportType = intf.getHidReportType();
        hidRepTypeElement.appendChild(doc.createTextNode(String.valueOf(reportType)));
        hidParamsElement.appendChild(hidRepTypeElement);
        
        if(reportType == 3){
            Element hidRepStringElement = doc.createElement("hidRepString");
            hidRepStringElement.appendChild(doc.createCDATASection(intf.getCustomReportText()));
            hidParamsElement.appendChild(hidRepStringElement);
            
            Element hidRepLengthElement = doc.createElement("hidRepLength");
            hidRepLengthElement.appendChild(doc.createTextNode(intf.getReportLenInput()));
            hidParamsElement.appendChild(hidRepLengthElement);
        }
    }
    
    private void addPHDCInterfaces(Document doc, Element interfacesElement, PHDCInterface430 intf) {
        Element phdcInterfaceElement = doc.createElement("phdcInterface");
        phdcInterfaceElement.setAttribute("intString", intf.getInterfString());
        interfacesElement.appendChild(phdcInterfaceElement);
    }
    
}
//Released_Version_5_20_06_02
