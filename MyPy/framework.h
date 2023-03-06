#pragma once

#define _CRT_SECURE_NO_WARNINGS

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DBG_NEW new
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <memory>
#include <stack>
#include <vector>
#include <functional>
#include <algorithm>

using namespace std;


// get list of all commands in `code`
vector<string> getCommands(string code);

// replace all sub-strings
// from: https://stackoverflow.com/questions/4643512/replace-substring-with-another-substring-c
void replaceAll(string& s, const string& search, const string& replace);

// concatenate all string and split them by `sep`
string concatenate(vector<string> strings, string sep);

bool startsWith(string str, string prefix);
bool endsWith(string str, string suffix);


// allocate new object as `shared_ptr`
template <class T, class ... Args>
shared_ptr<T> allocNew(Args... args)
{
	return shared_ptr<T>(DBG_NEW T(args...));
}

// check if `arr` contains `val`
template <class ArrT, class ValT>
inline bool FindInArray(ArrT arr, ValT val)
{
	auto arrEnd = end(arr);
	return find(begin(arr), arrEnd, val) != arrEnd;
}
