#include "ASTNode.h"

namespace AST
{
	BaseNode::BaseNode(Token token)
		: value(token), right(nullptr), left(nullptr)
	{
	}
	BaseNode::BaseNode(Token token, shared_ptr<BaseNode> left, shared_ptr<BaseNode> right)
		: value(token), right(right), left(left)
	{
	}
	BaseNode::~BaseNode()
	{
	}

	Literal::Literal(Token t)
		: BaseNode(t)
	{
	}

	List::List()
		: Literal(Token::Types::List)
	{
	}

	VariableName::VariableName(Token t)
		: BaseNode(t)
	{
	}

	Put::Put(shared_ptr<BaseNode> left, shared_ptr<BaseNode> right)
		: BaseNode(Token::Types::Put, left, right)
	{
	}

	BinOp::BinOp(Token t, shared_ptr<BaseNode> left, shared_ptr<BaseNode> right)
		: BaseNode(t, left, right)
	{
	}

	UnaryOp::UnaryOp(Token t, shared_ptr<BaseNode> expr)
		: BaseNode(t, expr, nullptr)
	{
	}
	shared_ptr<BaseNode> UnaryOp::getExpr()
	{
		return left;
	}

	Condition::Condition(shared_ptr<BaseNode> condition, vector<shared_ptr<BaseNode>> commands)
		: BaseNode(Token::Types::ConditionIf, condition, nullptr), commands(commands)
	{
	}
	shared_ptr<BaseNode> Condition::getCondition()
	{
		return left;
	}

	WhileLoop::WhileLoop(shared_ptr<BaseNode> condition, vector<shared_ptr<BaseNode>> commands)
		: BaseNode(Token::Types::WhileLoop, condition, nullptr), commands(commands)
	{
	}
	shared_ptr<BaseNode> WhileLoop::getCondition()
	{
		return left;
	}

	AST::FunctionDefinition::FunctionDefinition(string funcName, vector<string> argsNames, vector<shared_ptr<BaseNode>> commands)
		: BaseNode(Token::Types::FunctionDefinition), funcName(funcName), argsNames(argsNames), commands(commands)
	{
	}

	AST::FunctionCall::FunctionCall(string funcName, vector<shared_ptr<BaseNode>> args, map<string, shared_ptr<BaseNode>> kwargs)
		: BaseNode(Token::Types::FunctionCall), funcName(funcName), args(args), kwargs(kwargs)
	{
	}
}
