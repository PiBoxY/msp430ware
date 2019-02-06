package com.ti.msp.descriptortool.codegen;

public class StringDescriptor implements java.io.Serializable
{
    // USB 2.0 Standard Endpoint Descriptor, page 269
    // No access modifiers used = package private members


    private String eol;

    private short bLength; // Descriptor length in bytes
    private short LANGIDTag;
    private int wLANGID;
    private short bDescriptorType;
    private String bString;

    public StringDescriptor(String bString)
    {
        super();
        eol = System.getProperty("line.separator");

        bDescriptorType = 0x03; // Initialize to device descriptor type
        LANGIDTag = 0x03;
        wLANGID = 0x0409; // Initialize to English by default
        bDescriptorType = 3;
        this.bString = bString;

        // Calculate bLength for each class that is instantiated
        calculateBLength();
    }

    public String generateOutput(boolean generateTrailingComma)
    {
        StringBuilder returnValue = new StringBuilder();
        Object[] bLengthArg =
        { new Short(bLength) };
        Object[] bDescriptorTypeArg =
        { new Short(bDescriptorType) };

        returnValue.append(String.format(
                "    %d,        // Length of this string descriptor%n", bLengthArg));
        returnValue.append(String.format("    %d,        // bDescriptorType%n",
                bDescriptorTypeArg));
        returnValue.append(constructBString(this.bString));

        return returnValue.toString();
    }

    public CharSequence generateLanguageOutput()
    {
        StringBuilder returnValue = new StringBuilder();
        bLength = 4;

        Object[] bLengthArg =
        { new Short(bLength) };
        Object[] LANGIDTagArg =
        { new Short(LANGIDTag) };
        Object[] wLANGIDArg =
        { new Short((short) (wLANGID & 0xff)),
                new Short((short) (wLANGID >> 8)) };

        returnValue.append(String.format(
                "    %d,        // Length of language descriptor ID%n", bLengthArg));
        returnValue.append(String.format("    %d,        // LANGID tag%n",
                LANGIDTagArg));
        returnValue.append(String.format(
                "    %0#4x, %0#4x,    // 0x0409 for English%n", wLANGIDArg));
        return returnValue.toString();
    }

    // Calculate the index length
    private void calculateBLength()
    {
        bLength = (short) (((this.bString.length()) * 2) + 2);
    }

    private String constructBString(String bString)
    {
        StringBuilder returnValue = new StringBuilder();
        int sizeBString = bString.length();

        returnValue.append("    ");
        for (int i = 0; i < sizeBString; i++)
        {
            if ((i != 0) && ((i % 6) == 0))
            {
                returnValue.append(eol);
                returnValue.append("    ");
            }
            returnValue.append('\'');

            // Special characters ["\'] need to be preceded by \
            // Eg: "Will's USB" should be 'W','i','l','l','\'', 's', 'U', 'S',
            // 'B'
            switch (bString.charAt(i))
            {
            case '\'':
            case '\\':
            case '\"':
                returnValue.append('\\');
                returnValue.append(bString.charAt(i));
                break;
            default:
                returnValue.append(bString.charAt(i));
                break;
            }

            returnValue.append('\'');
            returnValue.append(",0x00,");
        }

        return returnValue.toString();
    }

    public short getBDescriptorType()
    {
        return bDescriptorType;
    }

    public void setBDescriptorType(short descriptorType)
    {
        bDescriptorType = descriptorType;
    }

    public short getBLength()
    {
        return bLength;
    }

    public void setBLength(short length)
    {
        bLength = length;
    }

    public String getBString()
    {
        return bString;
    }

    public void setBString(String string)
    {
        bString = string;
        calculateBLength();
    }

    public short getLANGIDTag()
    {
        return LANGIDTag;
    }

    public void setLANGIDTag(short tag)
    {
        LANGIDTag = tag;
    }

    public int getWLANGID()
    {
        return wLANGID;
    }

    public void setWLANGID(short wlangid)
    {
        wLANGID = wlangid;
    }
}
//Released_Version_5_20_06_02
