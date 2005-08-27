import com.ximpleware.*;
class LiteralExpr extends Expr {
	public String s;
	public LiteralExpr (String st){
		s = st;
	}	
	public String toString(){
		boolean b = true;
		for(int i = 0;i<s.length();i++){
			if (s.charAt(i) == '\''){
				b = false;
				break;
			}
		}
		if (b == true)
		  return "\""+s+"\"";
		else 
		  return "'" + s + "'";
	}

	public boolean evalBoolean(VTDNav vn){
		return s.length() == 0;
	}

	public double evalNumber(VTDNav vn){
		try {
			double dval = Double.parseDouble(s);
			return dval;
		}catch (NumberFormatException e){
			return Double.NaN;
		}	
	}
		
	public int evalNodeSet(VTDNav vn) throws XPathEvalException{
		
		throw new XPathEvalException("LiteralExpr can't eval to a node set!");
	}
	
        public String evalString(VTDNav vn){
		return s;
	}

	public void reset(){ }

		public boolean  isNodeSet(){
		return false;
	}

	public boolean  isNumerical(){
		return false;
	}
}
