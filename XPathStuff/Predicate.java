import com.ximpleware.*;

class Predicate implements LocationPathNode{
	double d; // only supports a[1] style of location path for now
	public int count;
	public Expr expr;
	public Predicate(){
		nextP = (Predicate) null;
		count = 1;
		d = 0;
	}
	public boolean eval(VTDNav vn) {
		if (expr.isNumerical())
		   return ( expr.evalNumber(vn)== (double) count++);
		return expr.evalBoolean(vn);
	}
	
	public void setIndex(double index) throws XPathEvalException{
		if (index<=0)
			throw new XPathEvalException("Invalid index number");
		d = (double) index;
	}
	
	public void reset(){
		count = 1;
		expr.reset();
	}
	public Predicate nextP;

	public String toString(){
		String s = "["+expr+"]";
		if (nextP==null){
			return s;
		} else {
			return s+nextP;
		}
	}
}

