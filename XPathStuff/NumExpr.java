import com.ximpleware.*;
class NumExpr extends Expr{
	public double dval;
	public NumExpr( double d){
		dval = d;
	}
	public String toString(){
		if (dval == (int) dval){
			return ""+(int) dval;
		}
		return  ""+dval;
	} 

	public double eval(){
		return dval;
	}
	public boolean  isNodeSet(){
		return false;
	}

	public boolean  isNumerical(){
		return true;
	}
		
	public boolean evalBoolean(VTDNav vn){
		if (dval == 0.0 || Double.isNaN(dval) )
			return false;
		return true;
	}

	public double evalNumber(VTDNav vn){ return dval;}
		
	public int evalNodeSet(VTDNav vn) throws XPathEvalException{
		
		throw new XPathEvalException("NumExpr can't eval to a node set!");
	}
	
        public String evalString(VTDNav vn){
		if (dval == (int) dval){
			return ""+((int) dval);
		}
		return ""+dval;
	}

	public void reset(){};
}
