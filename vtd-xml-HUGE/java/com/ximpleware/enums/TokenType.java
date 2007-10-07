package com.ximpleware.enums;

/**
 * // token type definitions
 * public final static int TOKEN_STARTING_TAG = 0;
 * public final static int TOKEN_ENDING_TAG = 1;
 * public final static int TOKEN_ATTR_NAME = 2;
 * public final static int TOKEN_ATTR_NS = 3;
 * public final static int TOKEN_ATTR_VAL = 4;
 * public final static int TOKEN_CHARACTER_DATA = 5;
 * public final static int TOKEN_COMMENT = 6;
 * public final static int TOKEN_PI_NAME = 7;
 * public final static int TOKEN_PI_VAL = 8;
 * public final static int TOKEN_DEC_ATTR_NAME = 9;
 * public final static int TOKEN_DEC_ATTR_VAL = 10;
 * public final static int TOKEN_CDATA_VAL = 11;
 * public final static int TOKEN_DTD_VAL = 12;
 * public final static int TOKEN_DOCUMENT =13;
 */
public enum TokenType
{
    TOKEN_STARTING_TAG(0),
    TOKEN_ENDING_TAG(1),
    TOKEN_ATTR_NAME(2),
    TOKEN_ATTR_NS(3),
    TOKEN_ATTR_VAL(4),
    TOKEN_CHARACTER_DATA(5),
    TOKEN_COMMENT(6),
    TOKEN_PI_NAME(7),
    TOKEN_PI_VAL(8),
    TOKEN_DEC_ATTR_NAME(9),
    TOKEN_DEC_ATTR_VAL(10),
    TOKEN_CDATA_VAL(11),
    TOKEN_DTD_VAL(12),
    TOKEN_DOCUMENT(13);

    TokenType(int val)
    {
        this.intVal = val;
    }

    private final int intVal;

    public int getIntValue()
    {
        return intVal;
    }

    public static TokenType valueOf(int intValue)
    {
        for (TokenType tt : values())
        {
            if (tt.getIntValue() == intValue)
            {
                return tt;
            }
        }
        throw new IllegalArgumentException("Cannot parse into an element of TokenType : '" + intValue + "'");
    }
}


