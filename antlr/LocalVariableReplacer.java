import java.util.LinkedList;
import java.util.Queue;

import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.TokenStreamRewriter;
import org.antlr.v4.runtime.misc.Pair;


public class LocalVariableReplacer extends CBaseListener {
	CParser parser;
	TokenStream tokens;
	TokenStreamRewriter rewriter;
	
	public LocalVariableReplacer(CParser parser, TokenStreamRewriter rewriter) {
		this.parser = parser;
		tokens = parser.getTokenStream();
		this.rewriter = rewriter;
	}

	@Override
	public void enterInitDeclarator(CParser.InitDeclaratorContext ctx) {
		// The type of the declaration (for example, volatile int*)
		int startIndex = ctx.getStart().getTokenIndex();

		rewriter.replace(ctx.start, "baa baa");
	}
}
