#include "StoredWords.h"


const map<string, Token::Types> StoredWords::_storedWords = {
	{ "None",	Token::Types::None },
	{ "True",	Token::Types::Boolean },
	{ "False",	Token::Types::Boolean },
	{ "not",	Token::Types::LogicalNot },
	{ "and",	Token::Types::LogicalAnd },
	{ "or",		Token::Types::LogicalOr },
	{ "pass",	Token::Types::Pass },
	{ "if",		Token::Types::ConditionIf },
	//{ "elif",	Token::Types::ConditionElif },
	//{ "else",	Token::Types::ConditionElse },
	{ "while",	Token::Types::WhileLoop },
	{ "def",	Token::Types::FunctionDefinition },
};


Token StoredWords::getWordToken(string word)
{
	if (_storedWords.count(word) == 0)
	{
		return Token(Token::Types::VariableName, word);
	}
	return Token(_storedWords.at(word), word);
}
