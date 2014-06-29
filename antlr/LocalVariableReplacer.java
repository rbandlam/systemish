import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.TokenStreamRewriter;

public class LocalVariableReplacer extends CBaseListener {
	CParser parser;
	TokenStream tokens;
	TokenStreamRewriter rewriter;
	Debug debug;
	
	public LocalVariableReplacer(CParser parser, TokenStreamRewriter rewriter) {
		this.parser = parser;
		tokens = parser.getTokenStream();
		this.rewriter = rewriter;
		this.debug = new Debug();
	}

	 // declaration ~ declarationSpecifiers initDeclaratorList? ';'
	 // initDeclarator ~ declarator | declarator '=' initializer
	@Override
	public void enterDeclaration(CParser.DeclarationContext ctx) {
		if(ctx.initDeclaratorList() == null) {
			debug.println("LocalVariableReplacer deleting declaration " + spaceSeparate(ctx));
			rewriter.delete(ctx.start, ctx.stop);
		} else {
			// The type of the declaration (for example, volatile int*)
			String declarationSpecifier = spaceSeparate(ctx.declarationSpecifiers());
			rewriter.delete(ctx.declarationSpecifiers().start, ctx.declarationSpecifiers().stop);
			debug.println("LocalVariableReplacer deleting declarationSpecifier `" + 
				declarationSpecifier + "`" );
		}
		// The identifiers declared	
		//extractDeclarators(declarationSpecifier, ctx.initDeclaratorList()); 
	}
	
	// Return a space-separate list of the tokens in this ParserRuleContext
	private String spaceSeparate(ParserRuleContext ctx) {
		int startIndex = ctx.getStart().getTokenIndex();
		int stopIndex = ctx.getStop().getTokenIndex();

		String ret = "";
		for(int i = startIndex; i <= stopIndex; i ++) {
			ret = ret + tokens.get(i).getText() + "";	
		}
			
		return ret;
	}
}
