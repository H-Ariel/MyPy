#include "framework.h"


vector<string> getCommands(string code)
{
	string delim = "\n";
	vector<string> splitedString;
	size_t start = 0, end = code.find(delim);
	while (end != std::string::npos)
	{
		splitedString.push_back(code.substr(start, end - start));
		start = end + delim.size();
		end = code.find(delim, start);
	}
	splitedString.push_back(code.substr(start, end - start));
	// delete empty-strings and spaces-string
	splitedString.resize(remove_if(splitedString.begin(), splitedString.end(),
		[](string s) { return s.empty() || all_of(s.begin(), s.end(), isspace); }) - splitedString.begin());
	return splitedString;
}

void replaceAll(string& s, const string& search, const string& replace)
{
	for (size_t pos = 0; ; pos += replace.length())
	{
		pos = s.find(search, pos);
		if (pos == string::npos) break;
		s.erase(pos, search.length());
		s.insert(pos, replace);
	}
}

string concatenate(vector<string> strings, string sep)
{
	string retStr;
	for (const string& str : strings)
	{
		retStr += str + sep;
	}
	return retStr.substr(0, retStr.length() - sep.length()); // remove last `sep`
}

bool startsWith(string str, string prefix)
{
	return str.rfind(prefix) != string::npos;
}
bool endsWith(string str, string suffix)
{
	size_t str_len = str.length(), suffix_len = suffix.length();
	return str_len >= suffix_len && str.compare(str_len - suffix_len, suffix_len, suffix) == 0;
}
