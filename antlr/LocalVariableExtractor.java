import java.util.LinkedList;
import java.util.Queue;

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.misc.Pair;

public class LocalVariableExtractor extends CBaseListener {
	CParser parser;
	TokenStream tokens;
	Queue<Pair<String, String>> ret;		// Type, Identifier
	int debug;
	
	public LocalVariableExtractor(CParser parser, int debug) {
		this.parser = parser;
		tokens = parser.getTokenStream();
		ret = new LinkedList<Pair<String, String>>();
		this.debug = debug;
	}

	 // declaration ~ declarationSpecifiers initDeclaratorList? ';'
	 // initDeclarator ~ declarator | declarator '=' initializer
	@Override
	public void enterDeclaration(CParser.DeclarationContext ctx) {
		// The type of the declaration (for example, volatile int*)
		String declarationSpecifier = spaceSeparate(ctx.declarationSpecifiers());
		
		// The identifiers declared	
		extractDeclarators(declarationSpecifier, ctx.initDeclaratorList()); 
	}

	// Extract the declarators in the declaration. The declaration has a list
	// of initDeclarators, which we can pass recursively. It's hard to do this 
	// iteratively because initDeclaratorList does not actually expose a List.
	private void extractDeclarators(String declarationSpecifier,
			CParser.InitDeclaratorListContext initDeclaratorList) {
		if(initDeclaratorList == null) {
			return;
		}
		ret.add(new Pair<String, String>(declarationSpecifier,
				initDeclaratorList.initDeclarator().declarator().getText()));
		extractDeclarators(declarationSpecifier, initDeclaratorList.initDeclaratorList());
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