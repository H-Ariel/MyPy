#pragma once

#include "framework.h"


class InterpreterError
{
public:
	InterpreterError(string msg = "");
	string what() const;

protected:
	string _msg;
};

#define THROW_NOT_IMPLETED throw InterpreterError(__FUNCTION__ " : not impleted")


class ZeroDivisionError : public InterpreterError
{
public:
	ZeroDivisionError();
};

class NameError : public InterpreterError
{
public:
	NameError(string msg);
};

class TypeError : public InterpreterError
{
public:
	TypeError(string msg);
};

class IndentationError : public InterpreterError
{
public:
	IndentationError(string msg);
};

class SyntaxError : public InterpreterError
{
public:
	SyntaxError(string msg, string text, size_t pos);
};
