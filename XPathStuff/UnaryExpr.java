import com.ximpleware.*;

public class UnaryExpr extends Expr {

	public int op;
	public Expr operand;
	public UnaryExpr (int o, Expr e){
		op = o;
		operand = e;
	}
	
	public String toString(){
		return "-"+operand;
	}

		
	public boolean evalBoolean(VTDNav vn){
		
		return operand.evalBoolean(vn);
	}

	public double evalNumber(VTDNav vn){ return -1*operand.evalNumber(vn);}
		
	public int evalNodeSet(VTDNav vn) throws XPathEvalException{
		
		throw new XPathEvalException("UnaryExpr can't eval to a node set!");
	}
	
        public String evalString(VTDNav vn){
		double dval = operand.evalNumber(vn);
		if (dval == (int) dval){
			return ""+((int) dval);
		}
		return ""+dval;
	}

	public void reset(){
		operand.reset();
	}

	public boolean  isNodeSet(){
		return false;
	}

	public boolean  isNumerical(){
		return true;
	}
}
