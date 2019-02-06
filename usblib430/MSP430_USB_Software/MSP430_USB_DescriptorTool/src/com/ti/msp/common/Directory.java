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

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.TreeMap;

/**
 * Contains some helper routines to recursively copy, list and delete directories and
 * files.
 *
 * @author a0182881
 */
public class Directory {

    /**
     * Get a recursive list of all the files in the directory
     *
     * @param src
     * @param fList
     */
    public static void listFiles(File src, ArrayList<File> fList) {

        if (src.isDirectory()) {

            //list all the directory contents
            String files[] = src.list();

            for (String file : files) {
                //construct the src and dest file structure
                File srcFile = new File(src, file);
                // recursive traveral
                listFiles(srcFile, fList);
            }

        } else {
            //if file, then add to the list
            fList.add(src);
        }
    }

    /**
     * Return a tree structure of the files. Where each key in the tree is directory name,
     * and the value is the list of leaf files in that directory
     *
     * @param src
     * @return
     */
    public static TreeMap<String, ArrayList<File>> getFileTree(File src) {

        ArrayList<File> fList = new ArrayList<File>();
        listFiles(src, fList);

        TreeMap<String, ArrayList<File>> dirTree = new TreeMap<String, ArrayList<File>>();
        for (File aF : fList) {
            String fPath = aF.getAbsolutePath();
            String dPath = aF.getParent();

            ArrayList<File> dirFList = dirTree.get(dPath);
            if (dirFList == null) {
                dirFList = new ArrayList<File>();
                dirTree.put(dPath, dirFList);
            }
            dirFList.add(aF);
        }
        return dirTree;
    }

    /**
     * Recursively copy from src to dest directory
     *
     * @param src
     * @param dest
     * @throws IOException
     */
    public static void copy(File src, File dest) throws IOException {

        if (src.isDirectory()) {

            //if directory not exists, create it
            if (!dest.exists()) {
                dest.mkdir();
                StaticInfo.debug("Directory copied from " + src + "  to " + dest);
            }

            //list all the directory contents
            String files[] = src.list();

            for (String file : files) {
                //construct the src and dest file structure
                File srcFile = new File(src, file);
                File destFile = new File(dest, file);
                //recursive copy
                copy(srcFile, destFile);
            }

        } else {
            //if file, then copy it
            //Use bytes stream to support all file types
            InputStream in = new FileInputStream(src);
            OutputStream out = new FileOutputStream(dest);

            byte[] buffer = new byte[1024];

            int length;
            //copy the file content in bytes 
            while ((length = in.read(buffer)) > 0) {
                out.write(buffer, 0, length);
            }

            in.close();
            out.close();
            StaticInfo.debug("File copied from " + src + " to " + dest);
        }
    }

    /**
     * Recursively delete a directory and all its contents
     *
     * @param file
     * @throws IOException
     */
    public static void delete(File file)
            throws IOException {

        if (file.isDirectory()) {

            //directory is empty, then delete it
            if (file.list().length == 0) {

                file.delete();
                StaticInfo.debug("Directory is deleted : " + file.getAbsolutePath());

            } else {

                //list all the directory contents
                String files[] = file.list();

                for (String temp : files) {
                    //construct the file structure
                    File fileDelete = new File(file, temp);

                    //recursive delete
                    delete(fileDelete);
                }

                //check the directory again, if empty then delete it
                if (file.list().length == 0) {
                    file.delete();
                    StaticInfo.debug("Directory is deleted : " + file.getAbsolutePath());
                }
            }

        } else {
            //if file, then delete it
            file.delete();
            StaticInfo.debug("File is deleted : " + file.getAbsolutePath());
        }
    }

    /**
     * Test main
     *
     * @param args
     */
    public static void main(String[] args) {
        File srcFolder = new File("C:/ccs_workspace");
        File destFolder = new File("C:/ccs_workspace_copy");

        //make sure source exists
        if (!srcFolder.exists()) {
            System.out.println("No list, Directory does not exist.");
            //just exit
            System.exit(0);

        } else {
            //List the contents
            ArrayList<File> fList = new ArrayList<File>();
            listFiles(srcFolder, fList);
            System.out.println(fList);
        }

        //make sure source exists
        if (!srcFolder.exists()) {
            System.out.println("No copy, Directory does not exist.");
            //just exit
            System.exit(0);

        } else {
            try {
                copy(srcFolder, destFolder);
            } catch (IOException e) {
                e.printStackTrace();
                //error, just exit
                System.exit(0);
            }
        }

        //make sure directory exists
        if (!destFolder.exists()) {
            System.out.println("No delete, Directory does not exist.");
            System.exit(0);

        } else {

            try {
                delete(destFolder);
            } catch (IOException e) {
                e.printStackTrace();
                System.exit(0);
            }
        }

        StaticInfo.debug("Done");
    }
}
//Released_Version_5_20_06_02
