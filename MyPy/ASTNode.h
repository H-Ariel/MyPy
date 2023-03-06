#pragma once

#include "Token.h"


// Abstract Syntax Tree

namespace AST
{
	class BaseNode
	{
	public:
		BaseNode(Token token);
		BaseNode(Token token, shared_ptr<BaseNode> left, shared_ptr<BaseNode> right);
		~BaseNode();

		virtual bool isBinOp() { return false; }
		virtual bool isUnaryOp() { return false; }
		virtual bool isLiteral() { return false; }
		virtual bool isVariableName() { return false; }
		virtual bool isPut() { return false; }
		virtual bool isCondition() { return false; }
		virtual bool isWhileLoop() { return false; }
		virtual bool isFunctionDefinition() { return false; }
		virtual bool isFunctionCall() { return false; }


		Token value;
		shared_ptr<BaseNode> right;
		shared_ptr<BaseNode> left;
	};


	class Literal : public BaseNode
	{
	public:
		Literal(Token t);
		bool isLiteral() override { return true; }
	};

	class List : public Literal
	{
	public:
		List();

		vector<shared_ptr<BaseNode>> elements;
	};

	class VariableName : public BaseNode
	{
	public:
		VariableName(Token t);
		bool isVariableName() override { return true; }
	};

	class BinOp : public BaseNode
	{
	public:
		BinOp(Token t, shared_ptr<BaseNode> left, shared_ptr<BaseNode> right);
		bool isBinOp() override { return true; }
	};

	class Put : public BaseNode
	{
	public:
		Put(shared_ptr<BaseNode> left, shared_ptr<BaseNode> right); // left = right
		bool isPut() override { return true; }
	};

	class UnaryOp : public BaseNode
	{
	public:
		UnaryOp(Token t, shared_ptr<BaseNode> expr);
		bool isUnaryOp() override { return true; }
		shared_ptr<BaseNode> getExpr();
	};

	class Condition : public BaseNode
	{
	public:
		Condition(shared_ptr<BaseNode> condition, vector<shared_ptr<BaseNode>> commands);
		bool isCondition() override { return true; }
		shared_ptr<BaseNode> getCondition();

		vector<shared_ptr<BaseNode>> commands;
	};

	class WhileLoop : public BaseNode
	{
	public:
		WhileLoop(shared_ptr<BaseNode> condition, vector<shared_ptr<BaseNode>> commands);
		bool isWhileLoop() override { return true; }
		shared_ptr<BaseNode> getCondition();

		vector<shared_ptr<BaseNode>> commands;
	};

	class FunctionDefinition : public BaseNode
	{
	public:
		FunctionDefinition(string funcName, vector<string> argsNames, vector<shared_ptr<BaseNode>> commands);
		bool isFunctionDefinition() override { return true; }

		string funcName;
		vector<string> argsNames;
		vector<shared_ptr<BaseNode>> commands;
	};
	
	class FunctionCall : public BaseNode
	{
	public:
		FunctionCall(string funcName, vector<shared_ptr<BaseNode>> args, map<string, shared_ptr<BaseNode>> kwargs);
		bool isFunctionCall() override { return true; }

		string funcName;
		vector<shared_ptr<BaseNode>> args;
		map<string, shared_ptr<BaseNode>> kwargs;
	};
}
