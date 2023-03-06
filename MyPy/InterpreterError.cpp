#include "InterpreterError.h"


InterpreterError::InterpreterError(string msg)
	: _msg(msg)
{
}
string InterpreterError::what() const
{
	return _msg;
}


ZeroDivisionError::ZeroDivisionError()
	: InterpreterError("ZeroDivisionError: division by zero")
{
}

NameError::NameError(string msg)
	: InterpreterError("NameError: " + msg)
{
}

TypeError::TypeError(string msg)
	: InterpreterError("TypeError: " + msg)
{
}

IndentationError::IndentationError(string msg)
	: InterpreterError("IndentationError: " + msg)
{
}

SyntaxError::SyntaxError(string msg, string text, size_t pos)
{
	_msg = text + "\n";
	for (size_t i = 0; i < pos; i++, _msg += ' ');
	_msg += "^\nSyntaxError: " + msg;
}
