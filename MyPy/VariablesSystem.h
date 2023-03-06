#pragma once

#include "BaseObject.h"


class VariablesSystem
{
public:
	VariablesSystem();
	~VariablesSystem();

	void set(string name, shared_ptr<BaseObject> obj);
	shared_ptr<BaseObject> get(string name);

private:
	map<string, shared_ptr<BaseObject>> _variables;
};
