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
		
		String declarationSpecifier = spaceSeparate(ctx.declarationSpecifiers());
		rewriter.delete(ctx.declarationSpecifiers().start, ctx.declarationSpecifiers().stop);
		debug.println("LocalVariableReplacer deleting declarationSpecifier: `" + 
			declarationSpecifier + "`" );
		
		deleteNonInitializedDeclarators(ctx.initDeclaratorList());
	}
	
	private void deleteNonInitializedDeclarators(
			CParser.InitDeclaratorListContext ctx) {
		CParser.InitDeclaratorContext idc = ctx.initDeclarator();
		CParser.InitializerContext ic = idc.initializer();
		if(ic == null) {
			debug.println("LocalVariableReplacer deleting non-initialized declarator: `" + 
					spaceSeparate(idc));
			rewriter.delete(idc.start, idc.stop);
		}
		
		if(ctx.initDeclaratorList() == null) {
			return;
		}
		
		deleteNonInitializedDeclarators(ctx.initDeclaratorList());
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
