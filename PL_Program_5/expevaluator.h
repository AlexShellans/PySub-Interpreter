#ifndef EXPEVALUATOR_H
#define EXPEVALUATOR_H

#include <string>
#include <vector>
#include <map>


class expEvaluator
{
private:
	typedef std::map<std::string, std::string> symbolTableType;
	symbolTableType symbolTable;

public:
	// Function declarations
	std::vector<std::pair<std::string, categoryType>> inToPost(std::vector<std::pair<std::string, categoryType>>);
	int postEval(std::vector<std::pair<std::string, categoryType>>);
	void storeVars(std::string, std::string);
	std::string findVars(std::string);
	int precedence(std::string);
	void displayVars();
	void clear();
};

#endif