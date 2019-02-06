package com.ti.msp.descriptortool;

import com.ti.msp.common.StaticInfo;
import com.ti.msp.descriptortool.USB.USBDeviceConfig;
import com.ti.msp.descriptortool.USB.USBModuleConfiguration;
import com.ti.msp.descriptortool.USBinterfaces.CDCInterface430;
import com.ti.msp.descriptortool.USBinterfaces.HIDInterface430;
import com.ti.msp.descriptortool.USBinterfaces.MSCInterface430;
import com.ti.msp.descriptortool.USBinterfaces.PHDCInterface430;
import com.ti.msp.descriptortool.mspConfig.MSP430Config;
import java.io.File;
import java.util.LinkedHashMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class DescriptorLoadFile {
    XMLDOM xmlDOM;
    LinkedHashMap<String,Object> descToolMap;
    
    public LinkedHashMap<String, Object> getDescToolMap() {
        return descToolMap;
    }

    public void setDescToolMap(LinkedHashMap<String, Object> descToolMap) {
        this.descToolMap = descToolMap;
    }

    DescriptorLoadFile() throws Exception {
        File file = new File("configFiles/templates/load/MSP430USBDesc.xml");
        String absPath = file.getAbsolutePath();

        boolean fileExists = file.exists() &  file.isFile();
        
        if(!fileExists){
            System.err.println("File: " + absPath + " doesn't exist or isn't"
                    + "a valid file." );
            System.exit(1);
        }else{
            this.xmlDOM = new XMLDOM(file.getAbsolutePath());
            
            extractContent();
            
        }
    }
    
    /**
     *
     * @param filePath
     */
    public DescriptorLoadFile(String filePath) throws Exception {
        
        File file = new File(filePath);
        String absPath = file.getAbsolutePath();

        boolean fileExists = file.exists() &  file.isFile();
        
        if(!fileExists){
            System.err.println("File: " + absPath + " doesn't exist or isn't"
                    + "a valid file." );
            System.exit(1);
        }else{
            this.xmlDOM = new XMLDOM(file.getAbsolutePath());
            
            extractContent();
            
        }
        
    }
   
    private LinkedHashMap<String,Object> extractContent(){
        this.descToolMap = new LinkedHashMap<String, Object>();
        Node mainNode;
        Node childNode;
        NodeList childNodes;
        // Extract ToolOptions
        mainNode = xmlDOM.findNode("toolOptions");
        childNodes = mainNode.getChildNodes();
        extractToolOptions(childNodes);
        
        // Get  Tree Configuration
        mainNode = xmlDOM.findNode("toolConfig");
        childNode = xmlDOM.findSubNode("treeConfig", mainNode);
        extractTreeConfig(childNode);    
        
        // Get  Code Generation Configuration
        mainNode = xmlDOM.findNode("codeGeneration");
        extractCodeGenConfig(mainNode);
        
        if(isFileContentValid()==true){
            System.out.println("Valid configuration file loaded");
            return descToolMap;
        }else{
            System.err.println("Invalid configuration file.");
            if(descToolMap.get("runGUI").toString().contentEquals("1")){
                StaticInfo.error("Invalid configuration file.");
            }
            System.exit(4);
        }
        
        
        return null;
    }
    
    public void showContent(){
        for(String content: descToolMap.keySet()){
            System.out.println(content +"\t" + descToolMap.get(content));
        }
    }

    private void extractToolOptions(NodeList childNodes) {
        for(int i=0; i<childNodes.getLength();i++){
             Node childNode = childNodes.item(i);
            if(childNode.getNodeType() == Node.ELEMENT_NODE){
                descToolMap.put(childNode.getNodeName(),
                        childNode.getTextContent());
            }
        }
    }

    private void extractTreeConfig(Node childNode) {
        // Extract MSP info
        Node mspNode = xmlDOM.findSubNode("MSP", childNode);
        //Determine MSP configuration
        String mspDevSelString = xmlDOM.getAttribute("mcuFam", mspNode);
        if(mspDevSelString.contentEquals("0")){
            extractMSP430Info(mspNode);
            extractUSBInfo(mspNode);
            extractUSBConfiguration(mspNode);
            
        }else{
            StaticInfo.error("MSP USB Descriptor tool only" +
                    "supports MSP430F5xx/F6xx mode!");
        }
        
    }
    
    private void extractCodeGenConfig(Node childNode) {
        // Extract Code Generation info
        descToolMap.put("codeGen", xmlDOM.getAttribute("generate", childNode));
        descToolMap.put("generateINF", xmlDOM.getAttribute("generateINF", childNode));
        descToolMap.put("generateXML", xmlDOM.getAttribute("generateXML", childNode));
    }

    private void extractMSP430Info(Node mspNode) {
        MSP430Config mspConfig = new MSP430Config();
        Node mspConfigNode = xmlDOM.findSubNode("mspConfig", mspNode);
        
        Node disXtalNode = xmlDOM.findSubNode("disableXtalOnSus", mspConfigNode);
        mspConfig.setDisCrysOnSus(
                xmlDOM.getText(disXtalNode).contentEquals("1"));
        
        Node xt2OscFreqNode = xmlDOM.findSubNode("XT2OscFreq", mspConfigNode);
        mspConfig.setCrystOScSel(
                Integer.parseInt(xmlDOM.getText(xt2OscFreqNode)));
        
        Node DMAChannelNode = xmlDOM.findSubNode("DMAChannel", mspConfigNode);
        mspConfig.setDamChannel(
                Integer.parseInt(xmlDOM.getText(DMAChannelNode)));
        
        Node useSYSBIOSNode = xmlDOM.findSubNode("useSYSBIOS", mspConfigNode);
        mspConfig.setSysBIOS(
                xmlDOM.getText(useSYSBIOSNode).contentEquals("1"));
        
        Node drvVBUSfrExtSupNode = xmlDOM.findSubNode("driveVBUSfromExtSupp", mspConfigNode);
        mspConfig.setDriveVBUSExt(
                xmlDOM.getText(drvVBUSfrExtSupNode).contentEquals("1"));
        
        Node bypassXT2Node = xmlDOM.findSubNode("bypassXT2", mspConfigNode);
        mspConfig.setBypass(
                xmlDOM.getText(bypassXT2Node).contentEquals("1"));
        
        descToolMap.put("msp", mspConfig);
    }

    private void extractUSBInfo(Node mspNode) {
        USBDeviceConfig usbDevConfig = new USBDeviceConfig();
        Node mspConfigNode = xmlDOM.findSubNode("USBDevice", mspNode);
        
        // Exctract USB device Properties
        Node devicePropNode = xmlDOM.findSubNode("deviceProp", mspConfigNode);
        
        Node vidNode = xmlDOM.findSubNode("vid", devicePropNode);
        usbDevConfig.setvID(xmlDOM.getText(vidNode));
        
        Node pidNode = xmlDOM.findSubNode("pid", devicePropNode);
        usbDevConfig.setpID(xmlDOM.getText(pidNode));
        
        Node vndStrNode = xmlDOM.findSubNode("vndStr", devicePropNode);
        usbDevConfig.setVendor(xmlDOM.getText(vndStrNode));
        
        Node prodStrNode = xmlDOM.findSubNode("prodStr", devicePropNode);
        usbDevConfig.setProduct(xmlDOM.getText(prodStrNode));
        
        Node devRelNumNode = xmlDOM.findSubNode("devRelNum", devicePropNode);
        usbDevConfig.setDevRelNum(xmlDOM.getText(devRelNumNode));
        
        Node useSerNumNode = xmlDOM.findSubNode("useSerNum", devicePropNode);
        usbDevConfig.setUseSerial(
                xmlDOM.getText(useSerNumNode).contentEquals("1"));
        
        descToolMap.put("usb", usbDevConfig);
        
    }

    private void extractUSBConfiguration(Node mspNode) {
        USBModuleConfiguration usbModConf = new USBModuleConfiguration();
        Node mspConfigNode = xmlDOM.findSubNode("USBDevice", mspNode);
        Node configNode = xmlDOM.findSubNode("config", mspConfigNode);
        
        // Exctract USB Module Configiration Properties
        Node configParamNode = xmlDOM.findSubNode("configParam", configNode);
        
        Node selfPoweredNode = xmlDOM.findSubNode(
                "selfPowered", configParamNode);
        usbModConf.setSelfPowered(
                xmlDOM.getText(selfPoweredNode).contentEquals("1"));
        
        Node remWkupSupNode = xmlDOM.findSubNode(
                "remWkupSup", configParamNode);
        usbModConf.setRemoteWakeUp(
                xmlDOM.getText(remWkupSupNode).contentEquals("1"));
        
        Node maxPowDrainNode = xmlDOM.findSubNode(
                "maxPowDrain", configParamNode);
        usbModConf.setMaxPowerDraw(
                Integer.parseInt(xmlDOM.getText(maxPowDrainNode)));
        
        Node configStrNode = xmlDOM.findSubNode("configStr", configParamNode);
        usbModConf.setConfigurationString(xmlDOM.getText(configStrNode));
        
        descToolMap.put("usbModConf", usbModConf);
        
        // Extract USB Interfaces
        Node interfacesNode = xmlDOM.findSubNode("interfaces", configNode);
        NodeList interfaces = interfacesNode.getChildNodes();
        LinkedHashMap<String,Object> interfMap = new LinkedHashMap<String, Object>();
        for(int i=0;i<interfaces.getLength(); i++){
            extractInterface(interfMap,interfaces.item(i),i);
            
        }   
        descToolMap.put("interfaces", interfMap);
        
    }

    private void extractInterface(LinkedHashMap<String, Object> interfMap, Node interf, int i) {
        if(interf.getNodeType()==Node.ELEMENT_NODE){
            String classString = interf.getNodeName();
            
            if(classString.contentEquals("cdcInterface")){
                interfMap.put(classString + i, extractCDCInterface(interf));
            }else if(classString.contentEquals("hidInterface")){
                interfMap.put(classString + i, extractHIDInterface(interf));
            }else if(classString.contentEquals("mscInterface")){
                interfMap.put(classString + i, extractMSCInterface(interf));
            }else if(classString.contentEquals("phdcInterface")){
                interfMap.put(classString + i, extractPHDCInterface(interf));
            }else{
                System.err.println("Invalid Interface found");
                System.exit(1);
            }
        }
    }

    private CDCInterface430 extractCDCInterface(Node interf) {
        CDCInterface430 cdcInt = new CDCInterface430();
        cdcInt.setInterfString(
                xmlDOM.getAttribute("intString", interf));
      
        return cdcInt;
        
    }

    private HIDInterface430 extractHIDInterface(Node interf) {
        HIDInterface430 hidIntf = new HIDInterface430();
        hidIntf.setInterfString(
                xmlDOM.getAttribute("intString", interf));
        
        // Esxtract HID Parameters
        Node hidParamNode = xmlDOM.findSubNode("hidParams", interf);
        
        Node pollIntvNode = xmlDOM.findSubNode("pollIntv", hidParamNode);
        hidIntf.setPollingInterv(
                Integer.parseInt(xmlDOM.getText(pollIntvNode)));
        
        Node hidRepTypeNode = xmlDOM.findSubNode("hidRepType", hidParamNode);
        hidIntf.setHidReportType(
                Integer.parseInt(xmlDOM.getText(hidRepTypeNode)));
        if(Integer.parseInt(xmlDOM.getText(hidRepTypeNode))==3){
           Node hidRepStringNode = xmlDOM.findSubNode("hidRepString", hidParamNode);
           hidIntf.setCustomReportText(xmlDOM.getText(hidRepStringNode));
           
           Node hidRepLengthNode = xmlDOM.findSubNode("hidRepLength", hidParamNode);
           hidIntf.setReportLenInput(xmlDOM.getText(hidRepLengthNode));
        }
        
        return hidIntf;
    
    }

    private MSCInterface430 extractMSCInterface(Node interf) {
        
        MSCInterface430 mSCIntf = new MSCInterface430();
        LinkedHashMap<Integer,MSCInterface430.LogicalUnit> lunMap = 
                new LinkedHashMap<Integer,MSCInterface430.LogicalUnit>();
        
        Node lunNode = xmlDOM.findSubNode("LUN", interf);
        
        mSCIntf.setNumOfLuns(
                Integer.parseInt(xmlDOM.getAttribute("numberOfLUN", lunNode)));
        
        mSCIntf.setCdRomMode(
                xmlDOM.getAttribute("cdRomMode", lunNode).contentEquals("1"));
        
        mSCIntf.setHardDriveMode(
                xmlDOM.getAttribute("hdMode", lunNode).contentEquals("1"));
       
        NodeList lunConfs = lunNode.getChildNodes();
        
        int lunNum=0;
        for(int i=0; i<lunConfs.getLength();i++){
            Node lunConf = lunConfs.item(i);
            if(lunConf.getNodeType()==Node.ELEMENT_NODE){
                MSCInterface430.LogicalUnit lun = mSCIntf.new LogicalUnit(i);

                Node removableMenuNode = xmlDOM.findSubNode("removableMedia", 
                        lunConf);
                lun.setRemovableMedia(
                        xmlDOM.getText(removableMenuNode).contentEquals("1"));

                Node t10StrNode = xmlDOM.findSubNode("t10Str", lunConf);

                Node vendorNode = xmlDOM.findSubNode("vendor", t10StrNode);
                lun.setT10Vendor(xmlDOM.getText(vendorNode));

                Node productNode = xmlDOM.findSubNode("product", t10StrNode);
                lun.setT10Product(xmlDOM.getText(productNode));

                Node revisionNode = xmlDOM.findSubNode("revision", t10StrNode);
                lun.setT10Revision(xmlDOM.getText(revisionNode));

                Node useDevforT10Node = xmlDOM.findSubNode("useDevforT10", 
                        lunConf);
                lun.setUseUSBVidinfo(
                        xmlDOM.getText(useDevforT10Node).contentEquals("1"));
                
                mSCIntf.getLunMap().put(lunNum, lun);
                lunNum++;
            }
        }
        
        return mSCIntf;
  
    }

    private PHDCInterface430 extractPHDCInterface(Node interf) {
        PHDCInterface430 phdcInt = new PHDCInterface430();
        phdcInt.setInterfString(
                xmlDOM.getAttribute("intString", interf));
      
        return phdcInt;
    }
    
    public boolean isRunGUI(){
        return descToolMap.get("runGUI").toString().contentEquals("1");
    }
    
    public boolean isGenearaeINF(){
        return descToolMap.get("generateINF").toString().contentEquals("1");
    }
    
    public boolean isGenearaeXML(){
        return descToolMap.get("generateXML").toString().contentEquals("1");
    }
    
    public String getOutputLocation(){
        String workspaceLoc = descToolMap.get("workspace").toString();
        
        if(workspaceLoc.toLowerCase().contentEquals("default")){
            String fileSep = File.separator;
            workspaceLoc = System.getProperty("user.home") + fileSep +
                    "MSP_DescriptorTool" + fileSep;
        }
        
        
        return workspaceLoc + File.separator;
    }
    
    public boolean isCmdLnMode(){
        return descToolMap.get("runGUI").toString().contentEquals("0");
    }
    
    public boolean isGenerateCode(){
        return descToolMap.get("codeGen").toString().contentEquals("1");
    }

    private boolean isFileContentValid() {
        //Determine mode
        boolean generateCode = isGenerateCode();
        boolean cmdLnEnabled = isCmdLnMode();
        
        // If file is going to be used to run tool in automated testing mode
        int endpoints = getNumberOfEndPoints();
            
        if((endpoints > 7)){
            System.err.println("Invalid number of USB interfaces");
            if(cmdLnEnabled== true){
                System.exit(6);
            }
            return false;
        }
        if(generateCode == true){
            USBDeviceConfig usb = (USBDeviceConfig)descToolMap.get("usb");
            String vid = usb.getvID().toLowerCase().trim();
            String pid = usb.getpID().toLowerCase().trim();
            
            if(vid.isEmpty() == false){
                if(pid.isEmpty() == false){
                    int vidInt;
                    int pidInt;

                    if(vid.contains("0x")){
                        vidInt = Integer.parseInt(vid.replace("0x", ""),16);
                    }else{
                        vidInt = Integer.parseInt(vid);
                    }

                    if(pid.contains("0x")){
                        pidInt = Integer.parseInt(pid.replace("0x", ""),16);
                    }else{
                        pidInt = Integer.parseInt(pid);
                    } 
                    
                    if((vidInt<0x10000)){
                        if(pidInt<0x10000){
                            return true;
                        }else{
                            System.err.println("Invalid \"PID\" value ");
                            return false;
                        }
                    }else{
                       System.err.println("Invalid \"VID\" value ");
                        return false; 
                    }                   
                }else{
                    System.err.println("Invalid \"PID\" field");
                    return false;
                }
                
            }else{
                System.err.println("Invalid \"VID\" field");
                return false;
            }     
        }
    
        return true;
    }

    private int getNumberOfEndPoints() {
        int endpoints=0;
        int mscIntf = 0;
        int phdcIntf = 0;
        LinkedHashMap<String,Object> intfMap =(LinkedHashMap<String,Object>)descToolMap.get("interfaces");
        
        for(String key: intfMap.keySet()){
            if(key.matches("cdcInterface.*")){
                endpoints += 2;
            }else if(key.matches("hidInterface.*")){
                endpoints += 1;
            }else if(key.matches("mscInterface.*")){
                if(mscIntf == 0 ){
                    endpoints += 1;
                    mscIntf +=1;
                }else{
                    System.err.println("Too many MSC Interfaces");
                    return -1;
                }  
            }else if(key.matches("phdcInterface.*")){
                if(phdcIntf == 0){
                    endpoints += 2;
                    phdcIntf =+1;
                }
                else{
                    System.err.println("Too many PHDC Interfaces");
                    return -1;
                }
            }  
        }
     
       return endpoints; 
    }
    
}
//Released_Version_5_20_06_02
