/* --COPYRIGHT--,BSD
 * Copyright (c) 2013, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.ti.msp.descriptortool.codegen;

import com.ti.msp.common.StaticInfo;
import com.ti.msp.descriptortool.DescriptorSaveFile;
import com.ti.msp.descriptortool.DescriptorTool;
import com.ti.msp.descriptortool.DescriptorToolMainPanel;
import com.ti.msp.descriptortool.Main;
import java.awt.Component;
import java.io.File;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JTree;

/**
 *
 * @author a0272979
 */
public class CodeGeneration {
    private String outputDir = null;
    private JTree configTree;
    private Component parent;
    private DescriptorTool desctool;

    public String getOutputDir() {
        return outputDir;
    }

    public void setOutputDir(String outputDir) {
        this.outputDir = outputDir;
    }

    
    // Constructor
    public CodeGeneration(String outputDir, JTree configTree, Component parent, 
            DescriptorTool descTool) { 
        this.outputDir = outputDir;
        this.configTree = configTree;
        this.parent = parent;
        this.desctool = descTool;
        
        if(validateOutputDir()){
            run(true,true);
        }
        
    }
    
    // Constructor
    public CodeGeneration(String outputDir, JTree configTree, Component parent, 
            boolean infGen, boolean xmlGen) { 
        this.outputDir = outputDir;
        this.configTree = configTree;
        this.parent = parent;
        
        if(validateOutputDir()){
            run(infGen, xmlGen);
        }
        
    }

    private boolean validateOutputDir() {
        boolean isOutputSet;
        
        if(parent != null){
            isOutputSet = askForOututDir();
        }else{
            return true;
        }
        
        if(isOutputSet){
            if(isDirectoryEmpty()){
                return true;
            }
        }
        
        return false;
    }

    private boolean askForOututDir() {
        JFileChooser fc = new JFileChooser(outputDir);
        fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
        int returnVal = fc.showSaveDialog(parent);
        
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            File file = fc.getSelectedFile();
            outputDir = file.getAbsolutePath();
        }else{
            return false;
        }
        
        return true;
        
    }

    private boolean isDirectoryEmpty() {
        // check directory exists
        File outputDirFile = new File(outputDir);
        if(outputDirFile.exists()){
            if(outputDirFile.canRead() && outputDirFile.canWrite()){
                int length = outputDirFile.listFiles().length;
                if(length > 0){
                    File[] files = outputDirFile.listFiles();
                    for(File file : files){
                        if(file.getAbsolutePath().contains("descriptors.c") ||
                           file.getAbsolutePath().contains("descriptors.h") ||
                           file.getAbsolutePath().contains("MSP430_CDC.inf")||
                           file.getAbsolutePath().matches(".*.xml") ||
                           file.getAbsolutePath().contains("UsbIsr.c"))
                        {
                            Object[] options = {"Yes","No"};

                            int n = JOptionPane.showOptionDialog(parent, 
                                "The destination directory is not empty.\n\n" + 
                                "Do you want to overide directory content?", 
                                "Overwrite directory content?", 
                                JOptionPane.YES_NO_OPTION, 
                                JOptionPane.QUESTION_MESSAGE,
                                null,
                                options,
                                options[1]);

                            if(n == JOptionPane.YES_OPTION){
                                return true;

                            }else{
                                return false;
                            }   
                        }else{
                            return true;
                        }
                    }
                }else{
                    return true;
                }
                
                
                
            }else{
                StaticInfo.error(outputDir + " is not readable or writable");
            }
        }else{
            if(parent != null){
                Object[] options = {"Yes","No"};

                int n = JOptionPane.showOptionDialog(parent, 
                    "Would you like to create output directory: \n" + outputDir, 
                    "Create output directory?", 
                    JOptionPane.YES_NO_OPTION, 
                    JOptionPane.QUESTION_MESSAGE,
                    null,
                    options,
                    options[1]);

                if(n == JOptionPane.YES_OPTION){
                    if(outputDirFile.mkdir()){
                        StaticInfo.debug("Output directory: " + outputDir + 
                                " created succesfully.");
                        return true;
                    }else{
                        StaticInfo.error("Error creating " + outputDir);
                    }
                }
            }else{
                StaticInfo.error(outputDir + " does not exist");
            }
        }
        return false;
    }
    
    

    private void run(boolean infGen, boolean xmlGen) {
        GeneratedFiles430 descFiles;
        if(parent != null){
            descFiles = new GeneratedFiles430(outputDir, 
                configTree, parent, false);
        }else{
            descFiles = new GeneratedFiles430(outputDir, 
                configTree, parent, true);
        }
        descFiles.generate(infGen);
        if(xmlGen == true){
            DescriptorSaveFile save = new DescriptorSaveFile(parent, outputDir + System.getProperty("file.separator") + "MSP430USBDesc.xml");
            if(parent != null){
                desctool.outputLoc = outputDir;
            }
            save.setGeneration(outputDir);
            save.saveDesign(configTree);
            if(parent != null){
                save.setSaveLocation(Main.DEFAULTCONF);
                save.setGeneration(outputDir);
                desctool.outputLoc = outputDir;
                DescriptorToolMainPanel panel = (DescriptorToolMainPanel) parent;
                panel.outputLoc = outputDir;
                save.saveDesign(configTree);
            }
        }
        
    }
    
    
    
}

    
//Released_Version_5_20_06_02
