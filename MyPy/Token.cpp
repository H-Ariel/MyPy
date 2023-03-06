#include "Token.h"
#include "InterpreterError.h"


Token::Token(Types t, string v)
	: type(t), value(v)
{
}

bool Token::isArithmeticOperator()
{
	switch (type)
	{
	case Types::Plus:
	case Types::Minus:
	case Types::Mul:
	case Types::FloatDiv:
	case Types::FloorDiv:
	case Types::Modulu:
	case Types::Power:
	case Types::BitwiseAnd:
	case Types::BitwiseOr:
	case Types::BitwiseNot:
	case Types::BitwiseXor:
	case Types::BitwiseRightShift:
	case Types::BitwiseLeftShift:
		return true;
	}

	return false;
}
bool Token::isComparisonOperator()
{
	switch (type)
	{
	case Types::GreaterThan:
	case Types::LessThan:
	case Types::GreaterThan_or_EqualTo:
	case Types::LessThan_or_EqualTo:
	case Types::EqualTo:
	case Types::NotEqualTo:
		return true;
	}

	return false;
}
bool Token::isAssignmentOperator()
{
	switch (type)
	{
	case Types::PlusPut:
	case Types::MinusPut:
	case Types::MulPut:
	case Types::PowerPut:
	case Types::FloatDivPut:
	case Types::FloorDivPut:
	case Types::ModuluPut:
	case Types::BitwiseAndPut:
	case Types::BitwiseOrPut:
	case Types::BitwiseXorPut:
	case Types::BitwiseRightShiftPut:
	case Types::BitwiseLeftShiftPut:
		return true;
	}

	return false;
}

Token::Types Token::getArithmeticOperator()
{
	switch (type)
	{
	case Types::PlusPut:				return Types::Plus;
	case Types::MinusPut:				return Types::Minus;
	case Types::MulPut:					return Types::Mul;
	case Types::PowerPut:				return Types::Power;
	case Types::FloatDivPut:			return Types::FloatDiv;
	case Types::FloorDivPut:			return Types::FloorDiv;
	case Types::ModuluPut:				return Types::Modulu;
	case Types::BitwiseAndPut:			return Types::BitwiseAnd;
	case Types::BitwiseOrPut:			return Types::BitwiseOr;
	case Types::BitwiseXorPut:			return Types::BitwiseXor;
	case Types::BitwiseRightShiftPut:	return Types::BitwiseRightShift;
	case Types::BitwiseLeftShiftPut:	return Types::BitwiseLeftShift;
	}

	throw InterpreterError(__FUNCTION__ " : invalid type");
}

bool Token::operator==(Types type) const { return this->type == type; }
bool Token::operator!=(Types type) const { return this->type != type; }
