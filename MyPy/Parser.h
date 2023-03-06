#pragma once

#include "ASTNode.h"
#include "Lexer.h"


class Parser
{
public:
	Parser(string code);

	shared_ptr<AST::BaseNode> parse();

private:
	void eat(Token::Types type);
	void back();

	shared_ptr<AST::BaseNode> help1();
	shared_ptr<AST::BaseNode> help2();
	shared_ptr<AST::BaseNode> help3();
	shared_ptr<AST::BaseNode> help4();
	shared_ptr<AST::BaseNode> help5();
	shared_ptr<AST::BaseNode> help6();
	shared_ptr<AST::BaseNode> help7();
	shared_ptr<AST::BaseNode> help8();
	shared_ptr<AST::BaseNode> help9();
	shared_ptr<AST::BaseNode> help10();
	shared_ptr<AST::BaseNode> calcExpr();

	shared_ptr<AST::BaseNode> parseVariableName();
	shared_ptr<AST::BaseNode> createList();
	shared_ptr<AST::BaseNode> parseCondition();
	shared_ptr<AST::BaseNode> parseWhile();
	shared_ptr<AST::BaseNode> parseFunctionDefinition();
	shared_ptr<AST::BaseNode> parseFunctionCall();
	
	void noMoreLinesWhenShould(string line, size_t lineNumber); // throw exception when should appear new command but it does not. for example: empty line after `if` or `while`
	vector<shared_ptr<AST::BaseNode>> getAstCommands(); // return the commands as AST list


	Lexer _lexer;
	Token _currToken;

	vector<Token::Types> _allowedTokens; // tokens that can appear at end of `Parser::parse`
};


bool shouldReadMultiLine(string cmd);
