package com.ti.msp.descriptortool;

import com.ti.msp.descriptortool.USBinterfaces.CDCInterface430;
import com.ti.msp.descriptortool.USBinterfaces.HIDInterface430;
import com.ti.msp.descriptortool.USBinterfaces.MSCInterface430;
import com.ti.msp.descriptortool.USBinterfaces.PHDCInterface430;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.Map;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JTree;
import javax.swing.UIManager;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;


public class DescriptorToolTree {
    LinkedHashMap<String,Object> descToolMap;

    public DescriptorToolTree(DescriptorLoadFile loadFile) {
        this.descToolMap = loadFile.getDescToolMap();
        generateTree();
    }

    public JTree generateTree() {
        DefaultMutableTreeNode mspNode;
            
        // Create mspNode as rootNode for Navigation Tree
        mspNode = new DefaultMutableTreeNode(
                descToolMap.get("msp"));

        // Add "USB Device" node as childNode of mspNode
        DefaultMutableTreeNode usbDeviceNode = new DefaultMutableTreeNode(
                descToolMap.get("usb"));
        mspNode.add(usbDeviceNode);

        // Add "Configuration" node as childNode of usbDeviceNode
        DefaultMutableTreeNode configNode = new DefaultMutableTreeNode(
                descToolMap.get("usbModConf"));
        usbDeviceNode.add(configNode);
        
        LinkedHashMap<String,Object> intfMap = (LinkedHashMap<String,Object>) descToolMap.get("interfaces");
        Map<String, ArrayList<String>> map = new LinkedHashMap<String, ArrayList<String>>();
        map = getInterfaceNumber(intfMap);
        
        ArrayList<String> tempArrayList = map.get("msc");
        int interfaceNumber = 0;
        
        // Add MSC Interface if available
        if(tempArrayList.isEmpty() == false){
            MSCInterface430 mSCInterface430 = (MSCInterface430) intfMap.get(tempArrayList.get(0));
            mSCInterface430.setIntfNum(interfaceNumber++);
            DefaultMutableTreeNode intfNode = new DefaultMutableTreeNode(mSCInterface430);
            configNode.add(intfNode);
        }
        
        tempArrayList = map.get("cdc");
        
        // Add CDC Interface(s) if available
        if(tempArrayList.isEmpty() == false){
            for(String intfName: tempArrayList){
                CDCInterface430 cDCInterface430 = (CDCInterface430) intfMap.get(intfName);
                cDCInterface430.setIntfNum(interfaceNumber++);
                DefaultMutableTreeNode intfNode = new DefaultMutableTreeNode(cDCInterface430);
                configNode.add(intfNode);
            }
        }
        
        tempArrayList = map.get("hid");
        
        // Add HID Interface(s) if available
        if(tempArrayList.isEmpty() == false){
            for(String intfName: tempArrayList){
                HIDInterface430 hIDInterface430 = (HIDInterface430) intfMap.get(intfName);
                hIDInterface430.setInterfNum(interfaceNumber++);
                DefaultMutableTreeNode intfNode = new DefaultMutableTreeNode(hIDInterface430);
                configNode.add(intfNode);
            }
        }
        
        tempArrayList = map.get("phdc");

        
        // Add PHDC Interface if available
        if(tempArrayList.isEmpty() == false){
            PHDCInterface430 pHDCInterface430 = (PHDCInterface430) intfMap.get(tempArrayList.get(0));
            pHDCInterface430.setInterfNum(interfaceNumber++);
            DefaultMutableTreeNode intfNode = new DefaultMutableTreeNode(pHDCInterface430);
            configNode.add(intfNode);
        }

        Icon leafIcon = new ImageIcon(getClass().getResource("/com/ti/msp/descriptortool/images/navigationLeafIcon.png"));
        Icon folderIcon = new ImageIcon(getClass().getResource("/com/ti/msp/descriptortool/images/navigationFolderIcon.png"));
        Icon expandedIcon = new ImageIcon(getClass().getResource("/com/ti/msp/descriptortool/images/minus.png"));
        Icon collapsedIcon = new ImageIcon(getClass().getResource("/com/ti/msp/descriptortool/images/add.png"));
        UIManager.put("Tree.expandedIcon", expandedIcon);
        UIManager.put("Tree.collapsedIcon", collapsedIcon);
        UIManager.put("Tree.leafIcon", leafIcon);
        UIManager.put("Tree.openIcon", folderIcon);
        UIManager.put("Tree.closedIcon", folderIcon);
        JTree configTree = new JTree(new DefaultTreeModel(mspNode));
        
        return configTree;
    } 
    

    private Map<String, ArrayList<String>> getInterfaceNumber(LinkedHashMap<String, Object> intfMap) {
        Map<String,ArrayList<String>> map = new LinkedHashMap<String, ArrayList<String>>();
        ArrayList<String> mscIntf = new ArrayList<String>();
        ArrayList<String> hidIntf = new ArrayList<String>();
        ArrayList<String> cdcIntf = new ArrayList<String>();
        ArrayList<String> phdcIntf = new ArrayList<String>();

        for(String key: intfMap.keySet()){
            if(key.matches("msc.*")){
                 mscIntf.add(key);
            }else if(key.matches("cdc.*")){
                cdcIntf.add(key);
            }else if(key.matches("hid.*")){
                hidIntf.add(key);  
            }else if(key.matches("phdc.*")){
                phdcIntf.add(key);
            }else{
                System.err.println("Error sorting interfaces!");
                return null;
            } 
        } 
        map.put("msc", mscIntf);
        map.put("cdc", cdcIntf);
        map.put("hid", hidIntf);
        map.put("phdc", phdcIntf);

        return map;
    }
       
}
//Released_Version_5_20_06_02
