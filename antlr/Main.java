import java.io.File;
import java.io.FileNotFoundException;
import java.util.List;
import java.util.Scanner;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.misc.NotNull;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

class ExtractInterfaceListener extends CBaseListener {
	CParser parser;
	TokenStream tokens;
	public ExtractInterfaceListener(CParser parser) {
		this.parser = parser;
		tokens = parser.getTokenStream();
	}
	
	
	/* Listen to matches of method declaration */
	@Override
	public void enterFunctionDefinition(CParser.FunctionDefinitionContext ctx) {
	//	printContext(ctx);
	}
	
	/* Listen to variable defns */
	@Override
	public void enterInitDeclarator(CParser.InitDeclaratorContext ctx) {	
		System.out.println(ctx.declarator().getText());
	}

	@Override
	public void enterEveryRule(@NotNull ParserRuleContext ctx) {
		//printContext(ctx);
	}

	private void printContext(ParserRuleContext ctx) {
		int startIndex = ctx.getStart().getTokenIndex();
		int stopIndex = ctx.getStop().getTokenIndex();

		for(int i = startIndex; i <= stopIndex; i ++) {
			System.out.print(tokens.get(i).getText() + " ");	
		}
		
		System.out.println("\n");
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
		
		// Parse and get the root of the parse tree
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
