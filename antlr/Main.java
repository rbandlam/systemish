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
import org.antlr.v4.runtime.misc.Pair;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

public class Main {
	static String gotoFilePath = "/Users/akalia/Documents/workspace/fastpp/src/goto.c";
	
	public static void main(String args[]) throws FileNotFoundException {
		String code = getCode(gotoFilePath);
		code = moveLocalVarsToTop(code);
	}
	
	private static String moveLocalVarsToTop(String code) {
		CharStream charStream = new ANTLRInputStream(code);
		
		CLexer lexer = new CLexer(charStream);
		CommonTokenStream tokens = new CommonTokenStream(lexer);
		CParser parser = new CParser(tokens);
		
		// Parse and get the root of the parse tree
		ParserRuleContext tree = parser.compilationUnit();

		ParseTreeWalker walker = new ParseTreeWalker();
		ExtractLocalVariablesListener extractor = new ExtractLocalVariablesListener(parser);
		walker.walk(extractor, tree);
		
		for(Pair localVar : extractor.ret) {
			System.out.println(localVar.toString());
		}
		return "";
	}

	// Get a String representation of the input code
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
