class Debug {
	
	static int debug = 1;
	public void println(String s) {
		if(debug == 1) {
			System.err.println(s);
		}
	}
	
	public void print(String s) {
		if(debug == 1) {
			System.err.print(s);
		}
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
}