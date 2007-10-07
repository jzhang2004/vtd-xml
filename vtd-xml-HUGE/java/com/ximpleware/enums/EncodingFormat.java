package com.ximpleware.enums;

/**
 * // encoding format definition here
 * public final static int FORMAT_UTF8 = 2;
 * public final static int FORMAT_ASCII = 0;
 * <p/>
 * public final static int FORMAT_ISO_8859_1 = 1;
 * public final static int FORMAT_ISO_8859_2 = 3;
 * public final static int FORMAT_ISO_8859_3 = 4;
 * public final static int FORMAT_ISO_8859_4 = 5;
 * public final static int FORMAT_ISO_8859_5 = 6;
 * public final static int FORMAT_ISO_8859_6 = 7;
 * public final static int FORMAT_ISO_8859_7 = 8;
 * public final static int FORMAT_ISO_8859_8 = 9;
 * public final static int FORMAT_ISO_8859_9 = 10;
 * public final static int FORMAT_ISO_8859_10 = 11;
 * public final static int FORMAT_ISO_8859_11 = 12;
 * public final static int FORMAT_ISO_8859_12 = 13;
 * public final static int FORMAT_ISO_8859_13 = 14;
 * public final static int FORMAT_ISO_8859_14 = 15;
 * public final static int FORMAT_ISO_8859_15 = 16;
 * public final static int FORMAT_ISO_8859_16 = 17;
 * <p/>
 * public final static int FORMAT_WIN_1250 = 18;
 * public final static int FORMAT_WIN_1251 = 19;
 * public final static int FORMAT_WIN_1252 = 20;
 * public final static int FORMAT_WIN_1253 = 21;
 * public final static int FORMAT_WIN_1254 = 22;
 * public final static int FORMAT_WIN_1255 = 23;
 * public final static int FORMAT_WIN_1256 = 24;
 * public final static int FORMAT_WIN_1257 = 25;
 * public final static int FORMAT_WIN_1258 = 26;
 * <p/>
 * <p/>
 * public final static int FORMAT_UTF_16LE = 64;
 * public final static int FORMAT_UTF_16BE = 63;
 */
public enum EncodingFormat
{

    FORMAT_ASCII(0),

    FORMAT_ISO_8859_1(1),
    FORMAT_UTF8(2),
    FORMAT_ISO_8859_2(3),
    FORMAT_ISO_8859_3(4),
    FORMAT_ISO_8859_4(5),
    FORMAT_ISO_8859_5(6),
    FORMAT_ISO_8859_6(7),
    FORMAT_ISO_8859_7(8),
    FORMAT_ISO_8859_8(9),
    FORMAT_ISO_8859_9(10),
    FORMAT_ISO_8859_10(11),
    FORMAT_ISO_8859_11(12),
    FORMAT_ISO_8859_12(13),
    FORMAT_ISO_8859_13(14),
    FORMAT_ISO_8859_14(15),
    FORMAT_ISO_8859_15(16),
    FORMAT_ISO_8859_16(17),

    FORMAT_WIN_1250(18),
    FORMAT_WIN_1251(19),
    FORMAT_WIN_1252(20),
    FORMAT_WIN_1253(21),
    FORMAT_WIN_1254(22),
    FORMAT_WIN_1255(23),
    FORMAT_WIN_1256(24),
    FORMAT_WIN_1257(25),
    FORMAT_WIN_1258(26),

    FORMAT_UTF_16BE(63),
    FORMAT_UTF_16LE(64);

    private final int intVal;

    EncodingFormat(int val)
    {
        this.intVal = val;
    }

    public int getIntValue()
    {
        return intVal;
    }

    public static EncodingFormat valueOf(int intValue)
    {
        for (EncodingFormat ef : values())
        {
            if (ef.getIntValue() == intValue)
            {
                return ef;
            }
        }
        throw new IllegalArgumentException("Cannot parse into an element of EncodingFormat : '" + intValue + "'");
    }
}
