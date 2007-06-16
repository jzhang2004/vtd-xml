using System;
using System.Collections.Generic;
using System.Text;
using com.ximpleware;
using System.IO;
// This example shows you how to create an index
namespace IndexWrite
{
    class IndexWrite
    {
        static void Main(string[] args)
        {
            try
            {
                VTDGen vg = new VTDGen();
                if (vg.parseFile("po.xml", true))
                {
                    vg.writeIndex("po.vxl");
                }
            }
            catch (Exception e)
            {
            }
        }
    }
}

