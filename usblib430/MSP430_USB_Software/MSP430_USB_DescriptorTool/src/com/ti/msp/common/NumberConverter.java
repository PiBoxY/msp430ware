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
package  com.ti.msp.common;

import java.util.*;

/**
  NumberConverter contains static methods to convert Strings to numeric
  values.

  @author SC Tsongas
  @version $Id: NumberConverter.java,v 1.1 1998/04/17 17:50:45 ada Exp $
  */
public class NumberConverter {

    /**
      public static long safeParseLong(String val) converts a String representation
      of a number into a long value or 0 if it is an invalid number.

      @author SC Tsongas
      */
    public static long safeParseLong(String val) {
        if (val==null) return(0);
        try {
            long ret=Long.parseLong(val.trim());
            return(ret);
        } catch (NumberFormatException e) {
            return(0);
        }
    }


    /**
      public static  int safeParseInt(String val) converts a String representation
      of a number into a int value or 0 if it is an invalid number.

      @author SC Tsongas
      */
    public static  int safeParseInt(String val) {
        //System.out.println("val="+val);
        if (val==null) return((int)0);
        try {
            int ret=Integer.parseInt(val.trim());
            return(ret);
        } catch (NumberFormatException e) {
            return(0);
        }
    }

    /**
      public static double safeParseDouble(String val) converts a String 
      representation of a number into a double value or 0.0 if it is an 
      invalid number.

      @author SC Tsongas
      */
    public static double safeParseDouble(String val) {
        if (val==null) return((double)0.0);
        try {
            return(Double.valueOf(val).doubleValue());
        } catch (NumberFormatException e) {
            return(0);
        }
    }



    /**
      public static float safeParseFloat(String val) converts a String 
      representation of a number into a float value or 0.0 if it is an
      invalid number.

      @author SC Tsongas
      */
    public static float safeParseFloat(String val) {
        return( (float) safeParseDouble(val));
    }



    /**
      public static String convertLong(long in) converts a long/int value
      into its string representation.

      @author SC Tsongas
      */
    public static String convertLong(long in) {
        Long I = new Long(in);
        return(I.toString());
    }

    /**
      public static String convertDouble(double in) converts a double/float value
      into its string representation.

      @author SC Tsongas
      */
    public static String convertDouble(double in) {
        Double D = new Double(in);
        return(D.toString());
    }

    public static double[] toDouble(String data,double div) {

        double retVal[];
        StringTokenizer st = new StringTokenizer(data," ");
        int tokens=st.countTokens();
        retVal= new double[tokens];

        for (int i=0;i<tokens;i++) {
            retVal[i]=(NumberConverter.safeParseDouble(st.nextToken())/div);
        }
        return(retVal);
    }

}
//Released_Version_5_20_06_02
