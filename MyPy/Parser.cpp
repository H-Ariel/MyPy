#include "Parser.h"
#include "InterpreterError.h"
#include <regex>


#define THROW_INVALID_SYNTAX throw SyntaxError(__FUNCTION__ ": invalid syntax", _lexer.getText(), _lexer.getPos())


// todo: find better position
bool shouldReadMultiLine(string cmd)
{
	const static string PossibleSituations[] = {
		"if", "while", "def" // todo: use stored-words
	};
	const static size_t len = sizeof(PossibleSituations) / sizeof(PossibleSituations[0]);

	size_t i = 0;
	bool readMultiLine = false;

	// Check for multi-line code
	for (i = 0; i < len && !readMultiLine; i++)
	{
		readMultiLine =
			regex_match(cmd, regex("\\s?" + PossibleSituations[i] + "\\s+.*")) ||
			regex_match(cmd, regex("\\s?" + PossibleSituations[i] + "(.*)"));
	}

	return readMultiLine;
}

string get_tabs(string line)
{
	string tabs;
	for (size_t i = 0; i < line.length() && isspace(line[i]); tabs += line[i++]);
	return tabs;
}



Parser::Parser(string code)
	: _lexer(code)
{
	if (!get_tabs(code).empty())
	{
		throw IndentationError("unexpected indent");
	}

	_currToken = _lexer.getNextToken();
}

shared_ptr<AST::BaseNode> Parser::parse()
{
	shared_ptr<AST::BaseNode> node;

	switch (_currToken.type)
	{
	case Token::Types::IntNumber:
	case Token::Types::FloatNumber:
	case Token::Types::String:
	case Token::Types::Boolean:
	case Token::Types::None:
	case Token::Types::LeftParenthesis:
	case Token::Types::LeftBrackets:
	case Token::Types::Plus:
	case Token::Types::Minus:
	case Token::Types::LogicalNot:
	case Token::Types::BitwiseNot:
		node = calcExpr();
		break;

	case Token::Types::VariableName:
		node = parseVariableName();
		break;

	case Token::Types::ConditionIf:
		node = parseCondition();
		break;
	
	case Token::Types::WhileLoop:
		node = parseWhile();
		break;

	case Token::Types::Pass:
		eat(Token::Types::Pass);
		break;

	case Token::Types::FunctionDefinition:
		node = parseFunctionDefinition();
		break;
	}

	if (_currToken == Token::Types::RemarkSign || FindInArray(_allowedTokens, _currToken.type))
	{
	}
	else if (_currToken != Token::Types::eof)
	{
		THROW_INVALID_SYNTAX;
	}

	return node;
}

void Parser::eat(Token::Types type)
{
	if (_currToken != type)
	{
		THROW_INVALID_SYNTAX;
	}
	_currToken = _lexer.getNextToken();
}
void Parser::back()
{
	_lexer.back();
	_currToken = _lexer.getNextToken();
}

// TODO: combine `help[2...8,calcExpr]`
shared_ptr<AST::BaseNode> Parser::help1()
{
	shared_ptr<AST::BaseNode> node;
	Token tok;

	switch (_currToken.type)
	{
	case Token::Types::IntNumber:
	case Token::Types::FloatNumber:
	case Token::Types::String:
	case Token::Types::Boolean:
	case Token::Types::None:
		node = allocNew<AST::Literal>(_currToken);
		eat(_currToken.type);
		break;

	case Token::Types::VariableName:
		node = allocNew<AST::VariableName>(_currToken);
		eat(Token::Types::VariableName);
		break;

	case Token::Types::LeftParenthesis:
		eat(Token::Types::LeftParenthesis);
		node = calcExpr();
		eat(Token::Types::RightParenthesis);
		break;

	case Token::Types::LeftBrackets:
		node = createList();
		break;

	case Token::Types::Plus:
	case Token::Types::Minus:
	case Token::Types::BitwiseNot:
		tok = _currToken;
		eat(tok.type);
		node = allocNew<AST::UnaryOp>(tok, help2());
		break;

	case Token::Types::LogicalNot:
		tok = _currToken;
		eat(tok.type);
		node = allocNew<AST::UnaryOp>(tok, calcExpr());
		break;

	default:
		THROW_INVALID_SYNTAX;
	}

	return node;
}
shared_ptr<AST::BaseNode> Parser::help2()
{
	shared_ptr<AST::BaseNode> node = help1();
	Token tok;

	while (_currToken == Token::Types::Power)
	{
		tok = _currToken;
		eat(_currToken.type);
		node = allocNew<AST::BinOp>(tok, node, help2());
	}

	return node;
}
shared_ptr<AST::BaseNode> Parser::help3()
{
	shared_ptr<AST::BaseNode> node = help2();
	Token tok;

	while (_currToken == Token::Types::Mul
		|| _currToken == Token::Types::FloatDiv
		|| _currToken == Token::Types::FloorDiv
		|| _currToken == Token::Types::Modulu)
	{
		tok = _currToken;
		eat(_currToken.type);
		node = allocNew<AST::BinOp>(tok, node, help2());
	}

	return node;
}
shared_ptr<AST::BaseNode> Parser::help4()
{
	shared_ptr<AST::BaseNode> node = help3();
	Token tok;

	while (_currToken == Token::Types::Plus
		|| _currToken == Token::Types::Minus)
	{
		tok = _currToken;
		eat(_currToken.type);
		node = allocNew<AST::BinOp>(tok, node, help3());
	}

	return node;
}
shared_ptr<AST::BaseNode> Parser::help5()
{
	shared_ptr<AST::BaseNode> node = help4();
	Token tok;

	while (_currToken == Token::Types::BitwiseRightShift
		|| _currToken == Token::Types::BitwiseLeftShift)
	{
		tok = _currToken;
		eat(_currToken.type);
		node = allocNew<AST::BinOp>(tok, node, help4());
	}

	return node;
}
shared_ptr<AST::BaseNode> Parser::help6()
{
	shared_ptr<AST::BaseNode> node = help5();
	Token tok;

	while (_currToken == Token::Types::BitwiseAnd)
	{
		tok = _currToken;
		eat(_currToken.type);
		node = allocNew<AST::BinOp>(tok, node, help5());
	}

	return node;
}
shared_ptr<AST::BaseNode> Parser::help7()
{
	shared_ptr<AST::BaseNode> node = help6();
	Token tok;

	while (_currToken == Token::Types::BitwiseOr
		|| _currToken == Token::Types::BitwiseXor)
	{
		tok = _currToken;
		eat(_currToken.type);
		node = allocNew<AST::BinOp>(tok, node, help6());
	}

	return node;
}
shared_ptr<AST::BaseNode> Parser::help8()
{
	shared_ptr<AST::BaseNode> node = help7();
	Token tok;

	while (_currToken == Token::Types::GreaterThan
		|| _currToken == Token::Types::GreaterThan_or_EqualTo
		|| _currToken == Token::Types::LessThan
		|| _currToken == Token::Types::LessThan_or_EqualTo)
	{
		tok = _currToken;
		eat(_currToken.type);
		node = allocNew<AST::BinOp>(tok, node, help7());
	}

	return node;
}
shared_ptr<AST::BaseNode> Parser::help9()
{
	shared_ptr<AST::BaseNode> node = help8();
	Token tok;

	while (_currToken == Token::Types::EqualTo
		|| _currToken == Token::Types::NotEqualTo)
	{
		tok = _currToken;
		eat(_currToken.type);
		node = allocNew<AST::BinOp>(tok, node, help8());
	}

	return node;
}
shared_ptr<AST::BaseNode> Parser::help10()
{
	shared_ptr<AST::BaseNode> node = help9();
	Token tok;

	while (_currToken == Token::Types::LogicalAnd)
	{
		tok = _currToken;
		eat(_currToken.type);
		node = allocNew<AST::BinOp>(tok, node, help9());
	}

	return node;
}
shared_ptr<AST::BaseNode> Parser::calcExpr()
{
	shared_ptr<AST::BaseNode> node = help10();
	Token tok;

	while (_currToken == Token::Types::LogicalOr)
	{
		tok = _currToken;
		eat(_currToken.type);
		node = allocNew<AST::BinOp>(tok, node, help10());
	}

	return node;
}

shared_ptr<AST::BaseNode> Parser::parseVariableName()
{
	shared_ptr<AST::BaseNode> node;

	Token tok;
	node = allocNew<AST::VariableName>(_currToken);
	eat(Token::Types::VariableName);

	if (_currToken == Token::Types::Put)
	{
		tok = _currToken;
		eat(Token::Types::Put);
		node = allocNew<AST::Put>(node, parse());
		if (node->right == nullptr)
		{
			THROW_INVALID_SYNTAX;
		}
	}
	else if (_currToken.isAssignmentOperator())
	{
		Token::Types op = _currToken.getArithmeticOperator();
		eat(_currToken.type);

		/*
		TODO: in case of list
			>>> a=[1,2,3]
			>>> b=a
			>>> b+=[4]
			>>> a
			[1, 2, 3]
			>>> b
			[1, 2, 3, 4]
		*/
		node = allocNew<AST::Put>(
			node,
			allocNew<AST::BinOp>(
				op,
				node,
				parse()
				)
			);
	}
	else if (_currToken.isArithmeticOperator() || _currToken.isComparisonOperator())
	{
		back();
		node = calcExpr();
	}
	else if (_currToken == Token::Types::LeftParenthesis)
	{
		back();
		node = parseFunctionCall();
	}
	else if (_currToken != Token::Types::eof)
	{
		THROW_INVALID_SYNTAX;
	}


	return node;
}

shared_ptr<AST::BaseNode> Parser::createList()
{
	shared_ptr<AST::List> node(DBG_NEW AST::List());

	eat(Token::Types::LeftBrackets);

	while (_currToken != Token::Types::RightBrackets)
	{
		node->elements.push_back(calcExpr());
		if (_currToken != Token::Types::RightBrackets)
		{
			eat(Token::Types::Comma);
		}
	}

	eat(Token::Types::RightBrackets);

	return node;
}

shared_ptr<AST::BaseNode> Parser::parseCondition()
{
	eat(Token::Types::ConditionIf);
	shared_ptr<AST::BaseNode> conditionAst = calcExpr();
	eat(Token::Types::Colon);
	vector<shared_ptr<AST::BaseNode>> astCommands = getAstCommands();
	// TODO: elif, else
	_currToken = Token::Types::eof; // disable exception from `Parser::parse`
	return allocNew<AST::Condition>(conditionAst, astCommands);
}
shared_ptr<AST::BaseNode> Parser::parseWhile()
{
	eat(Token::Types::WhileLoop);
	shared_ptr<AST::BaseNode> conditionAst = calcExpr();
	eat(Token::Types::Colon);
	vector<shared_ptr<AST::BaseNode>> astCommands = getAstCommands();
	// TODO: else
	_currToken = Token::Types::eof; // disable exception from `Parser::parse`
	return allocNew<AST::WhileLoop>(conditionAst, astCommands);
}

shared_ptr<AST::BaseNode> Parser::parseFunctionDefinition()
{
	eat(Token::Types::FunctionDefinition);
	
	string funcName = _currToken.value;
	eat(Token::Types::VariableName);

	eat(Token::Types::LeftParenthesis);
	vector<string> argsNames; // todo: use default value, e.g. print(1,2, end='\n')
	while (_currToken != Token::Types::RightParenthesis)
	{
		argsNames.push_back(_currToken.value);
		eat(Token::Types::VariableName);
		if (_currToken != Token::Types::RightParenthesis)
		{
			eat(Token::Types::Comma);
		}
	}
	eat(Token::Types::RightParenthesis);
	eat(Token::Types::Colon);

	vector<shared_ptr<AST::BaseNode>> astCommands = getAstCommands();
	_currToken = Token::Types::eof; // disable exception from `Parser::parse`
	return allocNew<AST::FunctionDefinition>(funcName, argsNames, astCommands);
}
shared_ptr<AST::BaseNode> Parser::parseFunctionCall()
{
	string funcName = _currToken.value;
	eat(Token::Types::VariableName);
	eat(Token::Types::LeftParenthesis);

	vector<shared_ptr<AST::BaseNode>> args;
	map<string, shared_ptr<AST::BaseNode>> kwargs;

	_allowedTokens = { Token::Types::Comma, Token::Types::RightParenthesis };

	while (_currToken != Token::Types::RightParenthesis)
	{
		args.push_back(parse());
		if (_currToken != Token::Types::RightParenthesis)
		{
			eat(Token::Types::Comma);
		}
	}
	eat(Token::Types::RightParenthesis);

	_allowedTokens = {};

	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i]->isPut())
		{
			kwargs[args[i]->left->value.value] = args[i]->right;
			args.erase(args.begin() + i);
			i--;
		}
	}

	return allocNew<AST::FunctionCall>(funcName, args, kwargs);
}

void Parser::noMoreLinesWhenShould(string line, size_t lineNumber)
{
	string word;
	size_t i = 0;
	while (isspace(line[i])) i++;
	for (; i < line.size() && !isspace(line[i]); word += line[i++]);
	throw IndentationError("expected an indented block after '" + word + "' statement on line " + to_string(lineNumber));
}
vector<shared_ptr<AST::BaseNode>> Parser::getAstCommands()
{
	vector<string> lines = getCommands(_lexer.getText());
	if (lines.size() == 1) noMoreLinesWhenShould(lines[0], 1);
	
	lines.erase(lines.begin()); // the first line is condition so we delete it
	const string tabs = get_tabs(lines[0]);

	// make sure we have valid indentation
	if (tabs.empty()) noMoreLinesWhenShould(lines[0], 1);

	for (string& line : lines)
	{
		if (!startsWith(line, tabs))
		{
			throw IndentationError("unindent does not match any outer indentation level");
		}
		line = line.substr(tabs.length()); // remove tabs
	}

	vector<string> commandsText;
	const size_t linesAmount = lines.size();
	size_t i;
	for (i = 0; i < linesAmount; i++)
	{
		if (shouldReadMultiLine(lines[i]))
		{
			// for example: a `while` loop in an `if` block
			// if we have sub-block we need to save all sub-block commands' together

			if (i + 1 < linesAmount)
			{
				const string _tabs = get_tabs(lines[i + 1]);
				string newCommandsBlock = lines[i++];
				while (i < linesAmount && startsWith(lines[i], _tabs))
				{
					newCommandsBlock += "\n" + lines[i];
					i++;
				}
				commandsText.push_back(newCommandsBlock);
			}
			else
			{
				noMoreLinesWhenShould(lines[i], i + 2); // +2: because we start from 0 and we delete the first line from list
			}
			i--; // disable previous `i++`
		}
		else
		{
			commandsText.push_back(lines[i]);
		}
	}

	vector<shared_ptr<AST::BaseNode>> astCommands;
	for (const string& cmd : commandsText)
	{
		astCommands.push_back(Parser(cmd).parse());
	}

	return astCommands;
}
