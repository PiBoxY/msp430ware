/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package com.ti.msp.descriptortool;

import com.ti.msp.common.ArgParser;
import com.ti.msp.common.Splash;
import com.ti.msp.common.StaticInfo;
import com.ti.msp.descriptortool.codegen.CodeGeneration;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.JTree;
import javax.swing.UnsupportedLookAndFeelException;

public class Main {
    static String userHome = System.getProperty("user.home");;
    static String fileSeparator = System.getProperty("file.separator");
    static String MSPUSBDESCPATH = "Texas_Instruments" + fileSeparator + "MSP_USB_Descriptor_Tool";
    static String MSPUSBDESCHOME = userHome + fileSeparator + MSPUSBDESCPATH;
    static String MSPUSBDESCWORKSP = MSPUSBDESCHOME + fileSeparator + "workspace";
    public static String DEFAULTCONF = MSPUSBDESCWORKSP + fileSeparator + "MSP430USBDesc.xml";

   
    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        

        File home = new File(MSPUSBDESCHOME);
        File project = new File(MSPUSBDESCWORKSP);
        File defaultConf = new File(DEFAULTCONF);
        
        if(home.exists() == false){
            home.mkdirs();
        }
        
        StaticInfo.initLogFile(MSPUSBDESCHOME + fileSeparator + "logFile.log" );
        
        if(project.exists() == false){
            project.mkdirs();
        }
        
        if(defaultConf.exists() == false){
            try {
                File template = new File("configFiles/templates/load/MSP430USBDesc.xml");
                copyFileUsingFileStreams(template.getAbsoluteFile(), defaultConf.getAbsoluteFile());
            } catch (IOException ex) {
                Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
            }
            
        }
        
        
        Map<String,String> arguments = ArgParser.parse(args);
        
        JTree descToolTree = null;
        DescriptorLoadFile loadFile = null;
                      
        for(String arg:arguments.keySet()){
            if(arg.contentEquals("-version") || arg.contentEquals("v") ){ 
                System.out.println("2.00.00.03");
                System.exit(1);
            }else if(arg.contentEquals("-help") || arg.contentEquals("h") ){
                displayUsage();
            }else if(arg.contentEquals("-load") || arg.contentEquals("l") ){
                String filePath = arguments.get(arg);
                try {
                    loadFile = new DescriptorLoadFile(filePath);
                } catch (Exception ex) {
                    StaticInfo.debug(ex.getMessage());
                }
                DescriptorToolTree tree =  new DescriptorToolTree(loadFile);
                descToolTree = tree.generateTree();

            }else if(arg.contentEquals("-debug") || arg.contentEquals("d") ){
                String debug = arguments.get(arg);
                StaticInfo.setDebug(Integer.parseInt(debug));
            }
            else{
                System.err.println("Command line argument: \"" + 
                        arg + "\" Not supported!");
                displayUsage();
                System.exit(1);
            }
        } 
        
        // If user didn't specify load file, determine which file to load
        if(descToolTree == null){
            try {     
                loadFile = new DescriptorLoadFile(DEFAULTCONF);
            } catch (Exception ex) {
                StaticInfo.debug(ex.getMessage());
            }
        }
        
        // Generate Configuration Tree based on file which was loaded
        DescriptorToolTree tree =  new DescriptorToolTree(loadFile);
        descToolTree = tree.generateTree();
        
        if(loadFile.isRunGUI()){
            try{
                Splash splash = new Splash();
                splash.show();
            }catch(Exception e){
                System.out.println(e.getMessage());
            }
            runGUI(descToolTree,loadFile.getOutputLocation());
        }else{
            CodeGeneration cg = new CodeGeneration(loadFile.getOutputLocation(), 
                    descToolTree, null, loadFile.isGenearaeINF(), loadFile.isGenearaeXML());
        }
    }

    private static void runGUI(final JTree descToolTree, final String outputLoc) {
        
            for (javax.swing.UIManager.LookAndFeelInfo info : 
                    javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    try {
          
                            javax.swing.UIManager.setLookAndFeel(info.getClassName());
                        
                        
                        break;
                    } catch (UnsupportedLookAndFeelException ex) {
                        StaticInfo.debug(ex.getMessage());
                    } catch (ClassNotFoundException ex) {
                        Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
                    } catch (InstantiationException ex) {
                        Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
                    } catch (IllegalAccessException ex) {
                        Logger.getLogger(Main.class.getName()).log(Level.SEVERE, null, ex);
                    }
               
                }
            }
        

        /* Create and display the form */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                DescriptorTool tool = new DescriptorTool(descToolTree,
                        outputLoc);
                tool.setVisible(true);
            }
        });
    }

    private static void displayUsage() {
        String content = null;
        File file = new File("configFiles/messages/helpContent.txt");
        System.out.println(file.getAbsolutePath());
        try {
            FileReader reader = new FileReader(file);
            char[] chars = new char[(int) file.length()];
            reader.read(chars);
            content = new String(chars);
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println(content);

    }
    
    private static void copyFileUsingFileStreams(File source, File dest)
	        throws IOException {
	    InputStream input = null;
	    OutputStream output = null;
	    try {
	        input = new FileInputStream(source);
	        output = new FileOutputStream(dest);
	        byte[] buf = new byte[1024];
	        int bytesRead;
	        while ((bytesRead = input.read(buf)) > 0) {
	            output.write(buf, 0, bytesRead);
	        }
	    } finally {
	        input.close();
	        output.close();
	    }
	}
    
}
//Released_Version_5_20_06_02
