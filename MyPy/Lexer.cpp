#include "Lexer.h"
#include "InterpreterError.h"
#include "StoredWords.h"


Lexer::Lexer(string txt)
	: _text(txt), _pos(0), _currChar(txt[0])
{
}

Token Lexer::getNextToken()
{
	if (isspace(_currChar))
	{
		skip_whitespace();
	}

	_posStack.push(_pos);


	if (_currChar == '\0')
	{
		return Token::Types::eof;
	}
	else if (isdigit(_currChar) || _currChar == '.')
	{
		string num;
		while (isdigit(_currChar) || _currChar == '.')
		{
			num += _currChar;
			advance();
		}

		if (num.find('.') == std::string::npos)
		{
			return Token(Token::Types::IntNumber, num);
		}
		return Token(Token::Types::FloatNumber, num);
	}
	else if (isalpha(_currChar) || _currChar == '_')
	{
		string word;
		while (isalpha(_currChar) || isdigit(_currChar) || _currChar == '_')
		{
			word += _currChar;
			advance();
		}

		return StoredWords::getWordToken(word);
	}
	else if (_currChar == '(')
	{
		advance();
		return Token::Types::LeftParenthesis;
	}
	else if (_currChar == ')')
	{
		advance();
		return Token::Types::RightParenthesis;
	}
	else if (_currChar == '[')
	{
		advance();
		return Token::Types::LeftBrackets;
	}
	else if (_currChar == ']')
	{
		advance();
		return Token::Types::RightBrackets;
	}
	else if (_currChar == '#')
	{
		advance();
		return Token::Types::RemarkSign;
	}
	else if (_currChar == ',')
	{
		advance();
		return Token::Types::Comma;
	}
	else if (_currChar == ':')
	{
		advance();
		return Token::Types::Colon;
	}
	else if (_currChar == '=')
	{
		advance();

		if (_currChar == '=')
		{
			advance();
			return Token::Types::EqualTo;
		}

		return Token::Types::Put;
	}
	else if (_currChar == '!')
	{
		advance();

		if (_currChar == '=')
		{
			advance();
			return Token::Types::NotEqualTo;
		}

		throw SyntaxError("invalid syntax", _text, _pos);
	}
	else if (_currChar == '+')
	{
		advance();

		if (_currChar == '=')
		{
			advance();
			return Token::Types::PlusPut;
		}

		return Token::Types::Plus;
	}
	else if (_currChar == '-')
	{
		advance();

		if (_currChar == '=')
		{
			advance();
			return Token::Types::MinusPut;
		}

		return Token::Types::Minus;
	}
	else if (_currChar == '*')
	{
		advance();

		if (_currChar == '*')
		{
			advance();

			if (_currChar == '=')
			{
				advance();
				return Token::Types::PowerPut;
			}

			return Token::Types::Power;
		}
		else if (_currChar == '=')
		{
			advance();
			return Token::Types::MulPut;
		}

		return Token::Types::Mul;
	}
	else if (_currChar == '/')
	{
		advance();

		if (_currChar == '=')
		{
			advance();
			return Token::Types::FloatDivPut;
		}
		else if (_currChar == '/')
		{
			advance();

			if (_currChar == '=')
			{
				advance();
				return Token::Types::FloorDivPut;
			}

			return Token::Types::FloorDiv;
		}

		return Token::Types::FloatDiv;
	}
	else if (_currChar == '%')
	{
		advance();

		if (_currChar == '=')
		{
			advance();
			return Token::Types::ModuluPut;
		}

		return Token::Types::Modulu;
	}
	else if (_currChar == '&')
	{
		advance();

		if (_currChar == '=')
		{
			advance();
			return Token::Types::BitwiseAndPut;
		}

		return Token::Types::BitwiseAnd;
	}
	else if (_currChar == '|')
	{
		advance();

		if (_currChar == '=')
		{
			advance();
			return Token::Types::BitwiseOrPut;
		}

		return Token::Types::BitwiseOr;
	}
	else if (_currChar == '~')
	{
		advance();
		return Token::Types::BitwiseNot;
	}
	else if (_currChar == '^')
	{
		advance();

		if (_currChar == '=')
		{
			advance();
			return Token::Types::BitwiseXorPut;
		}

		return Token::Types::BitwiseXor;
	}
	else if (_currChar == '>')
	{
		advance();

		if (_currChar == '=')
		{
			advance();
			return Token::Types::GreaterThan_or_EqualTo;
		}
		else if (_currChar == '>')
		{
			advance();

			if (_currChar == '=')
			{
				advance();
				return Token::Types::BitwiseRightShiftPut;
			}

			return Token::Types::BitwiseRightShift;
		}

		return Token::Types::GreaterThan;
	}
	else if (_currChar == '<')
	{
		advance();

		if (_currChar == '=')
		{
			advance();
			return Token::Types::LessThan_or_EqualTo;
		}
		else if (_currChar == '<')
		{
			advance();

			if (_currChar == '=')
			{
				advance();
				return Token::Types::BitwiseLeftShiftPut;
			}

			return Token::Types::BitwiseLeftShift;
		}

		return Token::Types::LessThan;
	}
	else if (_currChar == '"' || _currChar == '\'')
	{
		string str;
		const char firstChar = _currChar;
		advance();
		bool read = true;

		while (read)
		{
			if (_currChar == '\0')
			{
				throw SyntaxError("unterminated string literal", _text, _pos);
			}
			else if (_currChar == firstChar)
			{
				if (str.empty())
				{
					// when the current token is '' or ""
					read = false;
					advance();
				}
				else if (str[str.length() - 1] != '\\')
				{
					read = false;
					advance();
				}
			}

			if (read)
			{
				str += _currChar;
				advance();
			}
		}

		replaceAll(str, "\\\'", "\'");
		replaceAll(str, "\\\"", "\"");


		return Token(Token::Types::String, str);
	}

	throw SyntaxError("invalid syntax", _text, _pos);
}

// back to previous token
void Lexer::back()
{
	if (_posStack.size() < 2)
		return;

	_posStack.pop();
	_pos = _posStack.top();
	_posStack.pop();
	_currChar = _text[_pos];
}
string Lexer::getText()
{
	return _text;
}
size_t Lexer::getPos()
{
	return _pos;
}

void Lexer::advance()
{
	if (_pos >= _text.length())
	{
		_currChar = '\0';
	}
	else
	{
		_currChar = _text[++_pos];
	}
}
void Lexer::skip_whitespace()
{
	while (isspace(_currChar))
	{
		advance();
	}
}
