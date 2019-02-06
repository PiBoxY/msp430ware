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

/**
 * This class allows applications to capture and log to an external file any output to 
 * the StdOut and StdErr streams. This is especially useful when trying to capture 
 * un-caught exceptions and stack traces.
 *
 */
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.util.logging.ConsoleHandler;
import java.util.logging.FileHandler;
import java.util.logging.Level;
import java.util.logging.LogRecord;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;

public class LogOutputStream extends ByteArrayOutputStream {
    
    private static final Logger logger = Logger.getLogger(LogOutputStream.class.getName());
    private String lineSeparator = System.getProperty("line.separator");
    private static PrintStream printStream;

    public LogOutputStream(String logFile) {
        super();
     
        printStream = new PrintStream(this, true);

        // Disable the default console handler for this logger
        logger.setUseParentHandlers(false);

        SimpleFormatter sf = new SimpleFormatter() {
            @Override
            public String format(LogRecord record) {
                return record.getMessage() + lineSeparator;
            }
        };

        // Associate the logger with a file handler 
        try {
            FileHandler fileHandler = new FileHandler(logFile);
            fileHandler.setFormatter(sf);
            logger.addHandler(fileHandler);
        } catch (IOException ex) {
            logger.log(Level.SEVERE, null, ex);
        } catch (SecurityException ex) {
            logger.log(Level.SEVERE, null, ex);
        }

        // Associate the logger with a new console handler that will only print 
        // the simple message 
        try {
            ConsoleHandler consoleHandler = new ConsoleHandler();
            consoleHandler.setFormatter(sf);
            logger.addHandler(consoleHandler);
        } catch (SecurityException ex) {
            logger.log(Level.SEVERE, null, ex);
        }
    }

    @Override
    public void close() throws IOException {
        super.close();
        printStream.close();
    }

    /**
     * Get the print stream object that is used to redirect stderr
     * @return 
     */
    public PrintStream getPrintStream() {
        return printStream;
    }

    /** 
     * upon flush() write the existing contents of the OutputStream
     * to the logger as a log record. 
     * @throws java.io.IOException in case of error 
     */ 
    @Override
    public void flush() throws IOException { 
 
        String record; 
        synchronized(this) { 
            super.flush(); 
            record = this.toString(); 
            super.reset(); 
 
            if (record.length() == 0 || record.equals(lineSeparator)) { 
                // avoid empty records 
                return; 
            } 
            logger.info(record); 
        } 
    }     

    /**
     * Test main
     */
    static public void main(String[] args) {

        // Create a logger and tie it to StdErr and StdOut
        LogOutputStream los = new LogOutputStream("LogOutput.log");
        System.setErr(los.getPrintStream());
        System.setOut(los.getPrintStream());

        // Show that direct outputs will go to the log file
        StaticInfo.debug("StaticInfo msg txt", 0);
        System.out.println("StdOut text");
        System.err.println("StdErr text");

        // Show stderr stack trace going to logger
        try {
            throw new RuntimeException("Test");
        } catch (Exception e) {
            e.printStackTrace();
        }

        // Close the log stream
        try {
            los.close();
        } catch (IOException ex) {
            Logger.getLogger(LogOutputStream.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
}
//Released_Version_5_20_06_02
