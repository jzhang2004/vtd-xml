package com.ximpleware;

import com.ximpleware.enums.TokenType;

/**
 * Created by IntelliJ IDEA.
 * User: dmay
 * Date: Aug 14, 2007
 * Time: 3:29:20 PM
 * To change this template use File | Settings | File Templates.
 */
public class VTDBits
{
    public final static int OFFSET_BIT_SIZE = 30;
    public final static int RESERVED_BIT_SIZE = 2;
    public final static int QNAME_BIT_SIZE = 11;
    public final static int PREFIX_BIT_SIZE = 9;
    public final static int DEPTH_BIT_SIZE = 8;
    public final static int TYPE_BIT_SIZE = 4;

    public final static int OFFSET_START_BIT = OFFSET_BIT_SIZE - 1;
    public final static int RESERVED_START_BIT = OFFSET_START_BIT + RESERVED_BIT_SIZE;
    public final static int QNAME_START_BIT = RESERVED_START_BIT + QNAME_BIT_SIZE;
    public final static int PREFIX_START_BIT = QNAME_START_BIT + PREFIX_BIT_SIZE;
    public final static int DEPTH_START_BIT = PREFIX_START_BIT + DEPTH_BIT_SIZE;
    public final static int TYPE_START_BIT = DEPTH_START_BIT + TYPE_BIT_SIZE;

    public final static int OFFSET_SHIFT_BITS = 0;
    public final static int RESERVED_SHIFT_BITS = OFFSET_SHIFT_BITS + OFFSET_BIT_SIZE;
    public final static int QNAME_SHIFT_BITS = RESERVED_SHIFT_BITS + RESERVED_BIT_SIZE;
    public final static int PREFIX_SHIFT_BITS = QNAME_SHIFT_BITS + QNAME_BIT_SIZE;
    public final static int DEPTH_SHIFT_BITS = PREFIX_SHIFT_BITS + PREFIX_BIT_SIZE;
    public final static int TYPE_SHIFT_BITS = DEPTH_SHIFT_BITS + DEPTH_BIT_SIZE;


    private final static long maskstart = 0x8000000000000000l;
    public final static long OFFSET_MASK = (maskstart >> OFFSET_BIT_SIZE -1) >>> 63-OFFSET_START_BIT;
    public final static long RESERVED_MASK = (maskstart >> RESERVED_BIT_SIZE -1) >>> 63-RESERVED_START_BIT;
    public final static long QNAME_MASK = (maskstart >> QNAME_BIT_SIZE -1) >>> 63-QNAME_START_BIT;
    public final static long PREFIX_MASK = (maskstart >> PREFIX_BIT_SIZE -1) >>> 63-PREFIX_START_BIT;
    public final static long DEPTH_MASK = (maskstart >> DEPTH_BIT_SIZE -1) >>> 63-DEPTH_START_BIT;
    public final static long TYPE_MASK = (maskstart >> TYPE_BIT_SIZE -1) >>> 63-TYPE_START_BIT;

    public final static long OFFSET_MAX_VALUE = (1 << OFFSET_BIT_SIZE) -1;
    public final static long RESERVED_MAX_VALUE = (1 << RESERVED_BIT_SIZE) -1;
    public final static long QNAME_MAX_VALUE = (1 << QNAME_BIT_SIZE) -1;
    public final static long PREFIX_MAX_VALUE = (1 << PREFIX_BIT_SIZE) -1;
    public final static long DEPTH_MAX_VALUE = (1 << DEPTH_BIT_SIZE )-1;
    public final static long TYPE_MAX_VALUE = (1 << TYPE_BIT_SIZE) -1;

    public final static int VTD_START_BIT = TYPE_START_BIT;
    public final static int VTD_BIT_SIZE = VTD_START_BIT + 1;
    public final static long VTD_MASK = (maskstart >> VTD_BIT_SIZE -1) >>> 63-VTD_START_BIT;
    public final static long VTD_MAX_VALUE = Long.MAX_VALUE;

    public final static int TOKEN_BIT_SIZE = PREFIX_BIT_SIZE + QNAME_BIT_SIZE;
    public final static int TOKEN_START_BIT = PREFIX_START_BIT;
    public final static long TOKEN_MASK = (maskstart >> TOKEN_BIT_SIZE -1) >>> 63-TOKEN_START_BIT;
    public final static long TOKEN_MAX_VALUE = (1 << TOKEN_BIT_SIZE) -1;
    public final static int TOKEN_SHIFT_BITS = QNAME_SHIFT_BITS;

    public static long getOffset(long vtd)
    {
        return (OFFSET_MASK & vtd) >> OFFSET_SHIFT_BITS;
    }

    public static long getQName(long vtd)
    {
        return (QNAME_MASK & vtd) >> QNAME_SHIFT_BITS;
    }

    public static long getPrefix(long vtd)
    {
        return (PREFIX_MASK & vtd) >> PREFIX_SHIFT_BITS;
    }

    public static long getDepth(long vtd)
    {
        return (DEPTH_MASK & vtd) >> DEPTH_SHIFT_BITS;
    }

    public static TokenType getType(long vtd)
    {
        return TokenType.valueOf((int)((TYPE_MASK & vtd) >> TYPE_SHIFT_BITS));
    }

    public static long packns(long offset, long qname, long prefix, long depth, TokenType type)
    {
        offset = offset << OFFSET_SHIFT_BITS;
        qname = qname << QNAME_SHIFT_BITS;
        prefix = prefix << PREFIX_SHIFT_BITS;
        depth = depth << DEPTH_SHIFT_BITS;
        long typel = (long)(type.getIntValue()) << TYPE_SHIFT_BITS;

        return offset | qname | prefix | depth | typel; 
    }

    public static long pack(long offset, long qname, long depth, TokenType type)
    {
        offset = offset << OFFSET_SHIFT_BITS;
        qname = qname << QNAME_SHIFT_BITS;
        depth = depth << DEPTH_SHIFT_BITS;
        long typel = (long)(type.getIntValue()) << TYPE_SHIFT_BITS;

        return offset | qname | depth | typel; 
    }

    public static void main(String[] arg)
    {
        System.out.print("Offset::");
        System.out.print(" Size:");
        System.out.print(OFFSET_BIT_SIZE);
        System.out.print(" Max:");
        System.out.print(OFFSET_MAX_VALUE);
        System.out.print(" Mask:");
        System.out.println(Long.toHexString(OFFSET_MASK));

        System.out.print("Reserved::");
        System.out.print(" Size:");
        System.out.print(RESERVED_BIT_SIZE);
        System.out.print(" Max:");
        System.out.print(RESERVED_MAX_VALUE);
        System.out.print(" Mask:");
        System.out.println(Long.toHexString(RESERVED_MASK));

        System.out.print("QName::");
        System.out.print(" Size:");
        System.out.print(QNAME_BIT_SIZE);
        System.out.print(" Max:");
        System.out.print(QNAME_MAX_VALUE);
        System.out.print(" Mask:");
        System.out.println(Long.toHexString(QNAME_MASK));

        System.out.print("Prefix::");
        System.out.print(" Size:");
        System.out.print(PREFIX_BIT_SIZE);
        System.out.print(" Max:");
        System.out.print(PREFIX_MAX_VALUE);
        System.out.print(" Mask:");
        System.out.println(Long.toHexString(PREFIX_MASK));

        System.out.print("Token::");
        System.out.print(" Size:");
        System.out.print(TOKEN_BIT_SIZE);
        System.out.print(" Max:");
        System.out.print(TOKEN_MAX_VALUE);
        System.out.print(" Mask:");
        System.out.println(Long.toHexString(TOKEN_MASK));

        System.out.print("Depth::");
        System.out.print(" Size:");
        System.out.print(DEPTH_BIT_SIZE);
        System.out.print(" Max:");
        System.out.print(DEPTH_MAX_VALUE);
        System.out.print(" Mask:");
        System.out.println(Long.toHexString(DEPTH_MASK));

        System.out.print("Type::");
        System.out.print(" Size:");
        System.out.print(TYPE_BIT_SIZE);
        System.out.print(" Max:");
        System.out.print(TYPE_MAX_VALUE);
        System.out.print(" Mask:");
        System.out.println(Long.toHexString(TYPE_MASK));

        System.out.print("VTD::");
        System.out.print(" Size:");
        System.out.print(VTD_BIT_SIZE);
        System.out.print(" Max:");
        System.out.print(VTD_MAX_VALUE);
        System.out.print(" Mask:");
        System.out.println(Long.toHexString(VTD_MASK));

        long vtd = packns(25,2,3,6,TokenType.TOKEN_STARTING_TAG);
        System.out.print("SAMPLE VTD::");
        System.out.println(Long.toHexString(vtd));
        System.out.print("OFFSET::");
        System.out.println(getOffset(vtd));
        System.out.print("Type::");
        System.out.println(getType(vtd));



    }

}
