using System;
using System.Collections.Generic;
using System.Text;
using com.ximpleware;
using System.IO;
//This example shows how to load a VTD+XML index
// into memory then run an XPath against it
// vg.loadIndex returns an instance of VTDNav
namespace IndexLoad
{
    class IndexLoad
    {
        static void Main(string[] args)
        {
            try
            {
                VTDGen vg = new VTDGen();
                VTDNav vn = vg.loadIndex("po.vxl");
                AutoPilot ap = new AutoPilot(vn);
                ap.selectXPath("//items");
                int i;
                while ((i = ap.evalXPath()) != -1)
                {
                }
                ap.resetXPath();
            }
            catch (Exception e)
            {
            }
        }
    }
}

