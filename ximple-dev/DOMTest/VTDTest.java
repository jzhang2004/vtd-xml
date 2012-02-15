package DOMTest;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.io.RandomAccessFile;

import com.ximpleware.FastLongBuffer;

import com.ximpleware.extended.AutoPilotHuge;
import com.ximpleware.extended.VTDExceptionHuge;
import com.ximpleware.extended.VTDGenHuge;
import com.ximpleware.extended.VTDNavHuge;

public class VTDTest {

    public static void main(String[] args) {
        checkMethod(10000000l);
        checkMethod(80000000l);
    }

    private static void checkMethod(long testtags) {
        System.out.println ("Checking with "+testtags);
        try {
            File demofile = new File("demo"+testtags+".xml");
            BufferedWriter writer = new BufferedWriter(new
FileWriter(demofile));
            writer.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
            writer.write("<root>\n");
            for (long i = 0; i < testtags; i++) {
                writer.write("<test>abcdefg</test>");
            }
            writer.write("<next/>");
            writer.write("</root>\n");
            writer.close();

        } catch (IOException e1) {
            e1.printStackTrace();
        }
        System.out.println("file generated");

        try {
            VTDGenHuge vg = new VTDGenHuge();
            if (vg.parseFile("demo"+testtags+".xml", true,
VTDGenHuge.MEM_MAPPED)) {
                System.out.println ("Parsing file");
                VTDNavHuge vn = vg.getNav();
                AutoPilotHuge ap = new AutoPilotHuge(vn);
                ap.selectXPath("/root/next");
                FastLongBuffer flb = new FastLongBuffer(4);
                while (ap.evalXPath() != -1) {
                    flb.append(vn.getElementFragment());
                }
                System.out.println("HUGE : Size is " + flb.size() + ".");

                int size = flb.size() / 2;

                RandomAccessFile raf = new RandomAccessFile(
                        new File("demo"+testtags+".xml"), "r");
                for (int k = 0; k < size * 2; k++) {
                    byte[] fb = new byte[(int) flb.longAt(k + 1)];
                    raf.seek(flb.longAt(k));
                    raf.readFully(fb);
                    String string = new String(fb);
                    System.out.println(string);
                }

            }
        } catch (VTDExceptionHuge e) {
            e.printStackTrace();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
