import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.TokenStreamRewriter;

public class LocalVariableReplacer extends CBaseListener {
	CParser parser;
	TokenStream tokens;
	TokenStreamRewriter rewriter;
	int debug;
	
	public LocalVariableReplacer(CParser parser, 
			TokenStreamRewriter rewriter, int debug) {
		this.parser = parser;
		tokens = parser.getTokenStream();
		this.rewriter = rewriter;
		this.debug = debug;
	}

	@Override
	public void enterInitDeclarator(CParser.InitDeclaratorContext ctx) {
		rewriter.replace(ctx.start, "baa baa");
	}
}
