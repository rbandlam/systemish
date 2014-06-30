import java.util.LinkedList;

import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.TokenStreamRewriter;

public class LocalVariableVectorizer extends CBaseListener {
	CParser parser;
	TokenStream tokens;
	TokenStreamRewriter rewriter;
	Debug debug;
	LinkedList<VariableDecl> localVariables;
	
	public LocalVariableVectorizer(CParser parser, 
			TokenStreamRewriter rewriter, LinkedList<VariableDecl> localVariables) {
		this.parser = parser;
		tokens = parser.getTokenStream();
		this.rewriter = rewriter;
		this.debug = new Debug();
		this.localVariables = localVariables;
	}
	
	@Override
	public void enterPrimaryExpression(CParser.PrimaryExpressionContext ctx) {
		String primaryExpression = debug.btrText(ctx, tokens);
		VariableDecl vd = new VariableDecl("", primaryExpression, "");
		if(localVariables.contains(vd)) {
			debug.println(primaryExpression + " is a local variable. Vectorizing.");
			rewriter.replace(ctx.start, primaryExpression + "[I]");
		}
	}
	
}
