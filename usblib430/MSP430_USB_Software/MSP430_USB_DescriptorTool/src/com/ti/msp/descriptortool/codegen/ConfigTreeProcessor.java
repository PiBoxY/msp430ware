/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.ti.msp.descriptortool.codegen;

import com.ti.msp.descriptortool.USB.USBDeviceConfig;
import com.ti.msp.descriptortool.USB.USBModuleConfiguration;
import com.ti.msp.descriptortool.USBinterfaces.CDCInterface430;
import com.ti.msp.descriptortool.USBinterfaces.HIDInterface430;
import com.ti.msp.descriptortool.USBinterfaces.MSCInterface430;
import com.ti.msp.descriptortool.USBinterfaces.PHDCInterface430;
import com.ti.msp.descriptortool.mspConfig.MSP430Config;
import java.util.ArrayList;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;

/**
 *
 * @author a0272979
 */
public class ConfigTreeProcessor {
    private JTree configTree;

    public ConfigTreeProcessor(JTree configTree) {
        this.configTree = configTree;
    }
    
    
    
    private DefaultMutableTreeNode getConfigurationLevel() {
        // Gets msp rootNode
        DefaultMutableTreeNode mspNode = (DefaultMutableTreeNode)
                 (configTree.getModel().getRoot());
      
        //Gets "configuration" childNode
        return (DefaultMutableTreeNode) mspNode.getChildAt(0).getChildAt(0);
    }
    
    
    public MSP430Config getMSP430Config() {
        // Gets msp rootNode
        DefaultMutableTreeNode mspNode = (DefaultMutableTreeNode)
                 (configTree.getModel().getRoot());
      
        //Gets "configuration" childNode
        return (MSP430Config) mspNode.getUserObject();
    }
    
    public USBDeviceConfig getUSBDeviceConfig() {
        // Gets msp rootNode
        DefaultMutableTreeNode mspNode = (DefaultMutableTreeNode)
                 (configTree.getModel().getRoot());
        DefaultMutableTreeNode usbDevNode =  (DefaultMutableTreeNode) mspNode.getChildAt(0);
      
        //Gets "configuration" childNode
        return (USBDeviceConfig)usbDevNode.getUserObject();
    }
    
    public USBModuleConfiguration getUSBModuleConfig() {
        // Gets msp rootNode
        DefaultMutableTreeNode usbModuleConfNode = getConfigurationLevel();
      
        //Gets "configuration" childNode
        return (USBModuleConfiguration)usbModuleConfNode.getUserObject();
    }
    
    
    public short getNumberOfCDCinterfaces(){
        DefaultMutableTreeNode node = getConfigurationLevel();
        
        short numIntf=0;
        for(int i=0; i<node.getChildCount();i++){
            DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) node.getChildAt(i);
            if(childNode.getUserObject() instanceof CDCInterface430){
                numIntf ++;
            }
        }
        
        return numIntf;
    }
    
    
    public short getNumberOfHIDinterfaces(){
        DefaultMutableTreeNode node = getConfigurationLevel();
        
        short numIntf=0;
        for(int i=0; i<node.getChildCount();i++){
            DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) node.getChildAt(i);
            if(childNode.getUserObject() instanceof HIDInterface430){
                numIntf ++;
            }
        }
        
        return numIntf;
    }
    
    public short getNumberOfMSCinterfaces(){
        DefaultMutableTreeNode node = getConfigurationLevel();
        
        short numIntf=0;
        for(int i=0; i<node.getChildCount();i++){
            DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) node.getChildAt(i);
            if(childNode.getUserObject() instanceof MSCInterface430){
                numIntf ++;
            }
        }
        
        return numIntf;
    }
    
    public short getNumberOfPHDCinterfaces(){
        DefaultMutableTreeNode node = getConfigurationLevel();
        
        short numIntf=0;
        for(int i=0; i<node.getChildCount();i++){
            DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) node.getChildAt(i);
            if(childNode.getUserObject() instanceof PHDCInterface430){
                numIntf ++;
            }
        }
        
        return numIntf;
    }
    
    public ArrayList<CDCInterface430> getCDCinterfaces(){
        ArrayList<CDCInterface430> list = new ArrayList<CDCInterface430>();
        DefaultMutableTreeNode node = getConfigurationLevel();
        
        for(int i=0; i<node.getChildCount();i++){
            DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) node.getChildAt(i);
            if(childNode.getUserObject() instanceof CDCInterface430){
                list.add((CDCInterface430) childNode.getUserObject());
            }
        }
        
        return list;
    }
    
    public ArrayList<HIDInterface430> getHIDinterfaces(){
        ArrayList<HIDInterface430> list = new ArrayList<HIDInterface430>();
        DefaultMutableTreeNode node = getConfigurationLevel();
        
        for(int i=0; i<node.getChildCount();i++){
            DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) node.getChildAt(i);
            if(childNode.getUserObject() instanceof HIDInterface430){
                list.add((HIDInterface430) childNode.getUserObject());
            }
        }
        
        return list;
    }
    
    public ArrayList<MSCInterface430> getMSCinterfaces(){
        ArrayList<MSCInterface430> list = new ArrayList<MSCInterface430>();
        DefaultMutableTreeNode node = getConfigurationLevel();
        
        for(int i=0; i<node.getChildCount();i++){
            DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) node.getChildAt(i);
            if(childNode.getUserObject() instanceof MSCInterface430){
                list.add((MSCInterface430) childNode.getUserObject());
            }
        }
        
        return list;
    }
    
    public ArrayList<PHDCInterface430> getPHDCinterfaces(){
        ArrayList<PHDCInterface430> list = new ArrayList<PHDCInterface430>();
        DefaultMutableTreeNode node = getConfigurationLevel();
        
        for(int i=0; i<node.getChildCount();i++){
            DefaultMutableTreeNode childNode = (DefaultMutableTreeNode) node.getChildAt(i);
            if(childNode.getUserObject() instanceof PHDCInterface430){
                list.add((PHDCInterface430) childNode.getUserObject());
            }
        }
        
        return list;
    }
    
    
}
//Released_Version_5_20_06_02
