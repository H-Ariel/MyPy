#pragma once

#include "Token.h"


class StoredWords
{
public:
	// if `word` is a stored word it returns it type. else - it consider it as variable name
	static Token getWordToken(string word);


private:
	static const map<string, Token::Types> _storedWords;
};
