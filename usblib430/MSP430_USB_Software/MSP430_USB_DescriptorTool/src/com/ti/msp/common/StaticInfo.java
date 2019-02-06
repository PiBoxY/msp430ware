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
package com.ti.msp.common;

import java.applet.Applet;
import java.awt.*;
import java.io.*;
import java.net.InetAddress;
import java.net.UnknownHostException;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.swing.*;

/**
 * StaticInfo class is used set startup parameters and return them in various parts of the
 * program.
 *
 * @authors SC Tsongas and LJ Douet
 * @version $ID: StaticInfo.java, v 1.1 1998/07/01
 */
public class StaticInfo {

    private static SimpleDateFormat simpleDate = new SimpleDateFormat("MM.dd HH:mm:ss");
    private static Applet applet = null;
    public static int persist = 1000;
    // if input debug val is <= debug value then print
    private static int debug = 0;
    private static String logFile;
    private static LogOutputStream logOutputStream;
    private static String version;
    private static String copyright;
    public static Hashtable procArgs = new Hashtable();
    private static boolean initialized = false;

    /**
     * Get the version
     *
     * @return
     */
    public static String getVersion() {
        return (version);
    }

    /**
     * Set the version
     *
     * @param v
     */
    public static void setVersion(String v) {
        version = v;
    }

    /**
     * Get the copyright dates
     *
     * @return
     */
    public static String getCopyrightDates() {
        return (copyright);
    }

    /**
     * Set the copyright dates
     *
     * @param c
     */
    public static void setCopyrightDates(String c) {
        copyright = c;
    }

    /**
     * public StaticInfo(Applet applet) Constructor given process is an applet.
     */
    public StaticInfo(Applet applet) {
        StaticInfo.applet = applet;
    }

    /**
     * public static void setApplet(Applet app) Method sets the value of applet
     */
    public static void setApplet(Applet app) {
        applet = app;
    }

    /**
     * public static Applet getApplet() Method returns an instance of the Applet.
     */
    public static Applet getApplet() {
        return (applet);
    }

    /**
     * public static boolean isApplet() Method returns boolean value that is used to
     * determine if process is an Applet.
     */
    public static boolean isApplet() {
        return (applet != null);
    }

    /**
     * public static void setArgs(String type,String[] arg_array) Method sets string array
     * of arguments for a specified type.
     */
    public static void setArgs(String type, String[] arg_array) {
        procArgs.put(type, arg_array);
    }

    /**
     * public static void setArgs(String[] arg_array) Method sets a string array of
     * arguments.
     */
    public static void setArgs(String[] arg_array) {
        setArgs("default", arg_array);
    }

    /**
     * public static String[] getArgs(String type) Method returns string array of
     * arguments for a specified type or null if not found.
     */
    public static String[] getArgs(String type) {
        return ((String[]) procArgs.get(type));
    }

    /**
     * public static String[] getArgs() Method returns string array of arguments.
     */
    public static String[] getArgs() {
        return (getArgs("default"));
    }

    /**
     * public static void setDebug(int newVal) Method set the Debug level.
     */
    public static void setDebug(int newVal) {
        //System.out.println("StaticInfo.setDebug called "+newVal);
        debug = newVal;
    }

    /**
     * public static int getDebug() Method returns the integer value of the Debug level.
     */
    public static int getDebug() {
        return (debug);
    }

    /**
     * public static void debug(String inp) Method set the default Debug level.
     */
    public static void debug(String inp) {
        debug(inp, 3); // by default do not print the regular debug lines
        // unless debug is set to 3 or higher
    }

    /**
     * public static void debug(String inp,int value) Method prints the debug message if
     * value greater than default level.
     */
    public static void debug(String inp, int value) {
        if (value <= debug) {
            System.out.println(simpleDate.format(new Date()) + " " + inp);
        }
    }

    /**
     * prints out a stack trace for the exception
     *
     * @param e an exception
     */
    public static void debug(Throwable e) {
        e.printStackTrace(System.out);
    }

    /**
     * public static String getShortHostName() throws UnknownHostException Method returns
     * string value of HostName.
     *
     * @exception java.net.UnknownHostException could not determine IP address of a host
     */
    public static String getShortHostName() throws UnknownHostException {
        if (applet != null) {
            return (applet.getCodeBase().getHost());
        }
        InetAddress inetaddr = InetAddress.getLocalHost();
        String host = inetaddr.getHostName();
        if (host.indexOf('.') == -1) {
            return host;
        }
        return (host.substring(0, host.indexOf('.')));
    }

    /**
     * Use the arguments to initialize the class
     */
    public static void init() {
        if (initialized) {
            return;
        }
        String debugLevel = null;
        String persist = null;

        if (!StaticInfo.isApplet()) {
            debugLevel = getTaggedValue("-debug");
            // set the debug level so we can see debug info on the rest of args
            StaticInfo.setDebug(NumberConverter.safeParseInt(debugLevel));
            persist = getTaggedValue("-persistence");
        } else {
            debugLevel = StaticInfo.getApplet().getParameter("debug");
            // set the debug level so we can see debug info on the rest of args
            StaticInfo.setDebug(NumberConverter.safeParseInt(debugLevel));
            persist = StaticInfo.getApplet().getParameter("persistence");
        }

        initialized = true;
    }

    /**
     * Initialize the log file to capture all stdout and stderr
     */
    public static void initLogFile(String lFile) {

        logFile = lFile;

        if (!logFile.equals("")) {
            File f = new File(logFile);
            try {
                f.createNewFile();
            } catch (IOException ex) {
                Logger.getLogger(StaticInfo.class.getName()).log(Level.SEVERE, null, ex);
            }
            StaticInfo.debug("Opening log file: " + f.getAbsolutePath(), 0);

            logOutputStream = new LogOutputStream(logFile);
            System.setErr(logOutputStream.getPrintStream());
            System.setOut(logOutputStream.getPrintStream());
        }
    }

    /**
     * Initialize the log file to capture all stdout and stderr
     */
    public static void initLogFile() {
        initLogFile(logFile);
    }

    /**
     * public static String getAppletArgs(String arg) Method returns the string value for
     * the Applet parameters.
     */
    public static String getAppletArgs(String arg) {
        String value = getApplet().getParameter(arg);
        return (value);
    }

    public static String getTaggedValue(String target) {
        return (getTaggedValue(getArgs(), target));
    }

    /**
     * public static String getTaggedValue(String args[],String target) Method returns the
     * string value for tagged arguments.
     */
    public static String getTaggedValue(String localArgs[], String target) {
        return (ArgParser.getTaggedValue(target, localArgs));
    }

    public static Properties loadProperties(String fname) {
        Properties plotterProp = new Properties();
        if (!StaticInfo.isApplet()) {
            File f = new File(fname);
            try {
                FileInputStream fis = new FileInputStream(f);
                plotterProp.load(fis);
                fis.close();
            } catch (Exception e) {
                plotterProp = new Properties(System.getProperties());
                StaticInfo.debug("Unable to load props using defaults! "
                        + fname);
            }
        }
        return (plotterProp);
    }

    public static void saveProperties(String fname, Properties props) {
        if (!StaticInfo.isApplet()) {
            File f = new File(fname);
            f.getParentFile().mkdirs();
            try {
                FileOutputStream fos = new FileOutputStream(f);
                props.store(fos, fname + " Plotter Data");
                fos.close();
            } catch (Exception e) {
                StaticInfo.debug("Unable to save props! " + fname);
            }
        }
    }

    public static void info(String message) {
        message(null, JOptionPane.INFORMATION_MESSAGE, message, null);
    }

    public static void info(Component parent, String message) {
        message(parent, JOptionPane.INFORMATION_MESSAGE, message, null);
    }

    public static void info(Component parent, String message, String title) {
        message(parent, JOptionPane.INFORMATION_MESSAGE, message, title);
    }

    public static void warning(String message) {
        message(null, JOptionPane.WARNING_MESSAGE, message, null);
    }

    public static void warning(Component parent, String message) {
        message(parent, JOptionPane.WARNING_MESSAGE, message, null);
    }

    public static void warning(Component parent, String message,
            String title) {
        message(parent, JOptionPane.WARNING_MESSAGE, message, title);
    }

    public static void error(String message) {
        message(null, JOptionPane.ERROR_MESSAGE, message, null);
    }

    public static void error(Component parent, String message) {
        message(parent, JOptionPane.ERROR_MESSAGE, message, null);
    }

    public static void error(Component parent, String message, String title) {
        message(parent, JOptionPane.ERROR_MESSAGE, message, title);
    }

    public static void error(Component parent, String message, String title,
            Exception ex) {
        if (ex != null) {
            message += ":\n" + ex;
        }
        debug(message);
        debug(ex);
        message(parent, JOptionPane.ERROR_MESSAGE, message, null);
    }

    /**
     * Displays a JOptionPane with default options based on the given parameters.
     *
     * @param parent The parent Component. If null, a default frame is used.
     * @param type The message type
     * @param message The message text
     * @param title The title text
     */
    public static void message(Component parent, int type, String message,
            String title) {
        JOptionPane jop = new JOptionPane();
        jop.setOptionType(JOptionPane.DEFAULT_OPTION);
        jop.setMessageType(type);
        jop.setMessage(message);
        if (title == null) {
            if (type == JOptionPane.ERROR_MESSAGE) {
                title = "Error";
            } else if (type == JOptionPane.WARNING_MESSAGE) {
                title = "Warning";
            } else {
                title = "Info";
            }
        }
        JDialog jd = jop.createDialog(parent, title);
        jd.setVisible(true);
    }

    /**
     * public static void setDoubleBuffering(boolean state) Method used to remove
     * doubleBuffering when using Swing components. Must be call before any components are
     * created.
     */
    public static void setDoubleBuffering(boolean state) {
        RepaintManager.currentManager(null).setDoubleBufferingEnabled(state);
    }

    public static void dumpThreads() {
        debug("Threads=" + Thread.activeCount(), 0);
        Thread tArray[] = new Thread[Thread.activeCount()];
        Thread.enumerate(tArray);
        for (int i = 0; i < tArray.length; i++) {
            StaticInfo.debug(tArray[i].getName());
        }
    }
}
//Released_Version_5_20_06_02
