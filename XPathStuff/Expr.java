import com.ximpleware.*;

abstract public class Expr {

	abstract public boolean evalBoolean(VTDNav vn);

	abstract public double evalNumber(VTDNav vn);
		
	abstract public int evalNodeSet(VTDNav vn) throws XPathEvalException, NavException;
	
	abstract public String evalString(VTDNav vn);

	abstract public void reset();
	abstract public String toString();

	abstract public boolean isNumerical();
	abstract public boolean isNodeSet();
}
