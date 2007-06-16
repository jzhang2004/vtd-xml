/* 
 * Copyright (C) 2002-2006 XimpleWare, info@ximpleware.com
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
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.JApplet;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;



import com.ximpleware.NavException;
import com.ximpleware.ParseException;
import com.ximpleware.PilotException;
import com.ximpleware.AutoPilot;
import com.ximpleware.VTDGen;
import com.ximpleware.VTDNav;
import com.ximpleware.xpath.*;



public class XPathDemo extends JApplet{
	JButton but_parse_no_ns, but_parse_ns, but_clear;
	JButton nav_root, nav_parent, nav_first_child, nav_last_child;
	JButton nav_next_sib, nav_prev_sib, nav_push, nav_pop;
	//JButton but_get_attr; // get Attr val
	//JButton but_get_text; // get Text
	JButton but_frag; // getFragmenet
	JButton but_sel_xpath;
	JButton but_iterate;
	
	JButton but_sel_XPath;
	JButton but_evalXPath;
	JButton but_reset;
	//JButton but_touch;
	//JButton but_text_next;
	boolean test_text;
	JTextArea textArea;
	
	VTDGen vg;
	VTDNav vn;
	//TextIter ti;
	
	JPanel buttonArea;
	JPanel navPanel;
	JPanel parsePanel;
	JPanel dataPanel;
	JPanel inputPanel;
	JPanel statusPanel;
	JPanel XPathPanel;
	
	//JLabel attrLabel;
	JTextField XPathExpr;
	//JTextField attrName;
	//JLabel nsLabel;
	//JTextField nsVal;
	
	JLabel status;
	
	AutoPilot ap;
	
	public void init(){
		//ti = new TextIter();
		ap = null;
		vn = null;
		test_text = true;
		// create buttons and assign actionListener
		//but_parse_no_ns = new JButton("parse (ns disabled)");
		//but_parse_no_ns.addActionListener(new parse_no_ns_Listener());
		but_parse_ns = new JButton("parse");
		but_parse_ns.addActionListener(new parse_ns_Listener());
		but_clear = new JButton("clear");
		but_clear.addActionListener(new clearListener());
		
		/*nav_root = new JButton("toElement(ROOT)");
		nav_root.addActionListener(new navRootListener());
		nav_parent= new JButton("toElement(PARENT)");
		nav_parent.addActionListener(new navParentListener());
		nav_first_child = new JButton("toElement(FIRST_CHILD)");
		nav_first_child.addActionListener(new navFCListener());
		nav_last_child = new JButton("toElement(LAST_CHILD)");
		nav_last_child.addActionListener(new navLCListener());
		nav_next_sib = new JButton("toElement(NEXT_SIB)");
		nav_next_sib.addActionListener(new navNSListener());
		nav_prev_sib = new JButton("toElement(PREV_SIB)");
		nav_prev_sib.addActionListener(new navPSListener());
		nav_push = new JButton("push");
		nav_push.addActionListener(new navPushListener());
		nav_pop = new JButton("pop");
		nav_pop.addActionListener(new navPopListener());*/
		
		//but_get_attr = new JButton("get Attr val");
		//but_get_attr.addActionListener(new getAttrListener());

		//but_get_text = new JButton("get text/CDATA");
		//but_get_text.addActionListener(new getTextListener());
		
		but_sel_XPath = new JButton("select XPath");
		but_sel_XPath.addActionListener(new selXPathListener());
		
		but_evalXPath = new JButton("iterate");
		but_evalXPath.addActionListener(new iterXPathListener());
		
		but_reset = new JButton("reset expression");
		but_reset.addActionListener(new resetXPathListener());		
		
		but_frag = new JButton("get element fragment");
		but_frag.addActionListener(new getFragmentListener());
		
//		but_touch = new JButton("touch");
//		but_touch.addActionListener(new touchListener());
//		
//		but_text_next = new JButton("getNext");
//		but_text_next.addActionListener(new textGetNext());
		
		XPathExpr = new JTextField("(//b[c])[last()]");
		XPathExpr.setBorder(BorderFactory.createEtchedBorder());
		//XPathExpr.setEnabled(false);
		

		
		status = new JLabel();
		
		// instantiate panels
		buttonArea = new JPanel(new GridLayout(4,1));
		buttonArea.resize(540,300);
		
		parsePanel = new JPanel(new GridLayout(1,2));
		parsePanel.setBorder(BorderFactory.createTitledBorder("Parse and Clear"));
		
		//navPanel = new JPanel(new GridLayout(2,4));
		//navPanel.setBorder(BorderFactory.createTitledBorder("Navigation"));
		
				
		XPathPanel = new JPanel(new GridLayout(1,2));
		XPathPanel.setBorder(BorderFactory.createTitledBorder("XPath Control Buttons"));
		
		inputPanel = new JPanel(new GridLayout(1,3));
		inputPanel.setBorder(BorderFactory.createTitledBorder("XPath Expression"));
		
		statusPanel = new JPanel();
		statusPanel.setBorder(BorderFactory.createTitledBorder("status"));
		//inputPanel.setForeground(Color.BLUE);
				
		
		// add buttons
		//parsePanel.add(but_parse_no_ns);
		parsePanel.add(but_parse_ns);
		parsePanel.add(but_clear);
		
		/*navPanel.add(nav_root);
		navPanel.add(nav_parent);
		navPanel.add(nav_first_child);
		navPanel.add(nav_last_child);
		navPanel.add(nav_next_sib);
		navPanel.add(nav_prev_sib);
		navPanel.add(nav_push);
		navPanel.add(nav_pop);*/
//		if(!test_text){
//			dataPanel.add(but_get_text);
//			dataPanel.add(but_get_attr);
//			dataPanel.add(but_frag);
//		}
//		else{
//			textIterPanel.add(but_touch);
//			textIterPanel.add(but_text_next);
//		}
		inputPanel.add(XPathExpr);
		
		XPathPanel.add(but_sel_XPath);
		XPathPanel.add(but_evalXPath);
		XPathPanel.add(but_reset);
			
		statusPanel.add(status);
	
		
		
		// enable and disable various buttons
		//but_parse_no_ns.setEnabled(true);
		but_parse_ns.setEnabled(true);
		but_clear.setEnabled(true);
		
		setButtons(false);
		
		
		
		getContentPane().setLayout(new FlowLayout());
		textArea = new JTextArea(12,25);
		JScrollPane scrollPane = 
		    new JScrollPane(textArea,
		                    JScrollPane.VERTICAL_SCROLLBAR_ALWAYS,
		                    JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
		textArea.setEditable(true);
		textArea.setFont(new Font("Helvetica",Font.BOLD,18));
		textArea.setLineWrap(true);
		getContentPane().add(scrollPane);
		String s = "<root>\n"+				
					"  <b a='1' b='2'>\n"+
					"    <a>\n"+
					"      <b a='1' b='2'/>\n"+
					"      <b  a='1' b='2'> <c/> </b>\n"+
					"    </a>\n"+
					"    <d/>\n"+
					"    <c/>\n"+
					"    <c/>\n"+
					"    <a1> <a2/> </a1>\n"+
					" </b>\n"+
					" <c/>\n"+
					"</root>";
		textArea.setText(s);
		textArea.setSelectedTextColor(Color.RED);
		textArea.setSelectionColor(Color.DARK_GRAY);
		
		textArea.setSelectionStart(2);
		textArea.setSelectionEnd(6);
		
		buttonArea.add(parsePanel);
		//buttonArea.add(navPanel);
		
		buttonArea.add(XPathPanel);
		buttonArea.add(inputPanel);
		buttonArea.add(statusPanel);
		
		
		getContentPane().add(buttonArea);
		
		//buttonPanel.add(new JLabel("good"));
		//textArea.requestFocus();
		//getContentPane().add(but_parse);
		//buttonPanel.resize(200,400);
		//buttonPanel.show();
		vg = new VTDGen();
		
		resize(500,540);
	
		
	}
	
	public void setButtons(boolean b){
		/*nav_root.setEnabled(b);
		nav_parent.setEnabled(b);
		nav_first_child.setEnabled(b);
		nav_last_child.setEnabled(b);
		nav_next_sib.setEnabled(b);
		nav_prev_sib.setEnabled(b);
		nav_push.setEnabled(b);
		nav_pop.setEnabled(b);*/
		
		//but_get_text.setEnabled(b);
		//but_get_attr.setEnabled(b);
		but_frag.setEnabled(b);
		but_sel_XPath.setEnabled(b);
		but_evalXPath.setEnabled(b);
		but_reset.setEnabled(b);
		
		//but_touch.setEnabled(b);
		//but_text_next.setEnabled(b);
		
		//XPathExpr.setEnabled(b);
		//attrName.setEnabled(b);
	}
	public static void main(String[] args) {
	 JApplet ja = new JApplet();
	}
	
	
	public void showToken(){
		if (vn==null)return;
		textArea.setSelectedTextColor(Color.RED);
		textArea.setSelectionColor(Color.DARK_GRAY);
		int index = vn.getCurrentIndex();
		int os = vn.getTokenOffset(index);
		int len = vn.getTokenLength(index);
		int type = vn.getTokenType(index);
		if (type == VTDNav.TOKEN_STARTING_TAG 
				|| type == VTDNav.TOKEN_ATTR_NAME
				|| type == VTDNav.TOKEN_ATTR_NS){
		    len = len & 0xff;
		}
		System.out.println("os --> "+os);
		System.out.println("len --> "+len);
		textArea.setSelectionStart(os);
		textArea.setSelectionEnd(os + len );
		textArea.requestFocus();
		//repaint();
	}
	
	public void showToken(int index){
		if (vn==null)return;
		textArea.setSelectedTextColor(Color.RED);
		textArea.setSelectionColor(Color.DARK_GRAY);
		//int index = vn.getCurrentIndex();
		int os = vn.getTokenOffset(index);
		int len = vn.getTokenLength(index);
		int type = vn.getTokenType(index);
		if (type == VTDNav.TOKEN_STARTING_TAG 
				|| type == VTDNav.TOKEN_ATTR_NAME
				|| type == VTDNav.TOKEN_ATTR_NS){
		    len = len & 0xff;
		}
		System.out.println("os --> "+os);
		System.out.println("len --> "+len);
		textArea.setSelectionStart(os);
		textArea.setSelectionEnd(os + len );
		textArea.requestFocus();
	}
	
	public void showToken(int os, int len){
		textArea.setSelectionStart(os);
		textArea.setSelectionEnd(os + len );
		textArea.requestFocus();
	}
	
	public class parse_ns_Listener implements ActionListener {
		public void actionPerformed(ActionEvent e){
			try{
				status.setText("");
				vg.setDoc(textArea.getText().getBytes());
				vg.parse(true);
				vn = vg.getNav();
				
				setButtons(true);
				//nsVal.setEnabled(true);
/*				for (int i=0;i<vn.getTokenCount();i++)
				{
					int type = vn.getTokenType(i);
					if (type == VTDNav.TOKEN_STARTING_TAG
							|| type == VTDNav.TOKEN_ATTR_NAME
							|| type == VTDNav.TOKEN_ATTR_NS)
					{					
						System.out.println("Prefix: "+((vn.getTokenLength(i)>>16)&0xffff));
						System.out.println("Q length "+(vn.getTokenLength(i)&0xffff));
					}
				}*/
				showToken();
				status.setText("current index is "+vn.getCurrentIndex());
				
			}catch (ParseException pe){
				setButtons(false);
				status.setText(pe.toString());
			}
		}
	}
	
	public class clearListener implements ActionListener {
		public void actionPerformed(ActionEvent e){
			status.setText("");
			textArea.setText(" ");
			setButtons(false);
		}
	}

	/*public class navRootListener implements ActionListener {
		public void actionPerformed(ActionEvent e){
			try{
				status.setText("");
				vn.toElement(VTDNav.ROOT);
				showToken();
				status.setText("current index is "+vn.getCurrentIndex());
			}
			catch (NavException ne){
				status.setText(ne.toString());
			}
		}
	}*/
	
//	public class navParentListener implements ActionListener {
//		public void actionPerformed(ActionEvent e){
//			try{
//				status.setText("");
//				vn.toElement(VTDNav.PARENT);
//				/*if (attrName.getText().length() == 0)
//				   vn.toElement(VTDNav.);
//				else if (nsVal.getText().length() == 0)
//				   vn.toElement(VTDNav.);
//				else 
//					vn.toElementNS(VTDNav.);*/
//				   
//				showToken();
//				status.setText("current index is "+vn.getCurrentIndex());
//			}
//			catch (NavException ne){
//				status.setText(ne.toString());
//			}
//		}
//	}
	
//	public class navFCListener implements ActionListener {
//		public void actionPerformed(ActionEvent e){
//			try{
//				status.setText("");
//				vn.toElement(VTDNav.FC);
//				
//				showToken();
//				status.setText("current index is "+vn.getCurrentIndex());
//			}
//			catch (NavException ne){
//				status.setText(ne.toString());
//			}
//		}
//	}
//
//	public class navLCListener implements ActionListener {
//		public void actionPerformed(ActionEvent e){
//			try{
//				status.setText("");				
//				vn.toElement(VTDNav.LC);
//			
//				//System.out.println("to last child :"+vn.toElement(VTDNav.LC));
//				showToken();
//				status.setText("current index is "+vn.getCurrentIndex());
//			}
//			catch (NavException ne){
//				status.setText(ne.toString());
//			}
//		}
//	}
//	
//	public class navNSListener implements ActionListener {
//		public void actionPerformed(ActionEvent e){
//			try{
//				status.setText("");
//				
//				vn.toElement(VTDNav.NS);
//				
//				//System.out.println("to next sibling :"+vn.toElement(VTDNav.NS));
//				showToken();
//				status.setText("current index is "+vn.getCurrentIndex());
//			}
//			catch (NavException ne){
//				status.setText(ne.toString());
//			}
//		}
//	}
//	
////	public class navPSListener implements ActionListener {
//		public void actionPerformed(ActionEvent e){
//			try{
//				status.setText("");
//				
//				vn.toElement(VTDNav.PS);
//				
//				//System.out.println("to prev child :"+ vn.toElement(VTDNav.PS));
//				showToken();
//				status.setText("current index is "+vn.getCurrentIndex());
//			}
//			catch (NavException ne){
//				status.setText(ne.toString());
//			}
//		}
//	}
	
	public class getTextListener implements ActionListener {
		public void actionPerformed(ActionEvent e){
			System.out.println("get text");
			status.setText("");
			if (vn!=null){
				int i = vn.getText();
				if (i!=-1)
					showToken(i);
				else{
					showToken();
					status.setText("no text node found");
				}
			}
					
		}
	}
	

	
	public class getFragmentListener implements ActionListener {
		public void actionPerformed (ActionEvent e){
			try{
			  long l = vn.getElementFragment();
			  showToken((int)l, (int) ((l>>32)&0xffffffff));			  
			}
			catch (NavException ne){
				status.setText(ne.toString());
			}
		}
	}
	
//	public class navPushListener implements ActionListener{
//		public void actionPerformed (ActionEvent e){
//			if (vn!=null){
//				vn.push();
//				showToken();
//			}
//		}
//	}
//  public class navPopListener implements ActionListener{
//		public void actionPerformed (ActionEvent e){		
//			if (vn!=null){
//				vn.pop();
//				showToken();
//				status.setText("current index is "+vn.getCurrentIndex());
//			}
//		}
//	}
	public class selXPathListener implements ActionListener{
		public void actionPerformed (ActionEvent e){
			try{
				if (vn!=null){
					if (ap == null)
						ap = new AutoPilot(vn);
					else
						ap.bind(vn);
					ap.declareXPathNameSpace("ns1","GOOD");
					ap.declareXPathNameSpace("ns2","BAD");
					ap.selectXPath(XPathExpr.getText());
					but_evalXPath.setEnabled(true);
					but_reset.setEnabled(true);
					status.setText(ap.getExprString());
				}
			}
			catch (XPathParseException pe){
				but_evalXPath.setEnabled(false);
				but_reset.setEnabled(false);
				status.setText(pe.toString());
			}
		}		
	}
	
	public class iterXPathListener implements ActionListener{
		public void actionPerformed (ActionEvent e){
			try{
				int i = ap.evalXPath();
				if (i!=-1){
					showToken(i);	
					status.setText("current index is "+i);
				}
			}catch (XPathEvalException xe){
				status.setText(xe.toString());
			}catch (NavException ne){
				status.setText(ne.toString());
			}
		}
	}
	
	public class resetXPathListener implements ActionListener {
		public void actionPerformed (ActionEvent e){
			ap.resetXPath();
		}
	}
	
}
