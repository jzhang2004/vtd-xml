package com.ximpleware.extended.xpath;


import com.ximpleware.extended.NavExceptionHuge;
import com.ximpleware.extended.VTDNavHuge;
import com.ximpleware.extended.XPathEvalExceptionHuge;


public class VariableExpr extends Expr {
	
	private String exprName;
	private Expr exprVal;
	
	public VariableExpr(String name, Expr e){
		exprName = name;
		exprVal = e;
	}
	
	
	public int adjust(int n) {
		// TODO Auto-generated method stub
		return exprVal.adjust(n);
	}

	
	public boolean evalBoolean(VTDNavHuge vn) {
		// TODO Auto-generated method stub
		return exprVal.evalBoolean(vn);
	}

	
	public int evalNodeSet(VTDNavHuge vn) throws XPathEvalExceptionHuge, NavExceptionHuge {
		// TODO Auto-generated method stub
		return exprVal.evalNodeSet(vn);
	}

	
	public double evalNumber(VTDNavHuge vn) {
		// TODO Auto-generated method stub
		return exprVal.evalNumber(vn);
	}

	public String evalString(VTDNavHuge vn) {
		// TODO Auto-generated method stub
		return exprVal.evalString(vn);
	}

	
	public boolean isBoolean() {
		// TODO Auto-generated method stub
		return exprVal.isBoolean();
	}

	
	public boolean isNodeSet() {
		// TODO Auto-generated method stub
		return exprVal.isNodeSet();
	}

	
	public boolean isNumerical() {
		// TODO Auto-generated method stub
		return exprVal.isNumerical();
	}

	
	public boolean isString() {
		// TODO Auto-generated method stub
		return exprVal.isString();
	}

	
	public boolean requireContextSize() {
		// TODO Auto-generated method stub
		return exprVal.requireContextSize();
	}

	
	public void reset(VTDNavHuge vn) {
		// TODO Auto-generated method stub
		exprVal.reset(vn);
	}

	
	public void setContextSize(int size) {
		// TODO Auto-generated method stub
		exprVal.setContextSize(size);
	}

	
	public void setPosition(int pos) {
		// TODO Auto-generated method stub
		exprVal.setPosition(pos);
	}

	
	public String toString() {
		// TODO Auto-generated method stub
		return "$"+exprName;
	}
}
