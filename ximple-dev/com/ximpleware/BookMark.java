/* 
 * Copyright (C) 2002-2007 XimpleWare, info@ximpleware.com
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
package com.ximpleware;
public class BookMark {
    VTDNav vn1;
    int ba[];
    /**
     * Constructor for BookMark
     * Take no argument
     * 
     */
    public BookMark(){
        ba = null;
        vn1 = null;
    }
    /**
     * bind a BookMark object to a VTDNav object
     * the cursor position is set to an invalid state
     * @param vn
     *
     */
    public void bind(VTDNav vn){
        if (vn==null)
            throw new IllegalArgumentException("vn can't be null");
        vn1 = vn;
        ba = new int[vn.nestingLevel + 9];    
        ba[0]= -2 ; // this would never happen in a VTDNav obj's context
    }
    
    /**
     * BookMark constructor with an instance of vn
     * as input
     * @param vn
     */
    public BookMark(VTDNav vn){
       bind(vn);
       recordCursorPosition(vn);
    }
    
    /**
     * setCursorPosition
     * This method can only set the cursor position
     * of an VTDNav object identical to its internal copy 
     * @param vn
     * @return
     *
     */
    public boolean setCursorPosition(VTDNav vn){
        if (vn1 != vn || ba == null || ba[0] == -2)
            return false;
        for (int i = 0; i < vn.nestingLevel; i++) {
			vn.context[i] = ba[i];
		}

		vn.l1index = ba[vn.nestingLevel];
		vn.l2index = ba[vn.nestingLevel + 1];
		vn.l3index = ba[vn.nestingLevel + 2];
		vn.l2lower = ba[vn.nestingLevel + 3];
		vn.l2upper = ba[vn.nestingLevel + 4];
		vn.l3lower = ba[vn.nestingLevel + 5];
		vn.l3upper = ba[vn.nestingLevel + 6];
		vn.atTerminal = (ba[vn.nestingLevel + 7] == 1);
		vn.LN = ba[vn.nestingLevel+8];
		return true;
    }
    /**
     * Record Cursor position
     * This method is implemented to be lenient on loading in
     * that it can load nodes from any VTDNav object
     * if vn is null, return false
     * 
     * @param vn
     * @return
     *
     */
    public boolean recordCursorPosition(VTDNav vn){
        if (vn == null)
            return false;
        if (vn== vn1){
            
        }else {
            bind(vn);
        }
        for (int i = 0; i < vn.nestingLevel; i++) {
            ba[i] = vn1.context[i];
		}

		ba[vn.nestingLevel]= vn.l1index ;
		ba[vn.nestingLevel + 1]= vn.l2index ;
		ba[vn.nestingLevel + 2]= vn.l3index ;
		ba[vn.nestingLevel + 3]= vn.l2lower ;
		ba[vn.nestingLevel + 4]= vn.l2upper ;
		ba[vn.nestingLevel + 5]= vn.l3lower ;
		ba[vn.nestingLevel + 6]= vn.l3upper ;
		ba[vn.nestingLevel + 7]=(vn.atTerminal == true)?1:0;
		ba[vn.nestingLevel + 8]= vn.LN;
        return true;
    }
    
      
    public final boolean deepEquals(BookMark bm2) {
        if (bm2.vn1 == this.vn1){
            if (bm2.ba[bm2.ba[0]]==this.ba[this.ba[0]])
                return true;
        }
        return false;

    }

    public final boolean equals(BookMark bm2) {
        if (this == bm2)
            return true;
        return deepEquals(bm2);
    }

    public final boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (!(obj instanceof BookMark))
            return false;
        return deepEquals((BookMark) obj);
    }
    
    public final int hashCode(){
        if (ba == null || vn1==null || ba[0]==-2)
            return -2;
        if (ba[0]==1)
            return vn1.rootIndex;
        return ba[ba[0]];
    }
}
