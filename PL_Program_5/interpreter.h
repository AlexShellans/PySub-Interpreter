#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <vector>
#include <map>


class Interpreter
{
private:

	// Member variable for keeping track of whether the expression in a conditional or while
	// header is true
	bool trueStmt = false;

public:

	// interpret() function definition; returns a boolean value indicating whether or not
	// the line could be interpreted without any errors
	bool interpret(expEvaluator*, std::vector<std::pair<std::string, categoryType>>, bool*, bool*);
};

#endif