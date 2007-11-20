/*
 * Created on Nov 19, 2007
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package com.ximpleware.parser;

/**
 * @author jimmy zhang
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class ISO8859_15 {    
    static final char[] chars = new char[0x100];
    static {
        for (int i=0;i<256;i++){
            chars[i] = (char)i;
        }
        chars[0xA4]= 0x20AC;
        chars[0xA6]= 0x0160;
        chars[0xA8]= 0x0161;
        chars[0xB4]= 0x017D;
        chars[0xB8]= 0x017E;
        chars[0xBC]= 0x0152;
        chars[0xBD]= 0x0153;
        chars[0xBE]= 0x0178;        
    }
    public static char decode(byte b){
        return chars[b & 0xff];
    }
}
