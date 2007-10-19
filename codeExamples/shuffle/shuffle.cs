using System;
using System.Collections.Generic;
using System.Text;
using com.ximpleware;
using System.IO;
namespace shuffle
{
    public class shuffle
    {
        public static void Main(String[] args)
        {
            VTDGen vg = new VTDGen();
            AutoPilot ap0 = new AutoPilot();
            AutoPilot ap1 = new AutoPilot();
            AutoPilot ap2 = new AutoPilot();
            ap0.selectXPath("/root/a");
            ap1.selectXPath("/root/b");
            ap2.selectXPath("/root/c");
            Encoding eg = System.Text.Encoding.GetEncoding("utf-8");
            if (vg.parseFile("old.xml", false))
            {
                VTDNav vn = vg.getNav();
                ap0.bind(vn);
                ap1.bind(vn);
                ap2.bind(vn);
                FileStream fos = new FileStream("new.xml", System.IO.FileMode.OpenOrCreate);
                //fos.Write("<root>".getBytes());
                byte[] ba0,ba1, ba2, ba3, ba4;
                //ba0 = eg.GetBytes("
                ba1 = eg.GetBytes("<root>");
                ba2 = eg.GetBytes("</root>");
                ba3 = eg.GetBytes("\n");
                fos.Write(ba1, 0, ba1.Length);
                byte[] ba = vn.getXML().getBytes();
                while (ap0.evalXPath() != -1)
                {
                    long l = vn.getElementFragment();
                    int offset = (int)l;
                    int len = (int)(l >> 32);
                    fos.Write(ba3,0,ba3.Length);
                    fos.Write(ba, offset, len);
                }
                ap0.resetXPath();
                while (ap1.evalXPath() != -1)
                {
                    long l = vn.getElementFragment();
                    int offset = (int)l;
                    int len = (int)(l >> 32);
                    fos.Write(ba3,0,ba3.Length);
                    fos.Write(ba, offset, len);
                }
                ap1.resetXPath();
                while (ap2.evalXPath() != -1)
                {
                    long l = vn.getElementFragment();
                    int offset = (int)l;
                    int len = (int)(l >> 32);
                    fos.Write(ba3,0,ba3.Length);
                    fos.Write(ba, offset, len);
                }
                ap2.resetXPath();
                fos.Write(ba3,0,ba3.Length);
                fos.Write(ba2,0,ba2.Length);
            }
        }
    }
}

