#pragma once

#include "Token.h"


class Lexer
{
public:
	Lexer(string txt);

	Token getNextToken();

	void back();
	string getText();
	size_t getPos();

private:
	void advance();
	void skip_whitespace();


	const string _text;
	stack<size_t> _posStack;
	size_t _pos;
	char _currChar;
};
