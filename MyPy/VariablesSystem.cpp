#include "VariablesSystem.h"
#include "InterpreterError.h"


VariablesSystem::VariablesSystem()
{
	// set all builtis-functions
	set("print", allocNew<BuiltinFunctions::Print>());
	set("input", allocNew<BuiltinFunctions::Input>());
}
VariablesSystem::~VariablesSystem()
{
}

void VariablesSystem::set(string name, shared_ptr<BaseObject> obj)
{
	_variables[name] = obj;
}

shared_ptr<BaseObject> VariablesSystem::get(string name)
{
	if (_variables.count(name))
		return _variables[name];

	throw NameError("name '" + name + "' is not defined");
}
