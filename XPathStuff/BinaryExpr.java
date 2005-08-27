import com.ximpleware.*;

class BinaryExpr extends Expr {
	public final static int ADD = 0;
	public final static int SUB = 1;
	public final static int MULT = 2;
	public final static int DIV = 3;
	public final static int MOD = 4;
	public final static int OR = 5;
	public final static int AND = 6;
	public final static int EQ = 7;
	public final static int NE = 8;
	public final static int LE = 9;
	public final static int GE = 10;
	public final static int LT = 11;
	public final static int GT = 12;
	public final static int UNION = 13;

	public int op;
	boolean isNumerical;
	
	public Expr left;
	public Expr right;
	public BinaryExpr ( Expr l, int o, Expr r) {
		op = o;
		left = l;
		right = r;
		switch(op){
		 	case ADD:
			case SUB:
			case MULT:
			case DIV:
			case MOD: isNumerical = true; break;
			default: isNumerical = false;
		}
	}
	public String toString(){
		String os;
		switch(op){
			case ADD: os = " + "; break;
			case SUB: os = " - "; break;
			case MULT: os = " * "; break;
			case DIV: os = " / "; break;
			case MOD: os = " mod "; break;
			case OR : os = " or ";break;
			case AND: os = " and "; break;
			case EQ: os = " = "; break;
			case NE: os = " != "; break;
			case LE: os = " <= "; break;
			case GE: os = " >= "; break;
			case LT: os = " < "; break;
			case GT: os = " > "; break;
			default: os = " | "; 
		}
		
		return "("+ left + os + right+")";
	}

	
	public boolean evalBoolean(VTDNav vn){
		switch(op){
			case OR: return left.evalBoolean(vn) || right.evalBoolean(vn);
			case AND:return left.evalBoolean(vn) || right.evalBoolean(vn);
			case EQ: return left.evalNumber(vn) == right.evalNumber(vn);
			case NE: return left.evalNumber(vn) != right.evalNumber(vn);
			case LE: return left.evalNumber(vn) <= right.evalNumber(vn);
			case GE: return left.evalNumber(vn) >= right.evalNumber(vn);
			case LT: return left.evalNumber(vn) < right.evalNumber(vn);
			case GT: return left.evalNumber(vn) > right.evalNumber(vn);
			default: double dval = evalNumber(vn);
				 if (dval ==-0.0 || dval ==+0.0 || Double.isNaN(dval))
					 return true;
				 return false;
		}
	}

	public double evalNumber(VTDNav vn){
		switch(op){
			case ADD: return left.evalNumber(vn) + right.evalNumber(vn);
			case SUB: return left.evalNumber(vn) - right.evalNumber(vn);
			case MULT:return left.evalNumber(vn) * right.evalNumber(vn);
			case DIV: return left.evalNumber(vn) / right.evalNumber(vn);
			case MOD: return left.evalNumber(vn) % right.evalNumber(vn);
			default	: if (evalBoolean(vn) == true)
					  return 1;
				  return 0;

		}
	}
		
	public int evalNodeSet(VTDNav vn) throws XPathEvalException {
		throw new XPathEvalException("LiteralExpr can't eval to a node set!");
	}
	
        public String evalString(VTDNav vn){
		return "this";
	}

	public void reset(){left.reset(); right.reset();};

	public boolean  isNodeSet(){
		return false;
	}

	public boolean  isNumerical(){
		return isNumerical;
	}
}
