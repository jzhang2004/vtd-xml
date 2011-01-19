/* 
* Copyright (C) 2002-2011 XimpleWare, info@ximpleware.com
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

/* BookMark is based on (and inspired by) the concept and 
* implementation contributed by Rodrigo Cunha. 
* It corresponds to a single node position of VTDNav's cursor. 
* 
* 
* setCursorPosition(VTDNav vn) sets the node position of vn. 
* setCursorPosition() sets the node position of the BookMark object's embeddd 
* VTDNav object
* 
* recordCursorPosition(VTDNav vn) records the node position of the VTDNav
* Object.
* 
* recordCursorPosition() records the node position of the BookMark object's 
* embedded VTDNav object
* 
* BookMark(VTDNav vn) implicitly sets the node position for 
* the created BookMark instance.
*/
using System;
namespace com.ximpleware
{
    public class BookMark
    {
        /// <summary> This method returns the embedded VTDNav Object </summary>
        /// <returns> VTDNav
        /// 
        /// </returns>
        public VTDNav Nav
        {
            get
            {
                return vn1;
            }

        }
        internal VTDNav vn1; // the reference to the corresponding VTDNav object
        internal int[] ba;
        /// <summary> Constructor for BookMark
        /// Take no argument
        /// 
        /// </summary>
        public BookMark()
        {
            ba = null;
            vn1 = null;
        }

        /// <summary> separate a bookMark object apart from its embedded
        /// VTDNav instance 
        /// 
        /// </summary>
        public void unbind()
        {
            vn1 = null;
        }

        /// <summary> bind a BookMark object to a VTDNav object
        /// the cursor position is set to an invalid state
        /// </summary>
        /// <param name="vn">*
        /// </param>
        public void bind(VTDNav vn)
        {
            if (vn == null)
                throw new System.ArgumentException("vn can't be null");
            vn1 = vn;
            if (vn.shallowDepth)
            {
                if (ba == null || vn.nestingLevel + 8 != ba.Length)
                    ba = new int[vn.nestingLevel + 8];
            }
            else
            {
                if (ba == null || vn.nestingLevel + 14 != ba.Length)
                    ba = new int[vn.nestingLevel + 14];
            }
            ba[0] = -2; // this would never happen in a VTDNav obj's context
        }

        /// <summary> BookMark constructor with an instance of vn
        /// as input
        /// </summary>
        /// <param name="vn">
        /// </param>
        public BookMark(VTDNav vn)
        {
            bind(vn);
            recordCursorPosition(vn);
        }

        /// <summary> set cursor position
        /// This method can only set the cursor position
        /// of an VTDNav object identical to its internal copy 
        /// </summary>
        /// <param name="vn">
        /// </param>
        /// <returns>
        /// 
        /// </returns>
        public bool setCursorPosition(VTDNav vn)
        {
            if (vn1 != vn || ba == null || ba[0] == -2)
                return false;
            for (int i = 0; i < vn.nestingLevel; i++)
            {
                vn.context[i] = ba[i];
            }
            if (vn.shallowDepth)
            {
                vn.l1index = ba[vn.nestingLevel];
                vn.l2index = ba[vn.nestingLevel + 1];
                vn.l3index = ba[vn.nestingLevel + 2];
                vn.l2lower = ba[vn.nestingLevel + 3];
                vn.l2upper = ba[vn.nestingLevel + 4];
                vn.l3lower = ba[vn.nestingLevel + 5];
                vn.l3upper = ba[vn.nestingLevel + 6];
                if (ba[vn.nestingLevel + 7] < 0)
                {
                    vn.atTerminal = true;
                }
                else
                    vn.atTerminal = false;

                vn.LN = ba[vn.nestingLevel + 7] & 0x7fffffff;
            }
            else
            {
                VTDNav_L5 vnl = (VTDNav_L5)vn;
                vnl.l1index = ba[vn.nestingLevel];
                vnl.l2index = ba[vn.nestingLevel + 1];
                vnl.l3index = ba[vn.nestingLevel + 2];
                vnl.l4index = ba[vn.nestingLevel + 3];
                vnl.l5index = ba[vn.nestingLevel + 4];
                vnl.l2lower = ba[vn.nestingLevel + 5];
                vnl.l2upper = ba[vn.nestingLevel + 6];
                vnl.l3lower = ba[vn.nestingLevel + 7];
                vnl.l3upper = ba[vn.nestingLevel + 8];
                vnl.l4lower = ba[vn.nestingLevel + 9];
                vnl.l4upper = ba[vn.nestingLevel + 10];
                vnl.l5lower = ba[vn.nestingLevel + 11];
                vnl.l5upper = ba[vn.nestingLevel + 12];
                if (ba[vn.nestingLevel + 13] < 0)
                {
                    vn.atTerminal = true;
                }
                else
                    vn.atTerminal = false;

                vn.LN = ba[vn.nestingLevel + 13] & 0x7fffffff;
            }
            return true;
        }

        /// <summary> Set the cursor position of VTDNav object corresponding to the internal reference
        /// position of the embedded VTDNav object 
        /// </summary>
        /// <returns>
        /// 
        /// </returns>
        public bool setCursorPosition()
        {
            return setCursorPosition(vn1);
        }
        /// <summary> Record the cursor position
        /// This method is implemented to be lenient on loading in
        /// that it can load nodes from any VTDNav object
        /// if vn is null, return false
        /// 
        /// </summary>
        /// <param name="vn">
        /// </param>
        /// <returns>
        /// 
        /// </returns>
        public bool recordCursorPosition(VTDNav vn)
        {
            if (vn == null)
                return false;
            if (vn == vn1)
            {

            }
            else
            {
                bind(vn);
            }
            for (int i = 0; i < vn.nestingLevel; i++)
            {
                ba[i] = vn1.context[i];
            }
            if (vn.shallowDepth)
            {
                ba[vn.nestingLevel] = vn.l1index;
                ba[vn.nestingLevel + 1] = vn.l2index;
                ba[vn.nestingLevel + 2] = vn.l3index;
                ba[vn.nestingLevel + 3] = vn.l2lower;
                ba[vn.nestingLevel + 4] = vn.l2upper;
                ba[vn.nestingLevel + 5] = vn.l3lower;
                ba[vn.nestingLevel + 6] = vn.l3upper;
                //ba[vn.nestingLevel + 7]=(vn.atTerminal == true)?1:0;
                ba[vn.nestingLevel + 7] = (vn.atTerminal == true) ? (vn.LN | unchecked((int)0x80000000)) : vn.LN;
            }
            else
            {
                VTDNav_L5 vnl = (VTDNav_L5)vn;
                ba[vn.nestingLevel] = vnl.l1index;
                ba[vn.nestingLevel + 1] = vnl.l2index;
                ba[vn.nestingLevel + 2] = vnl.l3index;
                ba[vn.nestingLevel + 3] = vnl.l4index;
                ba[vn.nestingLevel + 4] = vnl.l5index;
                ba[vn.nestingLevel + 5] = vnl.l2lower;
                ba[vn.nestingLevel + 6] = vnl.l2upper;
                ba[vn.nestingLevel + 7] = vnl.l3lower;
                ba[vn.nestingLevel + 8] = vnl.l3upper;
                ba[vn.nestingLevel + 9] = vnl.l4lower;
                ba[vn.nestingLevel + 10] = vnl.l4upper;
                ba[vn.nestingLevel + 11] = vnl.l5lower;
                ba[vn.nestingLevel + 12] = vnl.l5upper;
                //ba[vn.nestingLevel + 7]=(vn.atTerminal == true)?1:0;
                ba[vn.nestingLevel + 13] =
                    (vn.atTerminal == true) ?
                        (vn.LN | unchecked((int)0x80000000)) : vn.LN;
            }
            return true;
        }
        /// <summary> Record cursor position of the VTDNav object as embedded in the
        /// bookmark
        /// 
        /// </summary>
        /// <returns>
        /// 
        /// </returns>
        public bool recordCursorPosition()
        {
            return recordCursorPosition(vn1);
        }
        public bool deepEquals(BookMark bm2)
        {
            if (bm2.vn1 == this.vn1)
            {
                if (bm2.ba[bm2.ba[0]] == this.ba[this.ba[0]])
                {
                    if (vn1.shallowDepth)
                    {
                        if (this.ba[this.vn1.nestingLevel + 7] < 0)
                        {
                            if (this.ba[this.vn1.nestingLevel + 7]
                                    != bm2.ba[this.vn1.nestingLevel + 7])
                                return false;
                        }
                    }
                    else
                    {
                        if (this.ba[this.vn1.nestingLevel + 13] < 0)
                        {
                            if (this.ba[this.vn1.nestingLevel + 13]
                                    != bm2.ba[this.vn1.nestingLevel + 13])
                                return false;
                        }
                    }
                    return true;
                }
            }
            return false;
        }

        public bool Equals(BookMark bm2)
        {
            if (this == bm2)
                return true;
            return deepEquals(bm2);
        }

        public override bool Equals(System.Object obj)
        {
            if (this == obj)
                return true;
            if (!(obj is BookMark))
                return false;
            return deepEquals((BookMark)obj);
        }

        public override int GetHashCode()
        {
            if (ba == null || vn1 == null || ba[0] == -2)
                return -2;
            if (ba[0] == 1)
                return vn1.rootIndex;
            return ba[ba[0]];
        }
        public bool compare(BookMark bm1)
        {

            /*for (int i = 0; i < vn1.nestingLevel; i++) {
                ba[i] = bm1.ba[i];
            }    	
            if (vn1.getCurrentDepth()>)*/
            if (vn1.shallowDepth)
            {
                for (int i = 0; i < vn1.nestingLevel + 7; i++)
                {
                    if (ba[i] != bm1.ba[i])
                        return false;
                }
                return true;
            }
            else
            {
                for (int i = 0; i < vn1.nestingLevel + 14; i++)
                {
                    if (ba[i] != bm1.ba[i])
                        return false;
                }
            }

            return true;
        }
    }
}
