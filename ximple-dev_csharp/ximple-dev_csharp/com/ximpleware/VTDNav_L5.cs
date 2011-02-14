using System;
using System.Collections.Generic;
using System.Text;
using com.ximpleware;

namespace com.ximpleware
{
    public class VTDNav_L5 : com.ximpleware.VTDNav
    {

        protected internal int l4index;
        protected internal int l5index; 
        protected internal int l4upper;
        protected internal int l4lower;
        protected internal int l5upper;
        protected internal int l5lower;

        protected new internal FastLongBuffer l3Buffer;
        protected internal FastLongBuffer l4Buffer;
        protected internal FastIntBuffer l5Buffer;
        /**
         * Initialize the VTD navigation object.
         * 
         * @param RootIndex
         *            int
         * @param maxDepth
         *            int
         * @param encoding
         *            int
         * @param NS
         *            boolean
         * @param x
         *            byte[]
         * @param vtd
         *            com.ximpleware.ILongBuffer
         * @param l1
         *            com.ximpleware.ILongBuffer
         * @param l2
         *            com.ximpleware.ILongBuffer
         * @param l3
         *            com.ximpleware.IIntBuffer
         * @param so
         *            int starting offset of the document(in byte)
         * @param length
         *            int length of the document (in byte)
         */
        protected internal VTDNav_L5(
            int RootIndex,
            int enc,
            bool NS,
            int depth,
            IByteBuffer x,
            FastLongBuffer vtd,
            FastLongBuffer l1,
            FastLongBuffer l2,
            FastLongBuffer l3,
            FastLongBuffer l4,
            FastIntBuffer l5,
            int so, // start offset of the starting offset(in byte)
            int length) // lengnth of the XML document (in byte))
        {
            //super();
            // initialize all buffers
            if (l1 == null
                || l2 == null
                || l3 == null
                || vtd == null
                || x == null
                || depth < 0
                || RootIndex < 0 //|| encoding <= FORMAT_UTF8
                //|| encoding >= FORMAT_ISO_8859_1
                || so < 0
                || length < 0)
            {
                throw new System.ArgumentException();
            }

            l1Buffer = l1;
            l2Buffer = l2;
            l3Buffer = l3;
            l4Buffer = l4;
            l5Buffer = l5;
            vtdBuffer = vtd;
            XMLDoc = x;

            encoding = enc;
            //System.out.println("encoding " + encoding);
            rootIndex = RootIndex;
            nestingLevel = depth + 1;
            ns = NS; // namespace aware or not
            if (ns == false)
                MASK_TOKEN_OFFSET = 0x000000007fffffffL; // this allows xml size to
            // be 2GB
            else // if there is no namespace
                MASK_TOKEN_OFFSET = 0x000000003fffffffL;


            atTerminal = false; //this variable will only change value during XPath
            // eval

            // initialize the context object
            this.context = new int[nestingLevel];
            //depth value is the first entry in the context because root is
            // singular.
            context[0] = 0;
            //set the value to zero
            for (int i = 1; i < nestingLevel; i++)
            {
                context[i] = -1;
            }
            //currentOffset = 0;
            //contextStack = new ContextBuffer(1024, nestingLevel + 7);
            contextStack = new ContextBuffer(10, nestingLevel + 15);
            contextStack2 = new ContextBuffer(10, nestingLevel + 15);
            stackTemp = new int[nestingLevel + 15];

            // initial state of LC variables
            l1index = l2index = l3index = l4index = l5index = -1;
            l2lower = l3lower = l4lower = l5lower = -1;
            l2upper = l3upper = l4upper = l5upper = -1;
            docOffset = so;
            docLen = length;
            //System.out.println("offset " + offset + " length " + length);
            //printL2Buffer();
            vtdSize = vtd.size_Renamed_Field;
            //writeOffsetAdjustment = false;
            //recentNS = -1;
            name = null;
            nameIndex = -1;
            localName = null;
            localNameIndex = -1;
            fib = new FastIntBuffer(5); // page size is 32 ints
            shallowDepth = false;
            maxLCDepthPlusOne = 6;

        }

        /**
         * Clone the VTDNav instance to get with shared XML, VTD and LC buffers
         * The node position is also copied from the original instance
         * @return a new instance of VTDNav
         */
        public override VTDNav cloneNav()
        {
            VTDNav_L5 vn = new VTDNav_L5(rootIndex,
                    encoding,
                    ns,
                    nestingLevel - 1,
                    XMLDoc,
                    vtdBuffer,
                    l1Buffer,
                    l2Buffer,
                    l3Buffer,
                    l4Buffer,
                    l5Buffer,
                    docOffset,
                    docLen
                    );
            vn.atTerminal = this.atTerminal;
            vn.LN = this.LN;
            if (this.context[0] != -1)
                Array.Copy(this.context, 0, vn.context, 0, this.context[0] + 1);
            else
                vn.context[0] = -1;
            vn.l1index = l1index;
            if (getCurrentDepth() > 1)
            {
                vn.l2index = this.l2index;
                vn.l2upper = l2upper;
                vn.l2lower = l2lower;
            }
            if (getCurrentDepth() > 2)
            {
                vn.l3lower = l3lower;
                vn.l3index = l3index;
                vn.l3upper = l3upper;
            }
            if (getCurrentDepth() > 3)
            {
                vn.l4lower = l4lower;
                vn.l4index = l4index;
                vn.l4upper = l4upper;
            }
            if (getCurrentDepth() > 4)
            {
                vn.l5lower = l5lower;
                vn.l5index = l5index;
                vn.l5upper = l5upper;
            }
            return vn;
        }

        /**
         * Duplicate the VTDNav instance with shared XML, VTD and LC buffers
         * This method may be useful for parallel XPath evaluation
         * The node Position is at root element
         * @return a VTDNav instance
         *
         */
        public override VTDNav duplicateNav()
        {
            return new VTDNav_L5(rootIndex,
                    encoding,
                    ns,
                    nestingLevel - 1,
                    XMLDoc,
                    vtdBuffer,
                    l1Buffer,
                    l2Buffer,
                    l3Buffer,
                    l4Buffer,
                    l5Buffer,
                    docOffset,
                    docLen
                    );
        }

        /**
         * This method is similar to getElementByName in DOM except it doesn't
         * return the nodeset, instead it iterates over those nodes . When URL is
         * "*" it will match any namespace if ns is false, return false immediately
         * 
         * @return boolean
         * @param dp
         *            int (The depth of the starting position before iterating)
         * @param URL
         *            java.lang.String
         * @param ln
         *            java.lang.String
         * @exception com.ximpleware.NavException
         *                The exception if the underlying byte content contains
         *                various errors. Notice that we are being conservative in
         *                making little assumption on the correctness of underlying
         *                byte content. This is because VTD records can be generated
         *                by another machine from a load-balancer..
         * @exception IllegalArguementException
         *                if ln is null example
         * 
         * int depth = nv.getCurrentDepth() while(iterateNS(depth,
         * "www.url.com","node_name")){ push(); // store the current position //move
         * position safely pop(); // load the position }
         */
        protected internal override bool iterateNS(int dp, String URL, String ln)
        {
            if (ns == false)
                return false;
            int tokenType;
            int index = getCurrentIndex() + 1;
            while (index < vtdSize)
            {
                tokenType = getTokenType(index);
                if (tokenType == VTDNav.TOKEN_ATTR_NAME
                        || tokenType == VTDNav.TOKEN_ATTR_NS)
                {
                    index = index + 2;
                    continue;
                }
                if (isElementOrDocument(index))
                {
                    int depth = getTokenDepth(index);
                    if (depth > dp)
                    {
                        context[0] = depth;
                        if (depth > 0)
                            context[depth] = index;
                        if (matchElementNS(URL, ln))
                        {
                            if (dp < 6)
                                resolveLC();
                            return true;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
                index++;
            }
            return false;
        }

        /**
         * Sync up the current context with location cache. This operation includes
         * finding out l1index, l2index, l3index and restores upper and lower bound
         * info To improve efficieny this method employs some heuristic search
         * algorithm. The result is that it is quite close to direct access.
         * Creation date: (11/16/03 7:44:53 PM)
         * 
         * @return int The index of the NS URL
         */
        protected override void resolveLC()
        {
            if (context[0] <= 0)
                return;
            resolveLC_l1();
            if (context[0] == 1)
                return;
            resolveLC_l2();
            if (context[0] == 2)
                return;
            resolveLC_l3();
            if (context[0] == 3)
                return;
            resolveLC_l4();
            if (context[0] == 4)
                return;
            resolveLC_l5();
        }

        /**
         * Sync L3 location Cache
         */
        protected override void resolveLC_l3()
        {
            int temp = l2Buffer.lower32At(l2index);
            if (l3lower != temp)
            {
                l3lower = temp;
                // l2lower shouldn't be -1 !!!! l2lower and l2upper always get
                // resolved simultaneously
                l3index = l3lower;
                l3upper = l3Buffer.size_Renamed_Field - 1;
                for (int i = l2index + 1; i < l2Buffer.size_Renamed_Field; i++)
                {
                    temp = l2Buffer.lower32At(i);
                    if (temp != -1)
                    {
                        l3upper = temp - 1;
                        break;
                    }
                }
            } // intelligent guess again ??

            if (l3index < 0 || l3index >= l3Buffer.size_Renamed_Field
                    || context[3] != l3Buffer.upper32At(l3index))
            {

                if (l3index >= l3Buffer.size_Renamed_Field || l3index < 0)
                    l3index = l3lower;
                if (l3index + 1 < l3Buffer.size_Renamed_Field && context[3] == l3Buffer.upper32At(l3index + 1))
                    l3index = l3index + 1;
                else if (l3upper - l3lower >= 16)
                {
                    int init_guess = l3lower
                            + (int)((l3upper - l3lower)
                                    * ((float)context[3] - l3Buffer
                                            .upper32At(l3lower)) / (l3Buffer
                                    .upper32At(l3upper) - l3Buffer
                                    .upper32At(l3lower)));
                    if (l3Buffer.upper32At(init_guess) > context[3])
                    {
                        while (context[3] != l3Buffer.upper32At(init_guess))
                            init_guess--;
                    }
                    else if (l3Buffer.upper32At(init_guess) < context[3])
                    {
                        while (context[3] != l3Buffer.upper32At(init_guess))
                            init_guess++;
                    }
                    l3index = init_guess;
                }
                else if (context[3] < l3Buffer.upper32At(l3index))
                {

                    while (context[3] != l3Buffer.upper32At(l3index))
                    {
                        l3index--;
                    }
                }
                else
                {
                    while (context[3] != l3Buffer.upper32At(l3index))
                        l3index++;
                }
            }
        }

        private void resolveLC_l4()
        {
            int temp = l3Buffer.lower32At(l3index);
            if (l4lower != temp)
            {
                l4lower = temp;
                // l2lower shouldn't be -1 !!!! l2lower and l2upper always get
                // resolved simultaneously
                l4index = l4lower;
                l4upper = l4Buffer.size_Renamed_Field - 1;
                for (int i = l3index + 1; i < l3Buffer.size_Renamed_Field; i++)
                {
                    temp = l3Buffer.lower32At(i);
                    if (temp != -1)
                    {
                        l4upper = temp - 1;
                        break;
                    }
                }
            } // intelligent guess again ??

            if (l4index < 0 || l4index >= l4Buffer.size_Renamed_Field
                    || context[4] != l4Buffer.upper32At(l4index))
            {

                if (l4index >= l4Buffer.size_Renamed_Field || l4index < 0)
                    l4index = l4lower;
                if (l4index + 1 < l4Buffer.size_Renamed_Field && context[4] == l4Buffer.upper32At(l4index + 1))
                    l4index = l4index + 1;
                else if (l4upper - l4lower >= 16)
                {
                    int init_guess = l4lower
                            + (int)((l4upper - l4lower)
                                    * ((float)context[4] - l4Buffer
                                            .upper32At(l4lower)) / (l4Buffer
                                    .upper32At(l4upper) - l4Buffer
                                    .upper32At(l4lower)));
                    if (l4Buffer.upper32At(init_guess) > context[4])
                    {
                        while (context[4] != l4Buffer.upper32At(init_guess))
                            init_guess--;
                    }
                    else if (l4Buffer.upper32At(init_guess) < context[4])
                    {
                        while (context[4] != l4Buffer.upper32At(init_guess))
                            init_guess++;
                    }
                    l4index = init_guess;
                }
                else if (context[4] < l4Buffer.upper32At(l4index))
                {

                    while (context[4] != l4Buffer.upper32At(l4index))
                    {
                        l4index--;
                    }
                }
                else
                {
                    while (context[4] != l4Buffer.upper32At(l4index))
                        l4index++;
                }
            }
        }
        /**
         * Sync L3 location Cache
         */
        private void resolveLC_l5()
        {
            int temp = l4Buffer.lower32At(l4index);
            if (l5lower != temp)
            {
                //l3lower and l3upper are always together
                l5lower = temp;
                // l3lower shouldn't be -1
                l5index = l5lower;
                l5upper = l5Buffer.size_Renamed_Field - 1;
                for (int i = l4index + 1; i < l4Buffer.size_Renamed_Field; i++)
                {
                    temp = l4Buffer.lower32At(i);
                    if (temp != -1)
                    {
                        l5upper = temp - 1;
                        break;
                    }
                }
            }

            if (l5index < 0 || l5index >= l5Buffer.size_Renamed_Field
                    || context[5] != l5Buffer.intAt(l5index))
            {
                if (l5index >= l5Buffer.size_Renamed_Field || l5index < 0)
                    l5index = l5lower;
                if (l5index + 1 < l5Buffer.size_Renamed_Field &&
                        context[5] == l5Buffer.intAt(l5index + 1))
                    l5index = l5index + 1;
                else if (l5upper - l5lower >= 16)
                {
                    int init_guess = l5lower
                            + (int)((l5upper - l5lower) * ((float)(context[5] - l5Buffer
                                    .intAt(l5lower)) / (l5Buffer.intAt(l5upper) - l5Buffer
                                    .intAt(l5lower))));
                    if (l5Buffer.intAt(init_guess) > context[5])
                    {
                        while (context[5] != l5Buffer.intAt(init_guess))
                            init_guess--;
                    }
                    else if (l5Buffer.intAt(init_guess) < context[5])
                    {
                        while (context[5] != l5Buffer.intAt(init_guess))
                            init_guess++;
                    }
                    l5index = init_guess;
                }
                else if (context[5] < l5Buffer.intAt(l5index))
                {
                    while (context[5] != l5Buffer.intAt(l5index))
                    {
                        l5index--;
                    }
                }
                else
                {
                    while (context[5] != l5Buffer.intAt(l5index))
                    {
                        l5index++;
                    }
                }
            }
        }

        /**
         * Load the context info from ContextBuffer. Info saved including LC and
         * current state of the context
         * 
         * @return boolean
         *  
         */
        public override bool pop()
        {
            bool b = contextStack.load(stackTemp);
            if (b == false)
                return false;
            for (int i = 0; i < nestingLevel; i++)
            {
                context[i] = stackTemp[i];
            }

            l1index = stackTemp[nestingLevel];
            l2index = stackTemp[nestingLevel + 1];
            l3index = stackTemp[nestingLevel + 2];
            l4index = stackTemp[nestingLevel + 3];
            l5index = stackTemp[nestingLevel + 4];
            l2lower = stackTemp[nestingLevel + 5];
            l2upper = stackTemp[nestingLevel + 6];
            l3lower = stackTemp[nestingLevel + 7];
            l3upper = stackTemp[nestingLevel + 8];
            l4lower = stackTemp[nestingLevel + 9];
            l4upper = stackTemp[nestingLevel + 10];
            l5lower = stackTemp[nestingLevel + 11];
            l5upper = stackTemp[nestingLevel + 12];
            atTerminal = (stackTemp[nestingLevel + 13] == 1);
            LN = stackTemp[nestingLevel + 14];
            return true;
        }

        /**
         * Load the context info from contextStack2. This method is dedicated for
         * XPath evaluation.
         * 
         * @return status of pop2
         */


        protected internal override bool pop2()
        {

            bool b = contextStack2.load(stackTemp);
            if (b == false)
                return false;
            for (int i = 0; i < nestingLevel; i++)
            {
                context[i] = stackTemp[i];
            }
            l1index = stackTemp[nestingLevel];
            l2index = stackTemp[nestingLevel + 1];
            l3index = stackTemp[nestingLevel + 2];
            l4index = stackTemp[nestingLevel + 3];
            l5index = stackTemp[nestingLevel + 4];
            l2lower = stackTemp[nestingLevel + 5];
            l2upper = stackTemp[nestingLevel + 6];
            l3lower = stackTemp[nestingLevel + 7];
            l3upper = stackTemp[nestingLevel + 8];
            l4lower = stackTemp[nestingLevel + 9];
            l4upper = stackTemp[nestingLevel + 10];
            l5lower = stackTemp[nestingLevel + 11];
            l5upper = stackTemp[nestingLevel + 12];
            atTerminal = (stackTemp[nestingLevel + 13] == 1);
            LN = stackTemp[nestingLevel + 14];
            return true;
        }

        /**
         * Store the context info into the ContextBuffer. Info saved including LC
         * and current state of the context Creation date: (11/16/03 7:00:27 PM)
         */
        public override void push()
        {

            for (int i = 0; i < nestingLevel; i++)
            {
                stackTemp[i] = context[i];
            }
            stackTemp[nestingLevel] = l1index;
            stackTemp[nestingLevel + 1] = l2index;
            stackTemp[nestingLevel + 2] = l3index;
            stackTemp[nestingLevel + 3] = l4index;
            stackTemp[nestingLevel + 4] = l5index;
            stackTemp[nestingLevel + 5] = l2lower;
            stackTemp[nestingLevel + 6] = l2upper;
            stackTemp[nestingLevel + 7] = l3lower;
            stackTemp[nestingLevel + 8] = l3upper;
            stackTemp[nestingLevel + 9] = l4lower;
            stackTemp[nestingLevel + 10] = l4upper;
            stackTemp[nestingLevel + 11] = l5lower;
            stackTemp[nestingLevel + 12] = l5upper;

            if (atTerminal)
                stackTemp[nestingLevel + 13] = 1;
            else
                stackTemp[nestingLevel + 13] = 0;
            stackTemp[nestingLevel + 14] = LN;
            contextStack.store(stackTemp);
        }

        /**
         * Store the context info into the contextStack2. This method is reserved
         * for XPath Evaluation
         *  
         */

        protected internal override void push2()
        {

            for (int i = 0; i < nestingLevel; i++)
            {
                stackTemp[i] = context[i];
            }
            stackTemp[nestingLevel] = l1index;
            stackTemp[nestingLevel + 1] = l2index;
            stackTemp[nestingLevel + 2] = l3index;
            stackTemp[nestingLevel + 3] = l4index;
            stackTemp[nestingLevel + 4] = l5index;
            stackTemp[nestingLevel + 5] = l2lower;
            stackTemp[nestingLevel + 6] = l2upper;
            stackTemp[nestingLevel + 7] = l3lower;
            stackTemp[nestingLevel + 8] = l3upper;
            stackTemp[nestingLevel + 9] = l4lower;
            stackTemp[nestingLevel + 10] = l4upper;
            stackTemp[nestingLevel + 11] = l5lower;
            stackTemp[nestingLevel + 12] = l5upper;

            if (atTerminal)
                stackTemp[nestingLevel + 13] = 1;
            else
                stackTemp[nestingLevel + 13] = 0;
            stackTemp[nestingLevel + 14] = LN;
            contextStack2.store(stackTemp);
        }


        protected override void recoverNode_l3(int index)
        {
            int i = l2Buffer.lower32At(l2index);

            if (l3lower != i)
            {
                l3lower = i;
                // l2lower shouldn't be -1 !!!! l2lower and l2upper always get
                // resolved simultaneously
                //l2index = l2lower;
                l3upper = l3Buffer.size_Renamed_Field - 1;
                for (int k = l2index + 1; k < l2Buffer.size_Renamed_Field; k++)
                {
                    i = l2Buffer.lower32At(k);
                    if (i != -1)
                    {
                        l3upper = i - 1;
                        break;
                    }
                }
            }
            // guess what i would be in l2 cache
            int t1 = l3Buffer.upper32At(l3lower);
            int t2 = l3Buffer.upper32At(l3upper);
            //System.out.print("   t2  ==>"+t2+"   t1  ==>"+t1);
            i = Math.Min(l3lower + (int)(((float)(index - t1) / (t2 - t1 + 1)) * (l3upper - l3lower)), l3upper);
            //System.out.print("  i1  "+i);
            while (i < l3Buffer.size_Renamed_Field - 1 && l3Buffer.upper32At(i) < index)
            {
                i++;
            }
            //System.out.println(" ==== i2    "+i+"    index  ==>  "+index);

            while (l3Buffer.upper32At(i) > index && i > 0)
                i--;
            context[3] = l3Buffer.upper32At(i);
            l3index = i;
            //System.out.println("l2lower ==>"+l2lower+"  l2upper==>"+l2upper+"   l2index==> "+l2index);
        }

        protected void recoverNode_l4(int index)
        {
            int i = l3Buffer.lower32At(l3index);

            if (l4lower != i)
            {
                l4lower = i;
                // l2lower shouldn't be -1 !!!! l2lower and l2upper always get
                // resolved simultaneously
                //l2index = l2lower;
                l4upper = l4Buffer.size_Renamed_Field - 1;
                for (int k = l3index + 1; k < l3Buffer.size_Renamed_Field; k++)
                {
                    i = l3Buffer.lower32At(k);
                    if (i != -1)
                    {
                        l4upper = i - 1;
                        break;
                    }
                }
            }
            // guess what i would be in l2 cache
            int t1 = l4Buffer.upper32At(l4lower);
            int t2 = l4Buffer.upper32At(l4upper);
            //System.out.print("   t2  ==>"+t2+"   t1  ==>"+t1);
            i = Math.Min(l4lower + (int)(((float)(index - t1) / (t2 - t1 + 1)) * (l4upper - l4lower)), l4upper);
            //System.out.print("  i1  "+i);
            while (i < l4Buffer.size_Renamed_Field- 1 && l4Buffer.upper32At(i) < index)
            {
                i++;
            }
            //System.out.println(" ==== i2    "+i+"    index  ==>  "+index);

            while (l4Buffer.upper32At(i) > index && i > 0)
                i--;
            context[4] = l4Buffer.upper32At(i);
            l4index = i;
            //System.out.println("l2lower ==>"+l2lower+"  l2upper==>"+l2upper+"   l2index==> "+l2index);
        }

        protected void recoverNode_l5(int index)
        { //l3
            int i = l4Buffer.lower32At(l4index);

            if (l5lower != i)
            {
                //l3lower and l3upper are always together
                l5lower = i;
                // l3lower shouldn't be -1
                //l3index = l3lower;
                l5upper = l5Buffer.size_Renamed_Field - 1;
                for (int k = l4index + 1; k < l4Buffer.size_Renamed_Field; k++)
                {
                    i = l4Buffer.lower32At(k);
                    if (i != -1)
                    {
                        l5upper = i - 1;
                        break;
                    }
                }
            }
            int t1 = l5Buffer.intAt(l5lower);
            int t2 = l5Buffer.intAt(l5upper);
            i = Math.Min(l5lower + (int)(((float)(index - t1) / (t2 - t1 + 1)) * (l5upper - l5lower)), l5upper);
            while (i < l5Buffer.size_Renamed_Field- 1 && l5Buffer.intAt(i) < index)
            {
                i++;
            }
            while (l5Buffer.intAt(i) > index && i > 0)
                i--;
            //System.out.println(" i ===> "+i);
            context[5] = l5Buffer.intAt(i);
            l5index = i;
        }

        /**
         * This is for debugging purpose
         * 
         * @param fib
         */

        public override void sampleState(FastIntBuffer fib)
        {
            //		for(int i=0;i<context.)
            //			context[i] = -1;
            //		fib.append(context);
            if (context[0] >= 1)
                fib.append(l1index);
            else return;

            if (context[0] >= 2)
            {
                fib.append(l2index);
                fib.append(l2lower);
                fib.append(l2upper);
            }
            else return;

            if (context[0] >= 3)
            {
                fib.append(l3index);
                fib.append(l3lower);
                fib.append(l3upper);
            }
            else return;

            if (context[0] >= 4)
            {
                fib.append(l4index);
                fib.append(l4lower);
                fib.append(l4upper);
            }
            else return;

            if (context[0] >= 5)
            {
                fib.append(l5index);
                fib.append(l5lower);
                fib.append(l5upper);
            }
        }

        /**
         * This function is called by selectElement_F in autoPilot
         * 
         * @param en
         *            ElementName
         * @param special
         *            whether it is a node()
         * @return boolean
         * @throws NavException
         */

        /*protected internal override bool iterate_following(String en, bool special)
        {
            int index = getCurrentIndex() + 1;
            //int size = vtdBuffer.size;
            while (index < vtdSize)
            {
                if (isElementOrDocument(index))
                {
                    int depth = getTokenDepth(index);
                    context[0] = depth;
                    if (depth > 0)
                        context[depth] = index;
                    if (special || matchElement(en))
                    {
                        if (depth < 6)
                            resolveLC();
                        return true;
                    }
                }
                index++;
            }
            return false;
        }*/

        /**
         * This function is called by selectElementNS_F in autoPilot
         * 
         * @param URL
         * @param ln
         * @return boolean
         * @throws NavException
         */
        /*protected internal override bool iterate_followingNS(String URL, String ln)
        {
            int index = getCurrentIndex() + 1;
            //int size = vtdBuffer.size;
            while (index < vtdSize)
            {
                if (isElementOrDocument(index))
                {
                    int depth = getTokenDepth(index);
                    context[0] = depth;
                    if (depth > 0)
                        context[depth] = index;
                    if (matchElementNS(URL, ln))
                    {
                        if (depth < 6)
                            resolveLC();
                        return true;
                    }
                }
                index++;
            }
            return false;
        }*/

        /**
         * This function is called by selectElement_P in autoPilot
         * 
         * @param en
         *            element Name
         * @param a
         *            context of current position
         * @param special
         *            whether the test type is node()
         * @return boolean
         * @throws NavException
         */
        /*protected internal override bool iterate_preceding(String en, int[] a, bool special)
        {
            int index = getCurrentIndex() - 1;
            int t, d;
            //int depth = getTokenDepth(index);
            //int size = vtdBuffer.size;
            while (index > 0)
            {
                if (isElementOrDocument(index))
                {
                    int depth = getTokenDepth(index);
                    context[0] = depth;
                    //context[depth]=index;
                    if (depth > 0)
                    {
                        context[depth] = index;
                        t = index - 1;
                        for (int i = depth - 1; i > 0; i--)
                        {
                            if (context[i] > index || context[i] == -1)
                            {
                                while (t > 0)
                                {
                                    d = getTokenDepth(t);
                                    if (d == i && isElement(t))
                                    {
                                        context[i] = t;
                                        break;
                                    }
                                    t--;
                                }
                            }
                            else
                                break;
                        }
                    }
                    //dumpContext();
                    if (index != a[depth] && (special || matchElement(en)))
                    {
                        if (depth < 6)
                            resolveLC();
                        return true;
                    }
                }
                index--;
            }
            return false;
        }*/

        /**
         * This function is called by selectElementNS_P in autoPilot
         * 
         * @param URL
         * @param ln
         * @return boolean
         * @throws NavException
         */
        /*protected internal override bool iterate_precedingNS(String URL, String ln, int[] a)
        {
            int index = getCurrentIndex() - 1;
            int t, d;
            //int depth = getTokenDepth(index);
            //int size = vtdBuffer.size;
            while (index > 0)
            {
                if (isElementOrDocument(index))
                {
                    int depth = getTokenDepth(index);
                    context[0] = depth;
                    //context[depth]=index;
                    if (depth > 0)
                    {
                        context[depth] = index;
                        t = index - 1;
                        for (int i = depth - 1; i > 0; i--)
                        {
                            if (context[i] > index || context[i] == -1)
                            {
                                while (t > 0)
                                {
                                    d = getTokenDepth(t);
                                    if (d == i && isElement(t))
                                    {
                                        context[i] = t;
                                        break;
                                    }
                                    t--;
                                }
                            }
                            else
                                break;
                        }
                    }
                    //dumpContext();
                    if (index != a[depth] && matchElementNS(URL, ln))
                    {
                        if (depth < 6)
                            resolveLC();
                        return true;
                    }
                }
                index--;
            }
            return false;
        }*/

        /**
         * This method is similar to getElementByName in DOM except it doesn't
         * return the nodeset, instead it iterates over those nodes. Notice that
         * this method is called by the "iterate" method in the Autopilot class. "*"
         * will match any element Creation date: (12/2/03 2:31:20 PM)
         * 
         * @return boolean
         * @param dp
         *            int (The depth of the starting position before iterating)
         * @param en
         *            java.lang.String
         * @exception com.ximpleware.NavException
         *                The exception is signaled if the underlying byte content
         *                contains various errors. Notice that we are being
         *                conservative in making little assumption on the
         *                correctness of underlying byte content. This is because
         *                VTD records can be generated by another machine from a
         *                load-balancer. null element name allowed represent
         *                node()in XPath;
         */
        /*protected internal override bool iterate(int dp, String en, bool special)
        { // the navigation doesn't rely on LC
            // get the current depth
            int index = getCurrentIndex() + 1;
            int tokenType;
            //int size = vtdBuffer.size;
            while (index < vtdSize)
            {
                tokenType = getTokenType(index);
                if (tokenType == VTDNav.TOKEN_ATTR_NAME
                        || tokenType == VTDNav.TOKEN_ATTR_NS)
                {
                    index = index + 2;
                    continue;
                }
                if (isElementOrDocument(index))
                {
                    int depth = getTokenDepth(index);
                    if (depth > dp)
                    {
                        context[0] = depth;
                        if (depth > 0)
                            context[depth] = index;
                        if (special || matchElement(en))
                        {
                            if (dp < 6)
                                resolveLC();
                            return true;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
                index++;

            }
            return false;
        }*/

        /**
         * A generic navigation method. Move the cursor to the element according to
         * the direction constants and the element name If no such element, no
         * position change and return false. "*" matches any element Creation date:
         * (12/2/03 1:43:50 PM) Legal direction constants are <br>
         * 
         * <pre>
         * 		ROOT            0  
         * </pre>
         * <pre>
         * 		PARENT          1  
         * </pre>
         * <pre>
         * 		FIRST_CHILD     2  
         * </pre>
         * <pre>
         * 		LAST_CHILD      3  
         * </pre>
         * <pre>
         * 		NEXT_SIBLING    4  
         * </pre>
         * <pre>
         * 		PREV_SIBLING    5  
         * </pre>
         * 
         * <br>
         * for ROOT and PARENT, element name will be ignored.
         * 
         * @return boolean
         * @param direction
         *            int
         * @param en
         *            String
         * @exception com.ximpleware.NavException
         *                When direction value is illegal. Or there are errors in
         *                underlying byte representation of the document
         * @exception IllegalArguementException
         *                if en is null
         */
        public override bool toElement(int direction, String en)
        {
            int temp;
            int d;
            int val = 0;
            if (en == null)
                throw new System.ArgumentException(" Element name can't be null ");
            if (en.Equals("*"))
                return toElement(direction);
            switch (direction)
            {
                case ROOT:
                    return toElement(ROOT);

                case PARENT:
                    return toElement(PARENT);

                case FIRST_CHILD:
                    if (atTerminal) return false;
                    if (toElement(FIRST_CHILD) == false)
                        return false;
                    // check current element name
                    if (matchElement(en) == false)
                    {
                        if (toElement(NEXT_SIBLING, en) == true)
                            return true;
                        else
                        {
                            //toParentElement();
                            //context[context[0]] = 0xffffffff;
                            context[0]--;
                            return false;
                        }
                    }
                    else
                        return true;

                case LAST_CHILD:
                    if (atTerminal) return false;
                    if (toElement(LAST_CHILD) == false)
                        return false;
                    if (matchElement(en) == false)
                    {
                        if (toElement(PREV_SIBLING, en) == true)
                            return true;
                        else
                        {
                            //context[context[0]] = 0xffffffff;
                            context[0]--;
                            //toParentElement();
                            return false;
                        }
                    }
                    else
                        return true;

                case NEXT_SIBLING:
                    if (atTerminal) return false;
                    d = context[0];

                    switch (d)
                    {
                        case -1:
                        case 0: return false;
                        case 1: val = l1index; break;
                        case 2: val = l2index; break;
                        case 3: val = l3index; break;
                        case 4: val = l4index; break;
                        case 5: val = l5index; break;
                        default: break;
                    }
                    temp = context[d]; // store the current position

                    while (toElement(NEXT_SIBLING))
                    {
                        if (matchElement(en))
                        {
                            return true;
                        }
                    }
                    switch (d)
                    {
                        case 1: l1index = val; break;
                        case 2: l2index = val; break;
                        case 3: l3index = val; break;
                        case 4: l4index = val; break;
                        case 5: l5index = val; break;
                        default: break;
                    }
                    context[d] = temp;
                    return false;

                case PREV_SIBLING:
                    if (atTerminal) return false;
                    d = context[0];
                    switch (d)
                    {
                        case -1:
                        case 0: return false;
                        case 1: val = l1index; break;
                        case 2: val = l2index; break;
                        case 3: val = l3index; break;
                        case 4: val = l4index; break;
                        case 5: val = l5index; break;
                        default: break;
                    }
                    temp = context[d]; // store the current position

                    while (toElement(PREV_SIBLING))
                    {
                        if (matchElement(en))
                        {
                            return true;
                        }
                    }
                    switch (d)
                    {
                        case 1: l1index = val; break;
                        case 2: l2index = val; break;
                        case 3: l3index = val; break;
                        case 4: l4index = val; break;
                        case 5: l5index = val; break;
                        default: break;
                    }
                    context[d] = temp;
                    return false;

                default:
                    throw new NavException("illegal navigation options");
            }
        }

        /**
         * A generic navigation method. Move the cursor to the element according to
         * the direction constants If no such element, no position change and return
         * false. Creation date: (12/2/03 1:43:50 PM) Legal direction constants are
         * 
         * <pre>
         *    			ROOT               0 
         * </pre>	
         *<pre>
         *  		    PARENT  		   1 
         * </pre>
         *<pre>
         *        	    FIRST_CHILD		   2 
         * </pre>  
         *<pre>
         *  		    LAST_CHILD 		   3 
         * </pre>
         *<pre>
         *     	  	    NEXT_SIBLING       4 
         * </pre>
         *<pre>
         *       	    PREV_SIBLING       5 
         * </pre>
         * 
         * @return boolean
         * @param direction
         *            int
         * @exception com.ximpleware.NavException
         *                When direction value is illegal.
         */
        public override bool toElement(int direction)
        {
            int size;
            switch (direction)
            {
                case ROOT: // to document element!
                    if (context[0] != 0)
                    {
                        /*
                         * for (int i = 1; i <= context[0]; i++) { context[i] =
                         * 0xffffffff; }
                         */
                        context[0] = 0;
                    }
                    atTerminal = false;
                    l1index = l2index = l3index = -1;
                    return true;
                case PARENT:
                    if (atTerminal == true)
                    {
                        atTerminal = false;
                        return true;
                    }
                    if (context[0] > 0)
                    {
                        //context[context[0]] = context[context[0] + 1] =
                        // 0xffffffff;
                        context[context[0]] = -1;
                        context[0]--;
                        return true;
                    }
                    else if (context[0] == 0)
                    {
                        context[0] = -1; //to be compatible with XPath Data model
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                case FIRST_CHILD:
                case LAST_CHILD:
                    if (atTerminal) return false;
                    switch (context[0])
                    {
                        case -1:
                            context[0] = 0;
                            return true;
                        case 0:
                            if (l1Buffer.size_Renamed_Field > 0)
                            {
                                context[0] = 1;
                                l1index =
                                    (direction == FIRST_CHILD)
                                        ? 0
                                        : (l1Buffer.size_Renamed_Field - 1);
                                context[1] = l1Buffer.upper32At(l1index);
                                //(int) (vtdToken >> 32);
                                return true;
                            }
                            else
                                return false;
                        case 1:
                            l2lower = l1Buffer.lower32At(l1index);
                            if (l2lower == -1)
                            {
                                return false;
                            }
                            context[0] = 2;
                            l2upper = l2Buffer.size_Renamed_Field - 1;
                            size = l1Buffer.size_Renamed_Field;
                            for (int i = l1index + 1; i < size; i++)
                            {
                                int temp = l1Buffer.lower32At(i);
                                if (temp != -1)
                                {
                                    l2upper = temp - 1;
                                    break;
                                }
                            }
                            //System.out.println(" l2 upper: " + l2upper + " l2
                            // lower : " + l2lower);
                            l2index =
                                (direction == FIRST_CHILD) ? l2lower : l2upper;
                            context[2] = l2Buffer.upper32At(l2index);
                            return true;

                        case 2:
                            l3lower = l2Buffer.lower32At(l2index);
                            if (l3lower == -1)
                            {
                                return false;
                            }
                            context[0] = 3;
                            l3upper = l3Buffer.size_Renamed_Field- 1;
                            size = l2Buffer.size_Renamed_Field;
                            for (int i = l2index + 1; i < size; i++)
                            {
                                int temp = l2Buffer.lower32At(i);
                                if (temp != -1)
                                {
                                    l3upper = temp - 1;
                                    break;
                                }
                            }
                            //System.out.println(" l2 upper: " + l2upper + " l2
                            // lower : " + l2lower);
                            l3index =
                                (direction == FIRST_CHILD) ? l3lower : l3upper;
                            context[3] = l3Buffer.upper32At(l3index);
                            return true;

                        case 3:
                            l4lower = l3Buffer.lower32At(l3index);
                            if (l4lower == -1)
                            {
                                return false;
                            }
                            context[0] = 4;
                            l4upper = l4Buffer.size_Renamed_Field - 1;
                            size = l3Buffer.size_Renamed_Field;
                            for (int i = l3index + 1; i < size; i++)
                            {
                                int temp = l3Buffer.lower32At(i);
                                if (temp != -1)
                                {
                                    l4upper = temp - 1;
                                    break;
                                }
                            }
                            //System.out.println(" l2 upper: " + l2upper + " l2
                            // lower : " + l2lower);
                            l4index =
                                (direction == FIRST_CHILD) ? l4lower : l4upper;
                            context[4] = l4Buffer.upper32At(l4index);
                            return true;

                        case 4:
                            l5lower = l4Buffer.lower32At(l4index);
                            if (l5lower == -1)
                            {
                                return false;
                            }
                            context[0] = 5;

                            l5upper = l5Buffer.size_Renamed_Field- 1;
                            size = l4Buffer.size_Renamed_Field;
                            for (int i = l4index + 1; i < size; i++)
                            {
                                int temp = l4Buffer.lower32At(i);
                                if (temp != -1)
                                {
                                    l5upper = temp - 1;
                                    break;
                                }
                            }
                            //System.out.println(" l3 upper : " + l3upper + " l3
                            // lower : " + l3lower);
                            l5index =
                                (direction == FIRST_CHILD) ? l5lower : l5upper;
                            context[5] = l5Buffer.intAt(l5index);

                            return true;

                        default:
                            if (direction == FIRST_CHILD)
                            {
                                size = vtdBuffer.size_Renamed_Field;
                                int index = context[context[0]] + 1;
                                while (index < size)
                                {
                                    long temp = vtdBuffer.longAt(index);
                                    int token_type =
                                        (int)((MASK_TOKEN_TYPE & temp) >> 60)
                                            & 0xf;

                                    if (token_type == TOKEN_STARTING_TAG)
                                    {
                                        int depth =
                                            (int)((MASK_TOKEN_DEPTH & temp) >> 52);
                                        if (depth <= context[0])
                                        {
                                            return false;
                                        }
                                        else if (depth == (context[0] + 1))
                                        {
                                            context[0] += 1;
                                            context[context[0]] = index;
                                            return true;
                                        }
                                    }

                                    index++;
                                } // what condition
                                return false;
                            }
                            else
                            {
                                int index = context[context[0]] + 1;
                                int last_index = -1;
                                size = vtdBuffer.size_Renamed_Field;
                                while (index < size)
                                {
                                    long temp = vtdBuffer.longAt(index);
                                    int depth =
                                        (int)((MASK_TOKEN_DEPTH & temp) >> 52);
                                    int token_type =
                                        (int)((MASK_TOKEN_TYPE & temp) >> 60)
                                            & 0xf;

                                    if (token_type == TOKEN_STARTING_TAG)
                                    {
                                        if (depth <= context[0])
                                        {
                                            break;
                                        }
                                        else if (depth == (context[0] + 1))
                                        {
                                            last_index = index;
                                        }
                                    }

                                    index++;
                                }
                                if (last_index == -1)
                                {
                                    return false;
                                }
                                else
                                {
                                    context[0] += 1;
                                    context[context[0]] = last_index;
                                    return true;
                                }
                            }
                    }

                case NEXT_SIBLING:
                case PREV_SIBLING:
                    if (atTerminal) return false;
                    switch (context[0])
                    {
                        case -1:
                        case 0:
                            return false;
                        case 1:
                            if (direction == NEXT_SIBLING)
                            {
                                if (l1index + 1 >= l1Buffer.size_Renamed_Field)
                                {
                                    return false;
                                }

                                l1index++; // global incremental
                            }
                            else
                            {
                                if (l1index - 1 < 0)
                                {
                                    return false;
                                }
                                l1index--; // global incremental
                            }
                            context[1] = l1Buffer.upper32At(l1index);
                            return true;
                        case 2:
                            if (direction == NEXT_SIBLING)
                            {
                                if (l2index + 1 > l2upper)
                                {
                                    return false;
                                }
                                l2index++;
                            }
                            else
                            {
                                if (l2index - 1 < l2lower)
                                {
                                    return false;
                                }
                                l2index--;
                            }
                            context[2] = l2Buffer.upper32At(l2index);
                            return true;

                        case 3:
                            if (direction == NEXT_SIBLING)
                            {
                                if (l3index + 1 > l3upper)
                                {
                                    return false;
                                }
                                l3index++;
                            }
                            else
                            {
                                if (l3index - 1 < l3lower)
                                {
                                    return false;
                                }
                                l3index--;
                            }
                            context[3] = l3Buffer.upper32At(l3index);
                            return true;

                        case 4:
                            if (direction == NEXT_SIBLING)
                            {
                                if (l4index + 1 > l4upper)
                                {
                                    return false;
                                }
                                l4index++;
                            }
                            else
                            {
                                if (l4index - 1 < l4lower)
                                {
                                    return false;
                                }
                                l4index--;
                            }
                            context[4] = l4Buffer.upper32At(l4index);
                            return true;
                        case 5:
                            if (direction == NEXT_SIBLING)
                            {
                                if (l5index + 1 > l5upper)
                                {
                                    return false;
                                }
                                l5index++;
                            }
                            else
                            {
                                if (l5index - 1 < l5lower)
                                {
                                    return false;
                                }
                                l5index--;
                            }
                            context[5] = l5Buffer.intAt(l5index);
                            return true;
                        default:
                            //int index = context[context[0]] + 1;

                            if (direction == NEXT_SIBLING)
                            {
                                int index = context[context[0]] + 1;
                                size = vtdBuffer.size_Renamed_Field;
                                while (index < size)
                                {
                                    long temp = vtdBuffer.longAt(index);
                                    int token_type =
                                        (int)((MASK_TOKEN_TYPE & temp) >> 60)
                                            & 0xf;

                                    if (token_type == TOKEN_STARTING_TAG)
                                    {
                                        int depth =
                                            (int)((MASK_TOKEN_DEPTH & temp) >> 52);
                                        if (depth < context[0])
                                        {
                                            return false;
                                        }
                                        else if (depth == (context[0]))
                                        {
                                            context[context[0]] = index;
                                            return true;
                                        }
                                    }
                                    index++;
                                }
                                return false;
                            }
                            else
                            {
                                int index = context[context[0]] - 1;
                                while (index > context[context[0] - 1])
                                {
                                    // scan backforward
                                    long temp = vtdBuffer.longAt(index);
                                    int token_type =
                                        (int)((MASK_TOKEN_TYPE & temp) >> 60)
                                            & 0xf;

                                    if (token_type == TOKEN_STARTING_TAG)
                                    {
                                        int depth =
                                            (int)((MASK_TOKEN_DEPTH & temp) >> 52);
                                        /*
                                         * if (depth < context[0]) { return false; }
                                         * else
                                         */
                                        if (depth == (context[0]))
                                        {
                                            context[context[0]] = index;
                                            return true;
                                        }
                                    }
                                    index--;
                                } // what condition
                                return false;
                            }
                    }

                default:
                    throw new NavException("illegal navigation options");
            }

        }

        /**
         * A generic navigation method with namespace support. Move the cursor to
         * the element according to the direction constants and the prefix and local
         * names If no such element, no position change and return false. URL *
         * matches any namespace, including undefined namespaces a null URL means
         * hte namespace prefix is undefined for the element ln * matches any
         * localname Creation date: (12/2/03 1:43:50 PM) Legal direction constants
         * are <br>
         * 
         * <pre>
         * 		ROOT            0  
         * </pre>
         * <pre>
         * 		PARENT          1  
         * </pre>
         * <pre>
         * 		FIRST_CHILD     2  
         * </pre>
         * <pre>
         * 		LAST_CHILD      3  
         * </pre>
         * <pre>
         * 		NEXT_SIBLING    4  
         * </pre>
         * <pre>
         * 		PREV_SIBLING    5  
         * </pre>
         * 
         * <br>
         * for ROOT and PARENT, element name will be ignored. If not ns enabled,
         * return false immediately with no position change.
         * 
         * @return boolean
         * @param direction
         *            int
         * @param URL
         *            String
         * @param ln
         *            String
         * @exception com.ximpleware.NavException
         *                When direction value is illegal. Or there are errors in
         *                underlying byte representation of the document
         */
        public override bool toElementNS(int direction, String URL, String ln)
        {
            int temp;
            int val = 0;
            int d; // temp location
            if (ns == false)
                return false;
            switch (direction)
            {
                case ROOT:
                    return toElement(ROOT);

                case PARENT:
                    return toElement(PARENT);

                case FIRST_CHILD:
                    if (atTerminal) return false;
                    if (toElement(FIRST_CHILD) == false)
                        return false;
                    // check current element name
                    if (matchElementNS(URL, ln) == false)
                    {
                        if (toElementNS(NEXT_SIBLING, URL, ln) == true)
                            return true;
                        else
                        {
                            //toParentElement();
                            //context[context[0]] = 0xffffffff;
                            context[0]--;
                            return false;
                        }
                    }
                    else
                        return true;

                case LAST_CHILD:
                    if (atTerminal) return false;
                    if (toElement(LAST_CHILD) == false)
                        return false;
                    if (matchElementNS(URL, ln) == false)
                    {
                        if (toElementNS(PREV_SIBLING, URL, ln) == true)
                            return true;
                        else
                        {
                            //context[context[0]] = 0xffffffff;
                            context[0]--;
                            //toParentElement();
                            return false;
                        }
                    }
                    else
                        return true;

                case NEXT_SIBLING:
                    if (atTerminal) return false;
                    d = context[0];
                    temp = context[d]; // store the current position
                    switch (d)
                    {
                        case -1:
                        case 0: return false;
                        case 1: val = l1index; break;
                        case 2: val = l2index; break;
                        case 3: val = l3index; break;
                        case 4: val = l4index; break;
                        case 5: val = l5index; break;
                        default: break;
                    }
                    //if (d == 0)
                    //	return false;
                    while (toElement(NEXT_SIBLING))
                    {
                        if (matchElementNS(URL, ln))
                        {
                            return true;
                        }
                    }
                    switch (d)
                    {
                        case 1: l1index = val; break;
                        case 2: l2index = val; break;
                        case 3: l3index = val; break;
                        case 4: val = l4index; break;
                        case 5: val = l5index; break;
                        default: break;
                    }
                    context[d] = temp;
                    return false;

                case PREV_SIBLING:
                    if (atTerminal) return false;
                    d = context[0];
                    temp = context[d]; // store the current position
                    switch (d)
                    {
                        case -1:
                        case 0: return false;
                        case 1: val = l1index; break;
                        case 2: val = l2index; break;
                        case 3: val = l3index; break;
                        case 4: val = l4index; break;
                        case 5: val = l5index; break;
                        default: break;
                    }
                    //if (d == 0)
                    //	return false;
                    while (toElement(PREV_SIBLING))
                    {
                        if (matchElementNS(URL, ln))
                        {
                            return true;
                        }
                    }
                    switch (d)
                    {
                        case 1: l1index = val; break;
                        case 2: l2index = val; break;
                        case 3: l3index = val; break;
                        case 4: val = l4index; break;
                        case 5: val = l5index; break;
                        default: break;
                    }
                    context[d] = temp;
                    return false;

                default:
                    throw new NavException("illegal navigation options");
            }
        }

        /**
         * This method takes a vtd index, and recover its correspondin
         * node position, the index can only be of node type element,
         * document, attribute name, attribute value or character data,
         * or CDATA
         * @param index
         * @throws NavException
         */
        public override void recoverNode(int index)
        {
            if (index < 0 || index >= vtdSize)
                throw new NavException("Invalid VTD index");

            int type = getTokenType(index);
            if (//type == VTDNav.TOKEN_COMMENT ||
                //	type == VTDNav.TOKEN_PI_NAME ||
                    type == VTDNav.TOKEN_PI_VAL ||
                    type == VTDNav.TOKEN_DEC_ATTR_NAME ||
                    type == VTDNav.TOKEN_DEC_ATTR_VAL ||
                    type == VTDNav.TOKEN_ATTR_VAL)
                throw new NavException("Token type not yet supported");

            // get depth
            int d = getTokenDepth(index);
            // handle document node;
            switch (d)
            {
                case -1:
                    context[0] = -1;
                    if (index != 0)
                    {
                        LN = index;
                        atTerminal = true;
                    }
                    return;
                case 0:
                    context[0] = 0;
                    if (index != rootIndex)
                    {
                        LN = index;
                        atTerminal = true;
                    }
                    return;
            }
            context[0] = d;
            if (type != VTDNav.TOKEN_STARTING_TAG)
            {
                LN = index;
                atTerminal = true;
            }
            // search LC level 1
            recoverNode_l1(index);

            if (d == 1)
                return;
            // search LC level 2
            recoverNode_l2(index);
            if (d == 2)
            {
                //resolveLC();
                return;
            }
            // search LC level 3
            recoverNode_l3(index);
            if (d == 3)
            {
                //resolveLC();
                return;
            }

            recoverNode_l4(index);
            if (d == 4)
            {
                //resolveLC();
                return;
            }

            recoverNode_l5(index);
            if (d == 5)
            {
                //resolveLC();
                return;
            }

            // scan backward
            if (type == VTDNav.TOKEN_STARTING_TAG)
            {
                context[d] = index;
            }
            else
            {
                int t = index - 1;
                while (!(getTokenType(t) == VTDNav.TOKEN_STARTING_TAG &&
                        getTokenDepth(t) == d))
                {
                    t--;
                }
                context[d] = t;
            }
            int t1 = context[d] - 1;
            d--;
            while (d > 5)
            {
                while (!(getTokenType(t1) == VTDNav.TOKEN_STARTING_TAG &&
                        getTokenDepth(t1) == d))
                {
                    t1--;
                }
                context[d] = t1;
                d--;
            }
            //resolveLC();		
        }
        public override void writeIndex(System.IO.Stream os)
        {
            IndexHandler.writeIndex_L5(1,
                 this.encoding,
                 this.ns,
                 true,
                 this.nestingLevel - 1,
                 5,
                 this.rootIndex,
                 this.XMLDoc.getBytes(),
                 this.docOffset,
                 this.docLen,
                 (FastLongBuffer)this.vtdBuffer,
                 (FastLongBuffer)this.l1Buffer,
                 (FastLongBuffer)this.l2Buffer,
                 (FastLongBuffer)this.l3Buffer,
                 (FastLongBuffer)this.l4Buffer,
                 (FastIntBuffer)this.l5Buffer,
                 os);
        }

        public override void writeSeparateIndex(System.IO.Stream os)
        {
            IndexHandler.writeSeparateIndex_L5(1,
                 this.encoding,
                 this.ns,
                 true,
                 this.nestingLevel - 1,
                 5,
                 this.rootIndex,
                 //this.XMLDoc.getBytes(),
                 this.docOffset,
                 this.docLen,
                 (FastLongBuffer)this.vtdBuffer,
                 (FastLongBuffer)this.l1Buffer,
                 (FastLongBuffer)this.l2Buffer,
                 (FastLongBuffer)this.l3Buffer,
                 (FastLongBuffer)this.l4Buffer,
                 (FastIntBuffer)this.l5Buffer,
                 os);
        }
    }
}
