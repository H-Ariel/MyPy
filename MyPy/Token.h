#pragma once

#include "framework.h"


class Token
{
public:
	enum class Types
	{
		eof = 0,

		// arithmetic operators
		Plus = 0x01,			// +
		Minus,					// -
		Mul,					// *
		Power,					// **
		FloatDiv,				// /
		FloorDiv,				// //
		Modulu,					// %

		// bitwise operators
		BitwiseAnd,				// &
		BitwiseOr,				// |
		BitwiseNot,				// ~
		BitwiseXor,				// ^
		BitwiseRightShift,		// >>
		BitwiseLeftShift,		// <<

		// assignment operator
		Put,					// =
		PlusPut,				// +=
		MinusPut,				// -=
		MulPut,					// *=
		PowerPut,				// **=
		FloatDivPut,			// /=
		FloorDivPut,			// //=
		ModuluPut,				// %=
		BitwiseAndPut,			// &=
		BitwiseOrPut,			// |=
		BitwiseXorPut,			// ^=
		BitwiseRightShiftPut,	// >>=
		BitwiseLeftShiftPut,	// <<=

		// comparison operators
		GreaterThan = 0x20,		// >
		LessThan,				// <
		EqualTo,				// ==
		NotEqualTo,				// !=
		GreaterThan_or_EqualTo,	// >=
		LessThan_or_EqualTo,	// <=

		// logical operators
		LogicalNot,				// not X
		LogicalAnd,				// and
		LogicalOr,				// or

		// literals
		None,
		FloatNumber,
		IntNumber,
		String,
		Boolean,
		List,

		VariableName,

		// punctuators
		LeftParenthesis,		// (
		RightParenthesis,		// )
		LeftBrackets,			// [
		RightBrackets,			// ]
		LeftBraces,				// {
		RightBraces,			// }
		Colon,					// :
		Comma,					// ,
		Dot,					// .
		RemarkSign,				// #

		// commands
		Pass,					// pass		
		ConditionIf,			// if ... :
		//ConditionElif,			// elif ... :
		//ConditionElse,			// else:
		WhileLoop,				// while ... :
		FunctionDefinition,			// def ... :
		FunctionCall,				// (used for AST node)
	};

	Token(Types t = Types::eof, string v = "");

	bool isArithmeticOperator();
	bool isComparisonOperator();
	bool isAssignmentOperator();

	Types getArithmeticOperator(); // return tne arithmetic operator of assignment operator

	bool operator==(Types type) const;
	bool operator!=(Types type) const;


	string value;
	Types type;
};
