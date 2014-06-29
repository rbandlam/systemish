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

	@Override
	public void enterInitDeclarator(CParser.InitDeclaratorContext ctx) {
		debug.println("LocalVariableReplacer replacing " + ctx.start.getText());
		rewriter.replace(ctx.start, "baa baa");
	}
}
