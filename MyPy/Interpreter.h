#pragma once

#include "Parser.h"
#include "VariablesSystem.h"


class Interpreter
{
public:
	Interpreter(VariablesSystem variablesSystem = {});
	~Interpreter();

	void run();

	shared_ptr<BaseObject> interpret(string text);
	shared_ptr<BaseObject> interpret(shared_ptr<AST::BaseNode> node);
	void interpret(vector<shared_ptr<AST::BaseNode>> commands);

private:
	string getInput(string txt, istream& istr); // prints the text (e.g. ">>>") and then receives commands

	shared_ptr<BaseObject> interpret(AST::BinOp* node);
	shared_ptr<BaseObject> interpret(AST::UnaryOp* node);
	shared_ptr<BaseObject> interpret(AST::Literal* node);
	shared_ptr<BaseObject> interpret(AST::VariableName* node);
	void interpret(AST::Put* node);
	void interpret(AST::Condition* node);
	void interpret(AST::WhileLoop* node);
	void interpret(AST::FunctionDefinition* node);
	shared_ptr<BaseObject> interpret(AST::FunctionCall* node);


	VariablesSystem _variables;
};
