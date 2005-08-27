public class Alist {

	public Expr e;
	public Alist next;
	public Alist (){
		next = null;
	}
	public String toString(){
		Alist temp = this;
		String s = "";
		while(temp!=null){
			s = s+temp.e;
			temp = temp.next;
			if (temp!=null)
				s = s+" ,";
		}
		return s;
	}

	public void reset(){
		Alist temp = this;
		while(temp!=null){
			temp.e.reset();
			temp = temp.next;
		}
	}
	
}
