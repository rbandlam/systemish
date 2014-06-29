import java.util.LinkedList;
import java.util.Queue;

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.misc.Pair;

public class LocalVariableExtractor extends CBaseListener {
	CParser parser;
	TokenStream tokens;
	Queue<VariableDecl> ret;		// Type, Identifier
	Debug debug;
	
	public LocalVariableExtractor(CParser parser) {
		this.parser = parser;
		tokens = parser.getTokenStream();
		ret = new LinkedList<VariableDecl>();
		this.debug = new Debug();
	}

	 // declaration ~ declarationSpecifiers initDeclaratorList? ';'
	 // initDeclarator ~ declarator | declarator '=' initializer
	@Override
	public void enterDeclaration(CParser.DeclarationContext ctx) {
		// The type of the declaration (for example, volatile int*)
		String declarationSpecifier = spaceSeparate(ctx.declarationSpecifiers());
		debug.println("LocalVariableExtractor found declarationSpecifier: `" + 
				declarationSpecifier + "`" );
		
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
		String declarator = initDeclaratorList.initDeclarator().declarator().getText();
		String initializer = "";
		if(initDeclaratorList.initDeclarator().initializer() != null) {
			initializer = initDeclaratorList.initDeclarator().initializer().getText();
		}
		
		VariableDecl var = new VariableDecl(declarationSpecifier, declarator, initializer);
		ret.add(var);
		debug.println("\tLocalVariableExtractor found declaration: " + var.toString());

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