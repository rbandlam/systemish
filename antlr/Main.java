import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

import org.antlr.v4.runtime.ANTLRInputStream;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.CommonTokenStream;
import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.TokenStreamRewriter;
import org.antlr.v4.runtime.misc.Pair;
import org.antlr.v4.runtime.tree.ParseTreeWalker;

public class Main {
	static String gotoFilePath = "/Users/akalia/Documents/workspace/fastpp/src/goto.c";
	
	public static void main(String args[]) throws FileNotFoundException {
		String code = getCode(gotoFilePath);
		moveLocalVarsToTop(code);
		replaceLocalVariables(code);
	}
	
	private static void replaceLocalVariables(String code) {
		CharStream charStream = new ANTLRInputStream(code);
		
		CLexer lexer = new CLexer(charStream);
		CommonTokenStream tokens = new CommonTokenStream(lexer);
		TokenStreamRewriter rewriter = new TokenStreamRewriter(tokens);
		CParser parser = new CParser(tokens);
		
		// Parse and get the root of the parse tree
		ParserRuleContext tree = parser.compilationUnit();

		LocalVariableReplacer replacer = new LocalVariableReplacer(parser, rewriter);
		
		ParseTreeWalker walker = new ParseTreeWalker();
		walker.walk(replacer, tree);
		
		System.out.println(rewriter.getText());
	}

	private static String moveLocalVarsToTop(String code) {
		CharStream charStream = new ANTLRInputStream(code);
		
		CLexer lexer = new CLexer(charStream);
		CommonTokenStream tokens = new CommonTokenStream(lexer);
		CParser parser = new CParser(tokens);
		
		// Parse and get the root of the parse tree
		ParserRuleContext tree = parser.compilationUnit();

		ParseTreeWalker walker = new ParseTreeWalker();
		LocalVariableExtractor extractor = new LocalVariableExtractor(parser);
		walker.walk(extractor, tree);
		
		for(VariableDecl var : extractor.ret) {
			System.out.println(var.type + " " + var.name + "[BATCH_SIZE];");
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
