package DOMTest;
import com.ximpleware.*;

public class XSL1Test {
	public class XSL_stylesheet_node extends XSLNode {

		public XSL_stylesheet_node(int type) {			
			// TODO Auto-generated constructor stub
			nodeType = STYLE_SHEET;
		}
		protected VTDNav sourceVN;
	
	}
	public class XSL_template_node extends XSLNode {

		public XSL_template_node() {
			nodeType = TEMPLATE;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_value_of_node extends XSLNode {

		public XSL_value_of_node() {
			nodeType = VALUE_OF;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_for_each_node extends XSLNode {

		public XSL_for_each_node() {
			nodeType = FOR_EACH;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_output_node extends XSLNode {

		public XSL_output_node() {
			nodeType = OUTPUT;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_sort_node extends XSLNode {

		public XSL_sort_node() {
			nodeType = SORT;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_if_node extends XSLNode {

		public XSL_if_node() {
			nodeType = IF;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_choose_node extends XSLNode {

		public XSL_choose_node() {
			nodeType = CHOOSE;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_when_node extends XSLNode {

		public XSL_when_node() {
			nodeType = WHEN;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_otherwise_node extends XSLNode {

		public XSL_otherwise_node() {
			nodeType = OTHERWISE;
			// TODO Auto-generated constructor stub
		}
		
	}
	public class XSL_apply_templates_node extends XSLNode {

		public XSL_apply_templates_node() {
			nodeType = APPLY_TEMPLATES;
			// TODO Auto-generated constructor stub
		}
		
	}
	public class XSL_attribute_node extends XSLNode {

		public XSL_attribute_node() {
			nodeType = ATTRIBUTE;
			// TODO Auto-generated constructor stub
		}
		
	}
	public class XSL_attribute_set_node extends XSLNode {

		public XSL_attribute_set_node() {
			nodeType = ATTRIBUTE_SET;
			// TODO Auto-generated constructor stub
		}
		
	}
	public class XSL_call_template_node extends XSLNode {

		public XSL_call_template_node() {
			nodeType = CALL_TEMPLATE;
			// TODO Auto-generated constructor stub
		}
		
	}
	public class XSL_comment_node extends XSLNode {

		public XSL_comment_node() {
			nodeType = COMMENT;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_copy_node extends XSLNode {

		public XSL_copy_node() {
			nodeType = COPY;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_copy_of_node extends XSLNode {

		public XSL_copy_of_node() {
			nodeType = COPY_OF;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_decimal_format_node extends XSLNode {

		public XSL_decimal_format_node() {
			nodeType = DECIMAL_FORMAT;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_element_node extends XSLNode {

		public XSL_element_node() {
			nodeType = ELEMENT;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_fall_back_node extends XSLNode {

		public XSL_fall_back_node() {
			nodeType = FALL_BACK;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_import_node extends XSLNode {

		public XSL_import_node() {
			nodeType = IMPORT;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_include_node extends XSLNode {

		public XSL_include_node() {
			nodeType = COMMENT;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	
	public class XSL_key_node extends XSLNode {

		public XSL_key_node() {
			nodeType = KEY;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	
	public class XSL_message_node extends XSLNode {

		public XSL_message_node() {
			nodeType = MESSAGE;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_namespace_alias_node extends XSLNode {

		public XSL_namespace_alias_node() {
			nodeType = NAMESPACE_ALIAS ;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSL_number_node extends XSLNode {

		public XSL_number_node() {
			nodeType = NUMBER;
			// TODO Auto-generated constructor stub
		}
		
	}
	
	public class XSLNode{
		public static final int STYLESHEET =0;
		public static final int TEMPLATE = 1;
		public static final int VALUE_OF = 2;
		public static final int FOR_EACH = 3;
		public static final int CHOOSE = 4;
		public static final int WHEN = 5;
		public static final int OTHERWISE = 6;
		public static final int APPLY_TEMPLATES = 7;
		public static final int APPLY_IMPORTS = 8;
		public static final int ATTRIBUTE = 9;
		public static final int ATTRIBUTE_SET = 10;
		public static final int CALL_TEMPLATE = 11;
		public static final int COMMENT = 12;
		public static final int COPY = 13;
		public static final int COPY_OF = 14;
		public static final int DECIMAL_FORMAT = 15;
		public static final int ELEMENT = 16;
		public static final int FALL_BACK = 17;
		public static final int IF = 18;
		public static final int IMPORT = 19;
		public static final int INCLUDE = 20;
		public static final int KEY = 21;
		public static final int MESSAGE = 22;
		public static final int NAMESPACE_ALIAS = 23;
		public static final int NUMBER = 24;
		public static final int OUTPUT = 25;
		public static final int PARAM = 26;
		public static final int PRESERVE_SPACE = 27;
		public static final int PROCESSING_INSTRUCTION = 28;
		public static final int SORT = 29;
		public static final int STRIP_SPACE = 30;
		public static final int STYLE_SHEET = 31;
		public static final int TEXT = 32;
		public static final int TRANSFORM = 33;
		public static final int VARIABLE = 34;
		public static final int WITH_PARAM = 35;
		
		protected int nodeType;
		protected XSLNode first_child;
		protected XSLNode last_child;
		protected XSLNode parent;
		protected XSLNode prev_sibling;
		protected XSLNode next_sibling;
		
		/*public XSLNode(int type){
			nodeType = type;
		}*/
		
		public int getNodeType (){
			return nodeType;
		} 
		protected VTDNav sourceVN;
		//public void add_first_child(){
			
		//}
		
		
	}
	
	public static void main(String s[]) throws Exception{
		//load xsl sheet with vtd-xml
		VTDGen vg = new VTDGen();
		vg.selectLcDepth(5);
		if(!vg.parseFile("c:/xml/xsl1.xsl", true))
			return;
		System.out.println("good");
		VTDNav vn = vg.getNav();
		
		// build xsl tree
		//1. verify the root node to be style sheet
		
		// top level xsl node names
		
		//template, import, 
		
		
		//load the input Xml document
		// apply the tree to input document
	}

}
