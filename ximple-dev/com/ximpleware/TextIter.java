package com.ximpleware;

/* 
 * Copyright (C) 2002-2004 XimpleWare, info@ximpleware.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
//import com.ximpleware.FastLongBuffer;
//import com.ximpleware.FastIntBuffer;
//import com.ximpleware.NavException;
//import java.io.UnsupportedEncodingException;

/**
 * This class iterates through all text nodes of an element.
 * VTDNav has getText() which is inadequate for mixed content style of XML.
 * text nodes include character_data and CDATA.
 * Creation date: (12/5/03 5:53:41 PM)
 */
public class TextIter {

    private int prevLocation; //previous location of text node
    protected int depth;
    protected int index; // this is index for the element

    protected VTDNav vn;

    private int lcIndex;
    private int lcLower;
    private int lcUpper;
    /**
     * TextIter constructor comment.
     */
    public TextIter() {
        super();
        /*sel_char_data = true;
        sel_comment = true;
        sel_cdata = true;*/
    }
/**
 * Get the index val for the text node according to action type.
 * Creation date: (12/5/03 6:11:50 PM)
 * @return int  (-1 if no more left)
 * @param action int
 */
public int getNext() throws TextIterException {
    int vtdSize = vn.vtdBuffer.size();
    switch (depth) {
        case 0 :
            // scan forward, if none found, jump to level 1 element and scan backward until one is found
            // if there isn't a level-one element, jump to the end of vtd buffer and scan backward
            int sp = (prevLocation != -1) ? prevLocation + 1 : vn.rootIndex + 1;
            if (vn.l1Buffer.size() != 0) {
                int temp1 = vn.l1Buffer.upper32At(0);
                int temp2 = vn.l1Buffer.upper32At(vn.l1Buffer.size() - 1);
                lcIndex = (lcIndex != -1) ? lcIndex : 0;
                while (sp < vtdSize) {
                    if (sp >= temp1 && sp < temp2) {
                        int s = vn.l1Buffer.upper32At(lcIndex);
                        if (sp == s) { // get to the next l1 element then do a rewind
                            lcIndex++;
                            sp = vn.l1Buffer.upper32At(lcIndex) - 1;
                            while (vn.getTokenDepth(sp) == 1
                                && vn.getTokenType(sp) != VTDNav.TOKEN_STARTING_TAG) { //probe depth in here
                                sp--;
                            }
                            sp++; // point to the first possible node
                            continue;
                        }
                        if (isText(sp) == true) {
                            prevLocation = sp;
                            return sp;
                        }
                        sp++;
                    } else if (sp < temp1) {
                        if (isText(sp) == true) {
                            prevLocation = sp;
                            return sp;
                        }
                        sp++;
                    } else {
                        if (sp == temp2) { // get to the end of the document and do a rewind
                            sp = vn.vtdBuffer.size() - 1;
                            while (vn.getTokenDepth(sp) == 1) {
                                sp--;
                            }
                            sp++;
                            continue;
                        }
                        if (isText(sp) == true) {
                            prevLocation = sp;
                            return sp;
                        }
                        sp++;
                    }
                }
                return -1;
                // found nothing
            } else {
                // no child element for root, just scan right forward
                while (sp < vtdSize) {
                    if (isText(sp) == true) {
                        prevLocation = sp;
                        return sp;
                    }
                    sp++;
                }
                return -1;
            }
        case 1 :
            if (prevLocation != -1) {
                sp = prevLocation + 1;
            } else {
                // fetch lclower and lcupper
                lcLower = vn.l1Buffer.lower32At(vn.l1index);
                if (lcLower != -1) {
                    lcUpper = vn.l2Buffer.size() - 1;
                    int size = vn.l1Buffer.size();
                    for (int i = lcLower + 1; i < size - 1; i++) {
                        int temp = vn.l1Buffer.lower32At(i);
                        if (temp != 0xffffffff) {
                            lcUpper = temp - 1;
                            break;
                        }
                    }
                }
                sp = vn.context[1] + 1;
            } // check for l2lower and l2upper

            if (lcLower != -1) { // have at least one child element
                int temp1 = vn.l2Buffer.upper32At(lcLower);
                int temp2 = vn.l2Buffer.upper32At(lcUpper);
                lcIndex = (lcIndex != -1) ? lcIndex : lcLower;
                while (sp < vtdSize) {
                    int s = vn.l2Buffer.upper32At(lcIndex);
                    if (sp >= temp1 && sp < temp2) {
                        if (sp == s) {
                            lcIndex++;
                            sp = vn.l2Buffer.upper32At(lcIndex) - 1;
                            while (vn.getTokenDepth(sp) == 1) {
                                sp--;
                            }
                            sp++;
                            continue;
                        }
                        if (isText(sp) == true) {
                            prevLocation = sp;
                            return sp;
                        }
                    } else if (sp < temp1) {
                        if (isText(sp) == true) {
                            prevLocation = sp;
                            return sp;
                        }
                    } else {
                        //if (sp == temp2) { // last child element
                        //} else 
                        if (isText(sp) == true && vn.getTokenDepth(sp) == 1) {
                            prevLocation = sp;
                            return sp;
                        } else if (vn.getTokenDepth(sp) == 0) {
                            break;
                        }
                    }
                    sp++;
                }
                return -1;
            } else { // no child element
                int depth = vn.getTokenDepth(sp);
                int type = vn.getTokenType(sp);
                while (sp < vtdSize
                    && depth >= 1
                    && !(depth == 1 && type == VTDNav.TOKEN_STARTING_TAG)) {
                    if (isText(sp) == true) {
                        prevLocation = sp;
                        return sp;
                    }
                    depth = vn.getTokenDepth(sp);
                    type = vn.getTokenType(sp);
                    sp++;
                }
                return -1;
            }

        case 2 :
            if (prevLocation != -1) {
                sp = prevLocation + 1;
            } else {
                // fetch lclower and lcupper
                lcLower = vn.l2Buffer.lower32At(vn.l2index);
                if (lcLower != -1) {
                    lcUpper = vn.l3Buffer.size() - 1;
                    int size = vn.l2Buffer.size();
                    for (int i = lcLower + 1; i < size - 1; i++) {
                        int temp = vn.l1Buffer.lower32At(i);
                        if (temp != 0xffffffff) {
                            lcUpper = temp - 1;
                            break;
                        }
                    }
                }
                sp = vn.context[2] + 1;
            } // check for l3lower and l3upper

            if (lcLower != -1) { // at least one child element
                int temp1 = vn.l3Buffer.intAt(lcLower);
                int temp2 = vn.l3Buffer.intAt(lcUpper);
                lcIndex = (lcIndex != -1) ? lcIndex : lcLower;
                while (sp < vtdSize) {
                    int s = vn.l3Buffer.intAt(lcIndex);
                    //int s = vn.l2Buffer.upper32At(lcIndex);
                    if (sp >= temp1 && sp < temp2) {
                        if (sp == s) {
                            lcIndex++;
                            sp = vn.l3Buffer.intAt(lcIndex) - 1;
                            while (vn.getTokenDepth(sp) == 2) {
                                sp--;
                            }
                            sp++;
                            continue;
                        }
                        if (isText(sp) == true) {
                            prevLocation = sp;
                            return sp;
                        }
                    } else if (sp < temp1) {
                        if (isText(sp) == true) {
                            prevLocation = sp;
                            return sp;
                        }
                    } else {
                        //if (sp == temp2) { // last child element
                        //} else 
                        if (isText(sp) == true && vn.getTokenDepth(sp) == 2) {
                            prevLocation = sp;
                            return sp;
                        } else if (vn.getTokenDepth(sp) < 2) {
                            break;
                        }
                    }
                    sp++;
                }
                return -1;
            } else { // no child elements
                int depth = vn.getTokenDepth(sp);
                int type = vn.getTokenType(sp);
                while (sp < vtdSize
                    && depth >= 2
                    && !(depth == 2 && type == VTDNav.TOKEN_STARTING_TAG)) {
                    // the last condition indicates the start of the next sibling element
                    if (isText(sp) == true) {
                        prevLocation = sp;
                        return sp;
                    }
                    depth = vn.getTokenDepth(sp);
                    type = vn.getTokenType(sp);
                    sp++;
                }
                return -1;
            }

        default :
            int curDepth = vn.context[0];
            sp = (prevLocation != -1) ? prevLocation + 1 : vn.context[curDepth] + 1;
            int depth = vn.getTokenDepth(sp);
            int type = vn.getTokenType(sp);
            while (sp < vtdSize
                && depth >= curDepth
                && !(depth == curDepth && type == VTDNav.TOKEN_STARTING_TAG)) {
                if (isText(sp) == true) {
                    prevLocation = sp;
                    return sp;
                }
                depth = vn.getTokenDepth(sp);
                type = vn.getTokenType(sp);
                sp++;
            }
    }
    return -1;
}
/**
 * Test whether a give token type is a TEXT.
 * Creation date: (12/11/03 3:46:10 PM)
 * @return boolean
 * @param type int
 */
final private boolean isText(int index) {
    int type = vn.getTokenType(index);
    return (type == VTDNav.TOKEN_CHARACTER_DATA
    //|| type == vn.TOKEN_COMMENT
    || type == VTDNav.TOKEN_CDATA_VAL);
}
    /**
     * Obtain the current navigation position and element info from VTDNav.
     * Creation date: (12/5/03 6:20:44 PM)
     * @param vn com.ximpleware.VTDNav
     */
    public void touch(VTDNav v) {
        if (vn == null)
            throw new IllegalArgumentException(" VTDNav instance can't be null");

        depth = vn.context[0];
        index = (depth != 0) ? vn.context[0] : vn.rootIndex;

        vn = v;
        prevLocation = -1;
        lcIndex = -1;
        lcUpper = -1;
        lcLower = -1;
    }
}
