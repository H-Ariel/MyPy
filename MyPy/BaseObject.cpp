#include "BaseObject.h"
#include "InterpreterError.h"
#include "Interpreter.h"


#define THROW_NOT_IMPLETED_OPERATOR(op)			throw TypeError("unsupported operand type(s) for " op ": '" + getTypeName(type) + "' and '" + getTypeName(b->type) + "'")
#define THROW_NOT_IMPLETED_UNARY_OPERATOR(op)	throw TypeError("bad operand type for unary " op ": '" + getTypeName(type) + "'")


// TODO: move to other place
bool str_to_bool(string str)
{
	/*if (str == "True") return true;
	if (str == "False") return false;
	throw InterpreterError("not bool: " + str);*/
	return str == "True";
}


shared_ptr<BaseObject> BaseObject::getObject(AST::BaseNode* node, Interpreter* interpreter)
{
	switch (node->value.type)
	{
	case Token::Types::None:		return allocNew<Obj_None>();
	case Token::Types::FloatNumber:	return allocNew<Obj_FloatNumber>(stod(node->value.value));
	case Token::Types::IntNumber:	return allocNew<Obj_IntNumber>(stoll(node->value.value));
	case Token::Types::String:		return allocNew<Obj_String>(node->value.value);
	case Token::Types::Boolean:		return allocNew<Obj_Boolean>(str_to_bool(node->value.value));
	case Token::Types::List:		return allocNew<Obj_List>(((AST::List*)node)->elements, interpreter);
	}
	throw InterpreterError(__FUNCTION__ " - not impleted");
}
string BaseObject::getTypeName(Types type)
{
	switch (type)
	{
	case BaseObject::Types::None:				return "NoneType";
	case BaseObject::Types::FloatNumber:		return "float";
	case BaseObject::Types::IntNumber:			return "int";
	case BaseObject::Types::Boolean:			return "bool";
	case BaseObject::Types::String:				return "str";
	case BaseObject::Types::List:				return "list";
	case BaseObject::Types::BuiltinFunction:	return "builtin_function_or_method";
	case BaseObject::Types::UserFunction:		return "function";
	}

	throw TypeError("not a type: " + to_string((int)type));
}


BaseObject::BaseObject(Types type)
	: type(type)
{
}
string BaseObject::toString() { THROW_NOT_IMPLETED; }
shared_ptr<Obj_Boolean> BaseObject::toBoolean() { THROW_NOT_IMPLETED; }
shared_ptr<BaseObject> BaseObject::add					(BaseObject* b) { THROW_NOT_IMPLETED_OPERATOR("+"); }
shared_ptr<BaseObject> BaseObject::sub					(BaseObject* b) { THROW_NOT_IMPLETED_OPERATOR("-"); }
shared_ptr<BaseObject> BaseObject::mul					(BaseObject* b) { THROW_NOT_IMPLETED_OPERATOR("*"); }
shared_ptr<BaseObject> BaseObject::fltDiv				(BaseObject* b) { THROW_NOT_IMPLETED_OPERATOR("/"); }
shared_ptr<BaseObject> BaseObject::flrDiv				(BaseObject* b) { THROW_NOT_IMPLETED_OPERATOR("//"); }
shared_ptr<BaseObject> BaseObject::mod					(BaseObject* b) { THROW_NOT_IMPLETED_OPERATOR("%"); }
shared_ptr<BaseObject> BaseObject::pow					(BaseObject* b) { THROW_NOT_IMPLETED_OPERATOR("**"); }
shared_ptr<BaseObject> BaseObject::leftShift			(BaseObject* b) { THROW_NOT_IMPLETED_OPERATOR("<<"); }
shared_ptr<BaseObject> BaseObject::rightShift			(BaseObject* b) { THROW_NOT_IMPLETED_OPERATOR(">>"); }
shared_ptr<BaseObject> BaseObject::bitwiseAnd			(BaseObject* b) { THROW_NOT_IMPLETED_OPERATOR("&"); }
shared_ptr<BaseObject> BaseObject::bitwiseOr			(BaseObject* b) { THROW_NOT_IMPLETED_OPERATOR("|"); }
shared_ptr<BaseObject> BaseObject::bitwiseXor			(BaseObject* b) { THROW_NOT_IMPLETED_OPERATOR("^"); }
shared_ptr<BaseObject> BaseObject::greaterThan			(BaseObject* b) { THROW_NOT_IMPLETED_OPERATOR(">"); }
shared_ptr<BaseObject> BaseObject::lessThan				(BaseObject* b) { return b->greaterThan(this); }
shared_ptr<BaseObject> BaseObject::greaterThanOrEqualTo	(BaseObject* b) { return greaterThan(b)->logicalOr(equalTo(b).get()); }
shared_ptr<BaseObject> BaseObject::lessThanOrEqualTo	(BaseObject* b) { return lessThan(b)->logicalOr(equalTo(b).get()); }
shared_ptr<BaseObject> BaseObject::equalTo				(BaseObject* b) { return allocNew<Obj_Boolean>(false); }
shared_ptr<BaseObject> BaseObject::notEqualTo			(BaseObject* b) { return equalTo(b)->logicalNot(); }
shared_ptr<BaseObject> BaseObject::logicalAnd			(BaseObject* b) { return allocNew<Obj_Boolean>(toBoolean()->val && b->toBoolean()->val); }
shared_ptr<BaseObject> BaseObject::logicalOr			(BaseObject* b) { return allocNew<Obj_Boolean>(toBoolean()->val || b->toBoolean()->val); }
shared_ptr<BaseObject> BaseObject::positive()	{ THROW_NOT_IMPLETED_UNARY_OPERATOR("+"); }
shared_ptr<BaseObject> BaseObject::negative()	{ THROW_NOT_IMPLETED_UNARY_OPERATOR("-"); }
shared_ptr<BaseObject> BaseObject::bitwiseNot()	{ THROW_NOT_IMPLETED_UNARY_OPERATOR("~"); }
shared_ptr<BaseObject> BaseObject::logicalNot() { return allocNew<Obj_Boolean>(!toBoolean()->val); }


Obj_None::Obj_None() : BaseObject(Types::None) {}
string Obj_None::toString() { return "None"; }
shared_ptr<Obj_Boolean> Obj_None::toBoolean() { return allocNew<Obj_Boolean>(false); }


Obj_FloatNumber::Obj_FloatNumber(FloatType n)
	: BaseObject(Types::FloatNumber), val(n)
{
}
string Obj_FloatNumber::toString()
{
	return to_string(val);
	// TODO: improve
}
shared_ptr<Obj_Boolean> Obj_FloatNumber::toBoolean()
{
	return allocNew<Obj_Boolean>(val);
}
shared_ptr<BaseObject> Obj_FloatNumber::add(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber:	return allocNew<Obj_FloatNumber>(val + ((Obj_FloatNumber*)b)->val);
	case Types::Boolean:
	case Types::IntNumber:		return allocNew<Obj_FloatNumber>(val + ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::add(b);
}
shared_ptr<BaseObject> Obj_FloatNumber::sub(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber:	return allocNew<Obj_FloatNumber>(val - ((Obj_FloatNumber*)b)->val);
	case Types::Boolean:
	case Types::IntNumber:		return allocNew<Obj_FloatNumber>(val - ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::sub(b);
}
shared_ptr<BaseObject> Obj_FloatNumber::mul(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber:	return allocNew<Obj_FloatNumber>(val * ((Obj_FloatNumber*)b)->val);
	case Types::Boolean:
	case Types::IntNumber:		return allocNew<Obj_FloatNumber>(val * ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::mul(b);
}
shared_ptr<BaseObject> Obj_FloatNumber::fltDiv(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber: {
		FloatType bn = ((Obj_FloatNumber*)b)->val;
		if (bn == 0) throw ZeroDivisionError();
		return allocNew<Obj_FloatNumber>(val / bn);
	}
	case Types::Boolean:
	case Types::IntNumber: {
		IntType bn = ((Obj_IntNumber*)b)->val;
		if (bn == 0) throw ZeroDivisionError();
		return allocNew<Obj_FloatNumber>(val / bn);
	}
	}

	return BaseObject::fltDiv(b);
}
shared_ptr<BaseObject> Obj_FloatNumber::flrDiv(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber: {
		FloatType bn = ((Obj_FloatNumber*)b)->val;
		if (bn == 0) throw ZeroDivisionError();
		return allocNew<Obj_IntNumber>(IntType(val / bn));
	}
	case Types::Boolean:
	case Types::IntNumber: {
		IntType bn = ((Obj_IntNumber*)b)->val;
		if (bn == 0) throw ZeroDivisionError();
		return allocNew<Obj_IntNumber>(IntType(val / bn));
	}
	}

	return BaseObject::flrDiv(b);
}
shared_ptr<BaseObject> Obj_FloatNumber::mod(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber:	return allocNew<Obj_FloatNumber>(fmod(val, ((Obj_FloatNumber*)b)->val));
	case Types::Boolean:
	case Types::IntNumber:		return allocNew<Obj_FloatNumber>(fmod(val, (FloatType)((Obj_IntNumber*)b)->val));
	}

	return BaseObject::mod(b);
}
shared_ptr<BaseObject> Obj_FloatNumber::pow(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber:	return allocNew<Obj_FloatNumber>(::pow(val, ((Obj_FloatNumber*)b)->val));
	case Types::Boolean:
	case Types::IntNumber:		return allocNew<Obj_FloatNumber>(::pow(val, (FloatType)((Obj_IntNumber*)b)->val));
	}

	return BaseObject::pow(b);
}
shared_ptr<BaseObject> Obj_FloatNumber::greaterThan(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber:	return allocNew<Obj_Boolean>(val > ((Obj_FloatNumber*)b)->val);
	case Types::Boolean:
	case Types::IntNumber:		return allocNew<Obj_Boolean>(val > ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::greaterThan(b);
}
shared_ptr<BaseObject> Obj_FloatNumber::equalTo(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber:	return allocNew<Obj_Boolean>(val == ((Obj_FloatNumber*)b)->val);
	case Types::Boolean:
	case Types::IntNumber:		return allocNew<Obj_Boolean>(val == ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::equalTo(b);
}
shared_ptr<BaseObject> Obj_FloatNumber::positive() { return allocNew<Obj_FloatNumber>(+val); }
shared_ptr<BaseObject> Obj_FloatNumber::negative() { return allocNew<Obj_FloatNumber>(-val); }


Obj_IntNumber::Obj_IntNumber(IntType n)
	: BaseObject(Types::IntNumber), val(n)
{
}
Obj_IntNumber::Obj_IntNumber(bool n)
	: BaseObject(Types::Boolean), val(n)
{
}
string Obj_IntNumber::toString()
{
	return to_string(val);
}
shared_ptr<Obj_Boolean> Obj_IntNumber::toBoolean()
{
	return allocNew<Obj_Boolean>(val);
}
shared_ptr<BaseObject> Obj_IntNumber::add(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber:	return allocNew<Obj_FloatNumber>(val + ((Obj_FloatNumber*)b)->val);
	case Types::Boolean:
	case Types::IntNumber:		return allocNew<Obj_IntNumber>(val + ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::add(b);
}
shared_ptr<BaseObject> Obj_IntNumber::sub(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber:	return allocNew<Obj_FloatNumber>(val - ((Obj_FloatNumber*)b)->val);
	case Types::Boolean:
	case Types::IntNumber:		return allocNew<Obj_IntNumber>(val - ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::sub(b);
}
shared_ptr<BaseObject> Obj_IntNumber::mul(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber:	return allocNew<Obj_FloatNumber>(val * ((Obj_FloatNumber*)b)->val);
	case Types::Boolean:
	case Types::IntNumber:		return allocNew<Obj_IntNumber>(val * ((Obj_IntNumber*)b)->val);
	case Types::String:			return b->mul(allocNew<Obj_IntNumber>(val).get());
	case Types::List:			return b->mul(allocNew<Obj_IntNumber>(val).get());
	}

	return BaseObject::mul(b);
}
shared_ptr<BaseObject> Obj_IntNumber::fltDiv(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber: {
		FloatType bn = ((Obj_FloatNumber*)b)->val;
		if (bn == 0) throw ZeroDivisionError();
		return allocNew<Obj_FloatNumber>(val / bn);
	}
	case Types::Boolean:
	case Types::IntNumber: {
		IntType bn = ((Obj_IntNumber*)b)->val;
		if (bn == 0) throw ZeroDivisionError();
		return allocNew<Obj_FloatNumber>((FloatType)val / bn);
	}
	}

	return BaseObject::fltDiv(b);
}
shared_ptr<BaseObject> Obj_IntNumber::flrDiv(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber: {
		FloatType bn = ((Obj_FloatNumber*)b)->val;
		if (bn == 0) throw ZeroDivisionError();
		return allocNew<Obj_IntNumber>(IntType(val / bn));
	}
	case Types::Boolean:
	case Types::IntNumber: {
		IntType bn = ((Obj_IntNumber*)b)->val;
		if (bn == 0) throw ZeroDivisionError();
		return allocNew<Obj_IntNumber>(IntType(val / bn));
	}
	}

	return BaseObject::flrDiv(b);
}
shared_ptr<BaseObject> Obj_IntNumber::mod(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber:	return allocNew<Obj_FloatNumber>(fmod((FloatType)val, ((Obj_FloatNumber*)b)->val));
	case Types::Boolean:
	case Types::IntNumber:		return allocNew<Obj_IntNumber>(val % ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::mod(b);
}
shared_ptr<BaseObject> Obj_IntNumber::pow(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber: return allocNew<Obj_FloatNumber>(::pow(val, ((Obj_FloatNumber*)b)->val));
	case Types::Boolean:
	case Types::IntNumber: {
		IntType bn = ((Obj_IntNumber*)b)->val;
		if (bn < 0)
			return allocNew<Obj_FloatNumber>((FloatType)::pow(val, bn));
		return allocNew<Obj_IntNumber>((IntType)::pow(val, bn));
	}
	}

	return BaseObject::pow(b);
}
shared_ptr<BaseObject> Obj_IntNumber::leftShift(BaseObject* b)
{
	switch (b->type)
	{
	case Types::Boolean:
	case Types::IntNumber: return allocNew<Obj_IntNumber>(val << ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::leftShift(b);
}
shared_ptr<BaseObject> Obj_IntNumber::rightShift(BaseObject* b)
{
	switch (b->type)
	{
	case Types::Boolean:
	case Types::IntNumber: return allocNew<Obj_IntNumber>(val >> ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::rightShift(b);
}
shared_ptr<BaseObject> Obj_IntNumber::bitwiseAnd(BaseObject* b)
{
	switch (b->type)
	{
	case Types::Boolean:
	case Types::IntNumber: return allocNew<Obj_IntNumber>(val & ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::bitwiseAnd(b);
}
shared_ptr<BaseObject> Obj_IntNumber::bitwiseOr(BaseObject* b)
{
	switch (b->type)
	{
	case Types::Boolean:
	case Types::IntNumber: return allocNew<Obj_IntNumber>(val | ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::bitwiseOr(b);
}
shared_ptr<BaseObject> Obj_IntNumber::bitwiseXor(BaseObject* b)
{
	switch (b->type)
	{
	case Types::Boolean:
	case Types::IntNumber: return allocNew<Obj_IntNumber>(val ^ ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::bitwiseXor(b);
}
shared_ptr<BaseObject> Obj_IntNumber::greaterThan(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber:	return allocNew<Obj_Boolean>(val > ((Obj_FloatNumber*)b)->val);
	case Types::Boolean:
	case Types::IntNumber:		return allocNew<Obj_Boolean>(val > ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::greaterThan(b);
}
shared_ptr<BaseObject> Obj_IntNumber::equalTo(BaseObject* b)
{
	switch (b->type)
	{
	case Types::FloatNumber:	return allocNew<Obj_Boolean>(val == ((Obj_FloatNumber*)b)->val);
	case Types::Boolean:
	case Types::IntNumber:		return allocNew<Obj_Boolean>(val == ((Obj_IntNumber*)b)->val);
	}

	return BaseObject::equalTo(b);
}
shared_ptr<BaseObject> Obj_IntNumber::positive()	{ return allocNew<Obj_IntNumber>(+val); }
shared_ptr<BaseObject> Obj_IntNumber::negative()	{ return allocNew<Obj_IntNumber>(-val); }
shared_ptr<BaseObject> Obj_IntNumber::bitwiseNot()	{ return allocNew<Obj_IntNumber>(~val); }


Obj_Boolean::Obj_Boolean(bool b)
	: Obj_IntNumber(b)
{
}
string Obj_Boolean::toString()
{
	return val ? "True" : "False";
}
shared_ptr<BaseObject> Obj_Boolean::logicalNot() { return allocNew<Obj_Boolean>(!val); }


template <class T>
Sequence<T>::Sequence(Types type)
	: BaseObject(type)
{
}
template <class T>
shared_ptr<Obj_Boolean> Sequence<T>::toBoolean()
{
	return allocNew<Obj_Boolean>(!_elements.empty());
}
template <class T>
shared_ptr<BaseObject> Sequence<T>::add(BaseObject* b)
{
	if (type != b->type)
	{
		return BaseObject::add(b);
	}

	shared_ptr<BaseObject> retVal;

	if (typeid(T) == typeid(char))
	{
		retVal = allocNew<Obj_String>("");
	}
	else if (typeid(T) == typeid(shared_ptr<BaseObject>))
	{
		retVal = allocNew<Obj_List>();
	}
	else
	{
		return BaseObject::add(b);
	}

	Sequence<T>* newSeq = (Sequence<T>*)retVal.get();

	for (auto& e : _elements)
	{
		newSeq->_elements.push_back(e);
	}
	for (auto& e : ((Sequence<T>*)b)->_elements)
	{
		newSeq->_elements.push_back(e);
	}

	return retVal;
}
template <class T>
shared_ptr<BaseObject> Sequence<T>::mul(BaseObject* b)
{
	if (b->type != Types::IntNumber)
	{
		return BaseObject::mul(b);
	}

	shared_ptr<BaseObject> retVal;

	if (typeid(T) == typeid(char))
	{
		retVal = allocNew<Obj_String>("");
	}
	else if (typeid(T) == typeid(shared_ptr<BaseObject>))
	{
		retVal = allocNew<Obj_List>();
	}
	else
	{
		return BaseObject::mul(b);
	}

	Sequence<T>* newSeq = (Sequence<T>*)retVal.get();
	IntType n = ((Obj_IntNumber*)b)->val;

	for (IntType i = 0; i < n; i++)
	{
		for (auto& e : _elements)
		{
			newSeq->_elements.push_back(e);
		}
	}

	return retVal;
}
template <class T>
shared_ptr<BaseObject> Sequence<T>::greaterThan(BaseObject* b)
{
	if (type != b->type) return BaseObject::greaterThan(b);

	Sequence<T>* bSeq = (Sequence<T>*)b;

	size_t len = min(_elements.size(), bSeq->_elements.size());
	for (size_t i = 0; i < len ;i++)
	{
		if (isGreater(_elements[i], bSeq->_elements[i]))
		{
			return allocNew<Obj_Boolean>(true);
		}
		if (isGreater(bSeq->_elements[i], _elements[i]))
		{
			return allocNew<Obj_Boolean>(false);
		}
		// if the elements are equal continue check the others
	}

	// if they have same elements compare by their size
	return allocNew<Obj_Boolean>(_elements.size() > bSeq->_elements.size());
}
template <class T>
shared_ptr<BaseObject> Sequence<T>::equalTo(BaseObject* b)
{
	if (type != b->type) return BaseObject::equalTo(b);

	Sequence<T>* bSeq = (Sequence<T>*)b;
	bool retVal = false;

	if (_elements.size() == bSeq->_elements.size())
	{
		retVal = true;
		size_t len = _elements.size();
		for (size_t i = 0; i < len && retVal == true; i++)
		{
			if (!areEqual(_elements[i], bSeq->_elements[i]))
			{
				retVal = false;
			}
		}
	}

	return allocNew<Obj_Boolean>(retVal);
}


Obj_String::Obj_String(string str)
	: Sequence(Types::String)
{
	for (auto& c : str)
	{
		_elements.push_back(c);
	}
}
string Obj_String::toString()
{
	string val;
	for (char& c : _elements)
	{
		val += c;
	}

	const bool has1 = FindInArray(_elements, '\''), has2 = FindInArray(_elements, '"');

	if (!has2 && has1) return '"' + val + '"';
	if (!has1 && has2) return '\'' + val + '\'';

	string res;
	for (char c : val)
	{
		if (c == '\'')
		{
			res += "\\";
		}
		res += c;
	}

	return "'" + res + "'";
}
bool Obj_String::areEqual(char a, char b) { return a == b; }
bool Obj_String::isGreater(char a, char b) { return a > b; }


Obj_List::Obj_List()
	: Sequence(Types::List)
{
}
Obj_List::Obj_List(vector<shared_ptr<AST::BaseNode>> elements, Interpreter* interpreter)
	: Sequence(Types::List)
{
	for (shared_ptr<AST::BaseNode>& e : elements)
	{
		_elements.push_back(interpreter->interpret(e));
	}
}
string Obj_List::toString()
{
	string str;

	size_t len = _elements.size();
	if (len > 0)
	{
		size_t i;
		for (i = 0; i < len - 1; i++)
		{
			str += _elements[i]->toString() + ", ";
		}
		str += _elements[i]->toString();
	}

	return '[' + str + ']';
}
bool Obj_List::areEqual(shared_ptr<BaseObject> a, shared_ptr<BaseObject> b) { return a->equalTo(b.get())->toBoolean()->val; }
bool Obj_List::isGreater(shared_ptr<BaseObject> a, shared_ptr<BaseObject> b) { return a->greaterThan(b.get())->toBoolean()->val; }


BaseFunction::BaseFunction(Types type, string funcName)
	: BaseObject(type), funcName(funcName)
{
}
shared_ptr<Obj_Boolean> BaseFunction::toBoolean() { return allocNew<Obj_Boolean>(true); }


Function::Function(string funcName, vector<string> argsNames, vector<shared_ptr<AST::BaseNode>> commands)
	: BaseFunction(Types::UserFunction, funcName), argsNames(argsNames), commands(commands)
{
}
string Function::toString()
{
	ostringstream ostr;
	ostr << "<function " << funcName << " at 0x" << this << ">";
	return ostr.str();
}
shared_ptr<BaseObject> Function::run(vector<shared_ptr<AST::BaseNode>> args, map<string, shared_ptr<AST::BaseNode>> kwargs, Interpreter* parent)
{
	shared_ptr<BaseObject> retVal; // TODO

	if (argsNames.size() > args.size()) throw TypeError("missing arguments");
	if (argsNames.size() < args.size()) throw TypeError("too many arguments");

	VariablesSystem funcVariables;

	for (size_t i = 0; i < argsNames.size(); i++)
	{
		funcVariables.set(argsNames[i], parent->interpret(args[i]));
	}
	for (auto& i : kwargs)
	{
		funcVariables.set(i.first, parent->interpret(i.second));
	}

	Interpreter funcInterpreter(funcVariables);

	for (auto& cmd : commands)
	{
		funcInterpreter.interpret(cmd);
	}

	return retVal;
}


BuiltinFunction::BuiltinFunction(string funcName)
	: BaseFunction(Types::BuiltinFunction, funcName)
{
}
string BuiltinFunction::toString() { return "<built-in function " + funcName + ">"; }


namespace BuiltinFunctions
{
	Print::Print() : BuiltinFunction("print") { }
	shared_ptr<BaseObject> Print::run(vector<shared_ptr<AST::BaseNode>> args, map<string, shared_ptr<AST::BaseNode>> kwargs, Interpreter* parent)
	{
		string end = "\n", sep = " ";

		if (kwargs.count("end")) end = parent->interpret(kwargs["end"])->toString();
		if (kwargs.count("sep")) sep = parent->interpret(kwargs["sep"])->toString();

		vector<string> argsStrings;
		for (auto& a : args)
		{
			argsStrings.push_back(parent->interpret(a)->toString());
		}

		cout << concatenate(argsStrings, sep) << end;


		return shared_ptr<BaseObject>();
	}

	Input::Input() : BuiltinFunction("input") { }
	shared_ptr<BaseObject> Input::run(vector<shared_ptr<AST::BaseNode>> args, map<string, shared_ptr<AST::BaseNode>> kwargs, Interpreter* parent)
	{
		if (kwargs.size()) throw TypeError("input() takes no keyword arguments");
		if (args.size() > 1) throw TypeError("too many arguments");

		if (args.size()) cout << args[0];
		string text;
		getline(cin, text);
		return allocNew<Obj_String>(text);
	}
}
