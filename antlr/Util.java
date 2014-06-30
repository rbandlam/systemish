import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.TokenStream;

class Debug {
	static int debug = 1;
	public void println(String s) {
		if(debug == 1) {
			System.out.flush();		// For ordering b/w stderr and stdout
			System.err.println(s);
		}
	}
	
	public void print(String s) {
		if(debug == 1) {
			System.out.flush();
			System.err.print(s);
		}
	}
	
	// ParserRuleContext.getText() seems to ignore the hidden tokens
	// Use this for printing contexts instead
	public String btrText(ParserRuleContext ctx, TokenStream tokens) {
		int startIndex = ctx.getStart().getTokenIndex();
		int stopIndex = ctx.getStop().getTokenIndex();

		String ret = "";
		for(int i = startIndex; i <= stopIndex; i ++) {
			ret = ret + tokens.get(i).getText() + "";	
		}
		
		return ret;
	}
}

class VariableDecl {
	String type;
	String name;
	String value;
	
	public VariableDecl(String type, String name, String value) {
		this.type = type;
		this.name = name;
		this.value = value;
	}
	
	public String toString() {
		if(value.contentEquals("")) {
			return type + " " + name + " = " + "??";
		} else {
			return type + " " + name + " = " + value;
		}
	}
	
	@Override
	public boolean equals(Object o) {
		VariableDecl ov = (VariableDecl) o;
		if(this.name.contentEquals(ov.name)) {
			return true;
		}
		return false;
	}
}