import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

class ExtractInterfaceListener extends CBaseListener {
	CParser parser;
	public ExtractInterfaceListener(CParser parser) {
		this.parser = parser;
	}
	
	/* Listen to matches of method declaration */
	@Override
	public void enterFunctionDefinition(CParser.FunctionDefinitionContext ctx) {
		System.out.println(ctx.getText());
	}
}

public class Main {
	static String gotoFilePath = "/Users/akalia/Documents/workspace/fastpp/src/goto.c";
	
	public static void main(String args[]) throws FileNotFoundException {
		String code = getCode(gotoFilePath);
		CharStream charStream = new ANTLRInputStream(code);
		
		CLexer lexer = new CLexer(charStream);
		CommonTokenStream tokens = new CommonTokenStream(lexer);
		CParser parser = new CParser(tokens);
		ParserRuleContext tree = parser.compilationUnit();
		
		ParseTreeWalker walker = new ParseTreeWalker();
		ExtractInterfaceListener extractor = new ExtractInterfaceListener(parser);
		walker.walk(extractor, tree);
	}
	
	
	private static String getCode(String gotoFilePath) throws FileNotFoundException {
		Scanner c = new Scanner(new File(gotoFilePath));
		String res = "";
		while(c.hasNext()) {
			res += c.nextLine();
			res += "\n";
		}
		c.close();
		return res;
	}
}
