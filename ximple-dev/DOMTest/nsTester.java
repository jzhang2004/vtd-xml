package DOMTest;
import com.ximpleware.*;


public class nsTester {
	// 12 cases
	public static void main(String[] sv){
		VTDGen vg = new VTDGen();
		int i=1;
		String s;
		VTDNav vn;
		
		String UTF16LE = "<?xml version='1.0' encoding='utf-16le'?>";
		String UTF16BE = "<?xml version='1.0' encoding='utf-16be'?>";
		try {
		    s = "<a xmlns=''> <a xmlns=' '/></a>";
		    System.out.println(s);
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" ok");
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" ok");
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" ok");
		}catch(ParseException e){
			System.out.println("test"+i+" failed"+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<a xmlns='http://www.w3.org/XML/1998/namespace'/>";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<a xmlns='http://www.w3.org/2000/xmlns/'/>";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		
		s = "<a xmlns='http://www.w3.org/2000/xmlns/ '/>";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" ok");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" ok");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" ok");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<a xmlns:abc=''/>";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<abc:a xmlns:abc=' ' />";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" ok");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" ok ");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" ok");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<xml:a xmlns:abc=' ' />";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" ok");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" ok ");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" ok");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<xmlns:a abc=' ' />";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" failed ");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<a xml:abc=' ' />";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" ok");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" ok ");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" ok ");
		}catch(ParseException e){
			System.out.println("test"+i+" failed "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<a xml:abc=' ' />";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" ok");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" ok ");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" ok ");
		}catch(ParseException e){
			System.out.println("test"+i+" failed "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		
		s = "<abc:a xml:abcd=' '>  </abc:a>";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed  "+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" failed ");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" failed ");
		}catch(ParseException e){
			System.out.println("test"+i+" ok "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<a xml:abc=' ' />";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" ok");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" ok ");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" ok ");
		}catch(ParseException e){
			System.out.println("test"+i+" failed "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		
		s = "<abc:a xml:abcd=' '>  </abc:a>";
		i++;
		//s = "<a xmlns=''/>";
		System.out.println(s);
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed  "+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" failed ");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" failed ");
		}catch(ParseException e){
			System.out.println("test"+i+" ok "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<abc:a xmlns:abc=' '> <a> <abc:abc/></a> </abc:a>";
		i++;
		//s = "<a xmlns=''/>";
		System.out.println(s);
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" ok");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed  "+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" ok ");
		}catch(ParseException e){
			System.out.println("test"+i+" failed  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" ok ");
		}catch(ParseException e){
			System.out.println("test"+i+" failed "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		
		s = "<abc:a xmlns:abc='--' xmlns:cde='--' abc:a='' cde:a=''> <a> <abc:abc/></a> </abc:a>";
		i++;
		//s = "<a xmlns=''/>";
		System.out.println(s);
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed  "+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" failed ");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" failed ");
		}catch(ParseException e){
			System.out.println("test"+i+" ok "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<abc:a xmlns:abc='--' xmlns:cde='--' cde:a=''> <a> <abc:abc abc:a='' /></a> </abc:a>";
		i++;
		//s = "<a xmlns=''/>";
		System.out.println(s);
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" ok ");
		}catch(ParseException e){
			System.out.println("test"+i+" failed "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed "+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" ok ");
		}catch(ParseException e){
			System.out.println("test"+i+" failed "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed "+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" ok ");
		}catch(ParseException e){
			System.out.println("test"+i+" failed "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed "+e);
		}
		
		s = "<abc:a xmlns:abc='&#45;-' xmlns:cde='-&#45;' cde:a=''>"
		        +"<a> <abc:abc xmlns:kge=' ' abc:a=''/>"
		        +"<b cde:a=''> </b></a> <cde:ggg/>"
		        +"</abc:a>";
		i++;
		//s = "<a xmlns=''/>";
		System.out.println(s);
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			vn = vg.getNav();
			vn.toElement(VTDNav.P);
			if (vn.toElementNS(VTDNav.FC, "--", "a")
					&& vn.hasAttrNS("--", "a"))					
				System.out.println("test"+i+" ok ");
			else 
				System.out.println("test"+i+" failed");
			
		}catch(ParseException e){
			System.out.println("test"+i+" failed "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed "+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			vn = vg.getNav();
			vn.toElement(VTDNav.P);
			if (vn.toElementNS(VTDNav.FC, "--", "a")
					&& vn.hasAttrNS("--", "a"))					
				System.out.println("test"+i+" ok ");
			else 
				System.out.println("test"+i+" failed");
			//System.out.println("test"+i+" ok ");
			
		}catch(ParseException e){
			System.out.println("test"+i+" failed "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed "+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			vn = vg.getNav();
			vn.toElement(VTDNav.P);
			if (vn.toElementNS(VTDNav.FC, "--", "a")
					&& vn.hasAttrNS("--", "a"))				
				System.out.println("test"+i+" ok ");
			else 
				System.out.println("test"+i+" failed");
			//System.out.println("test"+i+" ok ");
			
		}catch(ParseException e){
			System.out.println("test"+i+" failed "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed "+e);
		}
		
		s = "<a xmlns:xmlns=' '/>";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<a><d><c><b  xmlns:ns1=' '> </b></c></d><a ns1:a=''/></a>";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		
		s = "<a><d><c><b xmlns:ns1=' '> <ns1:d/> </b></c></d><ns1:a ns1:a=''/></a>";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<a><d><c><b xmlns:ns1=' '> <ns1:d/> </b></c></d><ns1:a/></a>";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<a><d><c><b > <ns1:d xmlns:ns1=' '/> </b></c></d><ns1:a/></a>";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		s = "<a><d><c><b xmlns:ns1=' '>  </b></c></d><ns1:d xmlns:ns1=' '/><ns1:a/></a>";
		i++;
		System.out.println(s);
		//s = "<a xmlns=''/>";
		try {    
			vg.setDoc(s.getBytes());
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}

		try{
			vg.setDoc((UTF16LE+s).getBytes("UTF-16LE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
		
		try{
			vg.setDoc((UTF16BE+s).getBytes("UTF-16BE"));
			vg.parse(true);
			System.out.println("test"+i+" failed");
		}catch(ParseException e){
			System.out.println("test"+i+" ok  "+e);
		}catch(Exception e){
			System.out.println("test"+i+" failed"+e);
		}
	}	
}
