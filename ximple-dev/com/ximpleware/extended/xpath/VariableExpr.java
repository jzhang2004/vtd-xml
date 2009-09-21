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
	
	@Override
	public int adjust(int n) {
		// TODO Auto-generated method stub
		return exprVal.adjust(n);
	}

	@Override
	public boolean evalBoolean(VTDNavHuge vn) {
		// TODO Auto-generated method stub
		return exprVal.evalBoolean(vn);
	}

	@Override
	public int evalNodeSet(VTDNavHuge vn) throws XPathEvalExceptionHuge, NavExceptionHuge {
		// TODO Auto-generated method stub
		return exprVal.evalNodeSet(vn);
	}

	@Override
	public double evalNumber(VTDNavHuge vn) {
		// TODO Auto-generated method stub
		return exprVal.evalNumber(vn);
	}

	@Override
	public String evalString(VTDNavHuge vn) {
		// TODO Auto-generated method stub
		return exprVal.evalString(vn);
	}

	@Override
	public boolean isBoolean() {
		// TODO Auto-generated method stub
		return exprVal.isBoolean();
	}

	@Override
	public boolean isNodeSet() {
		// TODO Auto-generated method stub
		return exprVal.isNodeSet();
	}

	@Override
	public boolean isNumerical() {
		// TODO Auto-generated method stub
		return exprVal.isNumerical();
	}

	@Override
	public boolean isString() {
		// TODO Auto-generated method stub
		return exprVal.isString();
	}

	@Override
	public boolean requireContextSize() {
		// TODO Auto-generated method stub
		return exprVal.requireContextSize();
	}

	@Override
	public void reset(VTDNavHuge vn) {
		// TODO Auto-generated method stub
		exprVal.reset(vn);
	}

	@Override
	public void setContextSize(int size) {
		// TODO Auto-generated method stub
		exprVal.setContextSize(size);
	}

	@Override
	public void setPosition(int pos) {
		// TODO Auto-generated method stub
		exprVal.setPosition(pos);
	}

	@Override
	public String toString() {
		// TODO Auto-generated method stub
		return "$"+exprName;
	}
}
