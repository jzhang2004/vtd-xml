package DOMTest;

import junit.framework.TestCase;

import com.ximpleware.extended.VTDGenHuge;
import com.ximpleware.extended.VTDNavHuge;

public class ParserTest extends TestCase {
	private VTDGenHuge vgh;
	private VTDNavHuge vnh;
	long start;
	long end;
	long count;
	
	protected void setUp() 
	{	
		vgh=new VTDGenHuge();
		
		start=System.currentTimeMillis();
		vgh.parseFile("c:/xml/pubmed_result.txt", false);
		vnh=vgh.getNav();
	}
 
	protected void tearDown()
	{
	  end=System.currentTimeMillis();
	  vgh=null;
	  vnh=null;
	  System.out.println("Number of articles found: "+count+" in "+(end-start)+" milliseconds");
	}
	private void printResults(String pmid) 
	{
		//System.out.println("PMID is: "+pmid);
	}
	
	public void testParser() throws Exception
	{
		
		if(vnh.matchElement("PubmedArticleSet"))
		{
			
			if(vnh.toElement(VTDNavHuge.FC,"PubmedArticle")){
			 do{
				if(vnh.toElement(VTDNavHuge.FC,"MedlineCitation"))
					{
			
			
						do {
				
							if(vnh.toElement(VTDNavHuge.FC,"PMID"))
							{
								do {
									int rp=vnh.getText();
									if(rp!=-1)
									{
										printResults(vnh.toString(rp));
										count++;
									}
									 
							
								   } while(vnh.toElement(VTDNavHuge.NS,"PMID")); 
								    vnh.toElement(VTDNavHuge.P);
							}
				
				  
	        	
						} while(vnh.toElement(VTDNavHuge.NS,"MedlineCitation")); 
	    	             vnh.toElement(VTDNavHuge.P); 
	    	
					}
			 }while(vnh.toElement(VTDNavHuge.NS,"PubmedArticle"));
		  	  vnh.toElement(VTDNavHuge.P); 
		  }// end of PubmedArticle if block
	   }// end of PubmedArticleSet if block

   }// end of testParser()

   public static void main(String argv[]) throws Exception {
	ParserTest pt = new ParserTest();
	pt.setUp();
	pt.testParser();
	pt.tearDown();
	//pt.printResults();
   }
	
}