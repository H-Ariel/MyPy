#include "Interpreter.h"
#include "InterpreterError.h"


Interpreter::Interpreter(VariablesSystem variablesSystem)
	: _variables(variablesSystem)
{
}
Interpreter::~Interpreter()
{
}

void Interpreter::run()
{
	cout << "Python by Ariel Halili" << endl;

	for (string cmd; cmd != "quit()" && cmd != "exit()";)
	{
		cmd = getInput(">>> ", cin);
		try
		{
			shared_ptr<BaseObject> obj = interpret(cmd);
			if (obj != nullptr)
			{
				cout << obj->toString() << endl;
			}
		}
		catch (InterpreterError& e)
		{
			cout << e.what() << endl;
		}
	}
}

string Interpreter::getInput(string txt, istream& istr)
{
	string code("");

	cout << txt;
	getline(istr, code);

	if (shouldReadMultiLine(code))
	{
		string line;

		if (!txt.empty())
		{
			txt = "... ";
		}

		do {
			line = getInput(txt, istr);
			code += '\n' + line;
			if (endsWith(code, "\n"))
			{
				line = "";
			}
		} while (line != "");
	}

	return code;
}

shared_ptr<BaseObject> Interpreter::interpret(string text)
{
	Parser parser(text);
	shared_ptr<AST::BaseNode> tree = parser.parse();
	shared_ptr<BaseObject> obj = interpret(tree);
	return obj;
}

shared_ptr<BaseObject> Interpreter::interpret(shared_ptr<AST::BaseNode> sNode)
{
	shared_ptr<BaseObject> obj;

	AST::BaseNode* node = sNode.get();

	if (node == nullptr)
	{
	}
	else if (node->isBinOp())
	{
		obj = interpret((AST::BinOp*)node);
	}
	else if (node->isUnaryOp())
	{
		obj = interpret((AST::UnaryOp*)node);
	}
	else if (node->isVariableName())
	{
		obj = interpret((AST::VariableName*)node);
	}
	else if (node->isLiteral())
	{
		obj = interpret((AST::Literal*)node);
	}
	else if (node->isPut())
	{
		interpret((AST::Put*)node);
	}
	else if (node->isCondition())
	{
		interpret((AST::Condition*)node);
	}
	else if (node->isWhileLoop())
	{
		interpret((AST::WhileLoop*)node);
	}
	else if (node->isFunctionDefinition())
	{
		interpret((AST::FunctionDefinition*)node);
	}
	else if (node->isFunctionCall())
	{
		obj = interpret((AST::FunctionCall*)node);
	}
	else
	{
		THROW_NOT_IMPLETED;
	}

	return obj;
}
void Interpreter::interpret(vector<shared_ptr<AST::BaseNode>> commands)
{
	for (shared_ptr<AST::BaseNode>& a : commands)
	{
		interpret(a);
	}
}

shared_ptr<BaseObject> Interpreter::interpret(AST::BinOp* node)
{
	shared_ptr<BaseObject> _a = interpret(node->left), _b = interpret(node->right);
	BaseObject *a = _a.get(), *b = _b.get();

	switch (node->value.type)
	{
	case Token::Types::Plus:					return a->add(b);
	case Token::Types::Minus:					return a->sub(b);
	case Token::Types::Mul:						return a->mul(b);
	case Token::Types::FloatDiv:				return a->fltDiv(b);
	case Token::Types::FloorDiv:				return a->flrDiv(b);
	case Token::Types::Modulu:					return a->mod(b);
	case Token::Types::Power:					return a->pow(b);
	case Token::Types::BitwiseLeftShift:		return a->leftShift(b);
	case Token::Types::BitwiseRightShift:		return a->rightShift(b);
	case Token::Types::BitwiseAnd:				return a->bitwiseAnd(b);
	case Token::Types::BitwiseOr:				return a->bitwiseOr(b);
	case Token::Types::BitwiseXor:				return a->bitwiseXor(b);
	case Token::Types::GreaterThan:				return a->greaterThan(b);
	case Token::Types::LessThan:				return a->lessThan(b);
	case Token::Types::GreaterThan_or_EqualTo:	return a->greaterThanOrEqualTo(b);
	case Token::Types::LessThan_or_EqualTo:		return a->lessThanOrEqualTo(b);
	case Token::Types::EqualTo:					return a->equalTo(b);
	case Token::Types::NotEqualTo:				return a->notEqualTo(b);
	case Token::Types::LogicalAnd:				return a->logicalAnd(b);
	case Token::Types::LogicalOr:				return a->logicalOr(b);
	}

	THROW_NOT_IMPLETED;
}
shared_ptr<BaseObject> Interpreter::interpret(AST::UnaryOp* node)
{
	shared_ptr<BaseObject> a = interpret(node->getExpr());
	switch (node->value.type)
	{
	case Token::Types::Plus:		return a->positive();
	case Token::Types::Minus:		return a->negative();
	case Token::Types::LogicalNot:	return a->logicalNot();
	case Token::Types::BitwiseNot:	return a->bitwiseNot();
	}
	
	THROW_NOT_IMPLETED;
}
shared_ptr<BaseObject> Interpreter::interpret(AST::Literal* node)
{
	return BaseObject::getObject(node, this);
}
shared_ptr<BaseObject> Interpreter::interpret(AST::VariableName* node)
{
	return _variables.get(node->value.value);
}
void Interpreter::interpret(AST::Put* node)
{
	_variables.set(node->left->value.value, interpret(node->right));
}
void Interpreter::interpret(AST::Condition* node)
{
	if (interpret(node->getCondition())->toBoolean()->val)
	{
		interpret(node->commands);
	}
}
void Interpreter::interpret(AST::WhileLoop* node)
{
	while (interpret(node->getCondition())->toBoolean()->val)
	{
		interpret(node->commands);
	}
}
void Interpreter::interpret(AST::FunctionDefinition* node)
{
	_variables.set(node->funcName, allocNew<Function>(node->funcName, node->argsNames, node->commands));
}
shared_ptr<BaseObject> Interpreter::interpret(AST::FunctionCall* node)
{
	BaseObject* obj = _variables.get(node->funcName).get();

	if (obj->type != BaseObject::Types::UserFunction && obj->type != BaseObject::Types::BuiltinFunction)
	{
		throw TypeError("object is not callable");
	}

	return ((BaseFunction*)obj)->run(node->args, node->kwargs, this);
}
