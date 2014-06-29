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
		
		String declarationSpecifier = debug.btrText(ctx.declarationSpecifiers(), tokens);
		rewriter.delete(ctx.declarationSpecifiers().start, ctx.declarationSpecifiers().stop);
		debug.println("LocalVariableReplacer deleting declarationSpecifier: `" + 
			declarationSpecifier + "`" );
		
		// Delete the space after the declarationSpecifier
		int stopIndex = ctx.declarationSpecifiers().stop.getTokenIndex();
		rewriter.delete(stopIndex + 1);
		
		deleteNonInitializedDeclarators(ctx.initDeclaratorList());
	}
	
	private void deleteNonInitializedDeclarators(
			CParser.InitDeclaratorListContext ctx) {
		CParser.InitDeclaratorContext idc = ctx.initDeclarator();
		CParser.InitializerContext ic = idc.initializer();
		if(ic == null) {
			debug.println("\tLocalVariableReplacer deleting non-initialized declarator: `" + 
					debug.btrText(idc, tokens) + "`");
			rewriter.delete(idc.start, idc.stop);
		}
		
		if(ctx.initDeclaratorList() == null) {
			return;
		}
		
		deleteNonInitializedDeclarators(ctx.initDeclaratorList());
	}
}
