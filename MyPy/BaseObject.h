#pragma once

#include "ASTNode.h"


class Obj_Boolean;
class Interpreter;


class BaseObject
{
public:
	enum class Types
	{
		None,
		FloatNumber,
		IntNumber,
		Boolean,
		String,
		List,
		BuiltinFunction,
		UserFunction
	};

	static shared_ptr<BaseObject> getObject(AST::BaseNode* node, Interpreter* interpreter);


	BaseObject(Types type);
	virtual string toString();
	virtual shared_ptr<Obj_Boolean> toBoolean();

	// for these function: `a` means `this`

	virtual shared_ptr<BaseObject> add(BaseObject* b); // a + b
	virtual shared_ptr<BaseObject> sub(BaseObject* b); // a - b
	virtual shared_ptr<BaseObject> mul(BaseObject* b); // a * b
	virtual shared_ptr<BaseObject> fltDiv(BaseObject* b); // a / b
	virtual shared_ptr<BaseObject> flrDiv(BaseObject* b); // a // b
	virtual shared_ptr<BaseObject> mod(BaseObject* b); // a % b
	virtual shared_ptr<BaseObject> pow(BaseObject* b); // a ** b
	virtual shared_ptr<BaseObject> leftShift(BaseObject* b); // a << b
	virtual shared_ptr<BaseObject> rightShift(BaseObject* b); // a >> b
	virtual shared_ptr<BaseObject> bitwiseAnd(BaseObject* b); // a & b
	virtual shared_ptr<BaseObject> bitwiseOr(BaseObject* b); // a | b
	virtual shared_ptr<BaseObject> bitwiseXor(BaseObject* b); // a ^ b
	virtual shared_ptr<BaseObject> greaterThan(BaseObject* b); // a > b
	virtual shared_ptr<BaseObject> lessThan(BaseObject* b); // a < b
	virtual shared_ptr<BaseObject> greaterThanOrEqualTo(BaseObject* b); // a >= b
	virtual shared_ptr<BaseObject> lessThanOrEqualTo(BaseObject* b); // a <= b
	virtual shared_ptr<BaseObject> equalTo(BaseObject* b); // a == b
	virtual shared_ptr<BaseObject> notEqualTo(BaseObject* b); // a != b
	virtual shared_ptr<BaseObject> logicalAnd(BaseObject* b); // a and b
	virtual shared_ptr<BaseObject> logicalOr(BaseObject* b); // a or b
	virtual shared_ptr<BaseObject> positive(); // +a
	virtual shared_ptr<BaseObject> negative(); // -a
	virtual shared_ptr<BaseObject> bitwiseNot(); // ~a
	virtual shared_ptr<BaseObject> logicalNot(); // not a


	const Types type;


protected:
	typedef long long int IntType;
	typedef long double FloatType;

	static string getTypeName(Types type);
};


class Obj_None : public BaseObject
{
public:
	Obj_None();
	string toString() override;
	shared_ptr<Obj_Boolean> toBoolean() override;
};


class Obj_FloatNumber : public BaseObject
{
public:
	Obj_FloatNumber(FloatType n);

	string toString() override;
	shared_ptr<Obj_Boolean> toBoolean() override;

	shared_ptr<BaseObject> add(BaseObject* b) override;
	shared_ptr<BaseObject> sub(BaseObject* b) override;
	shared_ptr<BaseObject> mul(BaseObject* b) override;
	shared_ptr<BaseObject> fltDiv(BaseObject* b) override;
	shared_ptr<BaseObject> flrDiv(BaseObject* b) override;
	shared_ptr<BaseObject> mod(BaseObject* b) override;
	shared_ptr<BaseObject> pow(BaseObject* b) override;
	shared_ptr<BaseObject> greaterThan(BaseObject* b) override;
	shared_ptr<BaseObject> equalTo(BaseObject* b) override;
	shared_ptr<BaseObject> positive() override;
	shared_ptr<BaseObject> negative() override;


	const FloatType val;
};

class Obj_IntNumber : public BaseObject
{
public:
	Obj_IntNumber(IntType n);
	Obj_IntNumber(bool n);

	string toString() override;
	shared_ptr<Obj_Boolean> toBoolean() override;

	shared_ptr<BaseObject> add(BaseObject* b) override;
	shared_ptr<BaseObject> sub(BaseObject* b) override;
	shared_ptr<BaseObject> mul(BaseObject* b) override;
	shared_ptr<BaseObject> fltDiv(BaseObject* b) override;
	shared_ptr<BaseObject> flrDiv(BaseObject* b) override;
	shared_ptr<BaseObject> mod(BaseObject* b) override;
	shared_ptr<BaseObject> pow(BaseObject* b) override;
	shared_ptr<BaseObject> leftShift(BaseObject* b) override;
	shared_ptr<BaseObject> rightShift(BaseObject*) override;
	shared_ptr<BaseObject> bitwiseAnd(BaseObject* b) override;
	shared_ptr<BaseObject> bitwiseOr(BaseObject* b) override;
	shared_ptr<BaseObject> bitwiseXor(BaseObject* b) override;
	shared_ptr<BaseObject> greaterThan(BaseObject* b) override;
	shared_ptr<BaseObject> equalTo(BaseObject* b) override;
	shared_ptr<BaseObject> positive() override;
	shared_ptr<BaseObject> negative() override;
	shared_ptr<BaseObject> bitwiseNot() override;

	
	const IntType val;
};

class Obj_Boolean : public Obj_IntNumber
{
public:
	Obj_Boolean(bool b);

	string toString() override;
	shared_ptr<BaseObject> logicalNot() override;
};

template <class T>
class Sequence : public BaseObject
{
public:
	Sequence(Types type);
	shared_ptr<Obj_Boolean> toBoolean() override;

	shared_ptr<BaseObject> add(BaseObject* b) override;
	shared_ptr<BaseObject> mul(BaseObject* b) override;
	shared_ptr<BaseObject> greaterThan(BaseObject* b) override;
	shared_ptr<BaseObject> equalTo(BaseObject* b) override;

protected:
	virtual bool areEqual(T a, T b) = 0; // a == b
	virtual bool isGreater(T a, T b) = 0; // a > b

	vector<T> _elements;
};

class Obj_String : public Sequence<char>
{
public:
	Obj_String(string str);
	string toString() override;

private:
	bool areEqual(char a, char b) override;
	bool isGreater(char a, char b) override;
};

class Obj_List : public Sequence<shared_ptr<BaseObject>>
{
public:
	Obj_List();
	Obj_List(vector<shared_ptr<AST::BaseNode>> elements, Interpreter* interpreter);
	string toString() override;

private:
	bool areEqual(shared_ptr<BaseObject> a, shared_ptr<BaseObject> b) override;
	bool isGreater(shared_ptr<BaseObject> a, shared_ptr<BaseObject> b) override;
};

// in python functions are objects...
class BaseFunction : public BaseObject
{
public:
	BaseFunction(Types type, string funcName);
	virtual string toString() override = 0;
	shared_ptr<Obj_Boolean> toBoolean() override;

	virtual shared_ptr<BaseObject> run(vector<shared_ptr<AST::BaseNode>> args, map<string, shared_ptr<AST::BaseNode>> kwargs, Interpreter* parent) = 0;

protected:
	const string funcName;
};

class Function : public BaseFunction
{
public:
	Function(string funcName, vector<string> argsNames, vector<shared_ptr<AST::BaseNode>> commands);
	string toString() override;
	shared_ptr<BaseObject> run(vector<shared_ptr<AST::BaseNode>> args, map<string, shared_ptr<AST::BaseNode>> kwargs, Interpreter* parent) override;

private:
	vector<string> argsNames;
	vector<shared_ptr<AST::BaseNode>> commands;
};

class BuiltinFunction : public BaseFunction
{
public:
	BuiltinFunction(string funcName);
	string toString() override;
};
namespace BuiltinFunctions
{
	class Print : public BuiltinFunction
	{
	public:
		Print();
		shared_ptr<BaseObject> run(vector<shared_ptr<AST::BaseNode>> args, map<string, shared_ptr<AST::BaseNode>> kwargs, Interpreter* parent) override;
	};
	class Input : public BuiltinFunction
	{
	public:
		Input();
		shared_ptr<BaseObject> run(vector<shared_ptr<AST::BaseNode>> args, map<string, shared_ptr<AST::BaseNode>> kwargs, Interpreter* parent) override;
	};
}
