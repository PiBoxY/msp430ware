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

import java.util.*;

/**
 * ArgParser class is used to find the string value of tag from the command line
 * arguments.
 */
public final class ArgParser {

    /**
     * This method returns a Map whose keys are the tags (converted to lower case, without
     * leading dashes) and whose values are the argument values. Tags that are not
     * followed by values are entered as keys with empty string values.
     *
     * @param args The argument string array
     * @return A Map representation of the arguments
     * @author Paul Morken
     */
    public static Map parse(String[] args) {
        Hashtable argHash = new Hashtable();
        if (args != null) {
            for (int i = 0; i < args.length; i++) {
                String key = args[i];
                if (key.startsWith("-")) {
                    key = key.substring(1).toLowerCase();
                    String val = "";
                    if ((i + 1) < args.length && !(args[i + 1].startsWith("-"))) {
                        val = args[i + 1];
                    }
                    argHash.put(key, val);
                }
            }
        }
        return argHash;
    }

    /**
     * Pulls a tagged string value from the parsed command line args.
     *
     * @param argMap The parsed command line arguments (from ArgParser.parse())
     * @param key The argument key to search for (without a leading dash)
     * @return def The default value if the argument is not in the map
     */
    public static String getArg(Map argMap, String key, String def) {
        String val = (String) argMap.get(key.toLowerCase());
        return (val == null) ? def : val;
    }

    /**
     * Pulls a tagged integer value from the parsed command line args.
     *
     * @param argMap The parsed command line arguments (from ArgParser.parse())
     * @param key The argument key to search for (without a leading dash)
     * @return def The default value if the argument is not in the map
     */
    public static int getArg(Map argMap, String key, int def) {
        String val = (String) argMap.get(key.toLowerCase());
        if (val == null) {
            return def;
        }
        try {
            return Integer.parseInt(val);
        } catch (NumberFormatException nfe) {
            return def;
        }
    }

    /**
     * Pulls a tagged long value from the parsed command line args.
     *
     * @param argMap The parsed command line arguments (from ArgParser.parse())
     * @param key The argument key to search for (without a leading dash)
     * @return def The default value if the argument is not in the map
     */
    public static long getArg(Map argMap, String key, long def) {
        String val = (String) argMap.get(key.toLowerCase());
        if (val == null) {
            return def;
        }
        try {
            return Long.parseLong(val);
        } catch (NumberFormatException nfe) {
            return def;
        }
    }

    /**
     * Pulls a tagged float value from the parsed command line args.
     *
     * @param argMap The parsed command line arguments (from ArgParser.parse())
     * @param key The argument key to search for (without a leading dash)
     * @return def The default value if the argument is not in the map
     */
    public static float getArg(Map argMap, String key, float def) {
        String val = (String) argMap.get(key.toLowerCase());
        if (val == null) {
            return def;
        }
        try {
            return Float.parseFloat(val);
        } catch (NumberFormatException nfe) {
            return def;
        }
    }

    /**
     * Pulls a boolean flag from the command line args
     *
     * @param argMap The parsed command line arguments (from ArgParser.parse())
     * @param key The argument key to search for (without a leading dash)
     * @return def The default value if the argument is not in the map
     */
    public static boolean getArg(Map argMap, String key, boolean def) {
        if (!argMap.keySet().contains(key.toLowerCase())) {
            return false;
        }
        String val = (String) argMap.get(key.toLowerCase());
        if (val == null) {
            return def;
        }
        val = val.toLowerCase();
        if (val.equals("n") || val.equals("no") || val.equals("f")
                || val.equals("false") || val.equals("0") || val.equals("off")) {
            return false;
        }
        return true;
    }

    /**
     * public static String getTaggedValue(String target,String[] args) Method finds the
     * value of a command line tag.
     *
     * @deprecated Use the methods above instead.
     */
    public static String getTaggedValue(String target, String[] args) {
        if (args != null) {
            for (int i = 0; i < args.length; i++) {
                if (args[i].equalsIgnoreCase(target)) {
                    i++;
                    if (i < args.length) {
                        return (args[i]);
                    }
                    System.out.println("The parameter value must follow the "
                            + target + " tag");
                }
            }
        }
        return ("");
    }

    /**
     * @deprecated Use the methods above instead.
     */
    public static String[] getTags(String[] args) {
        Vector v = new Vector();
        if (args != null) {
            StaticInfo.debug("args.length=" + args.length);
            for (int i = 0; i < args.length; i++) {
                StaticInfo.debug("args=" + args[i]);
                if ((args[i] != null) && (args[i].length() > 0)) {
                    if (args[i].charAt(0) == '-') {
                        v.addElement(args[i]);
                    }
                }
            }
        }
        String ret[] = new String[v.size()];
        for (int i = 0; i < v.size(); i++) {
            ret[i] = (String) v.elementAt(i);
        }
        return (ret);
    }
}
//Released_Version_5_20_06_02
