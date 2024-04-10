#ifndef LEXANALYZER_H
#define LEXANALYZER_H

#include <string>
#include <vector>
#include <map>


// Enum class definition with token category types
enum class categoryType
{
	KEYWORD,
	IDENTIFIER,
	STRING_LITERAL,
	NUMERIC_LITERAL,
	ASSIGNMENT_OP,
	ARITH_OP,
	LOGICAL_OP,
	RELATIONAL_OP,
	LEFT_PAREN,
	RIGHT_PAREN,
	COLON,
	COMMA,
	COMMENT,
	INDENT,
	UNKNOWN
};

class LexicalAnalyzer
{
private:
	typedef std::vector<std::vector<std::pair<std::string, categoryType>>> tokenType;
	typedef std::vector<std::pair<std::string, categoryType>> tokenLineType;
	typedef std::pair<std::string, categoryType> pairType;

	tokenType tokenInfo;

	// Function to convert enums to strings for displaying token information
	std::string enumToStr(categoryType c) {
		switch (c) {
		case categoryType::KEYWORD:
			return "KEYWORD";
		case categoryType::IDENTIFIER:
			return "IDENTIFIER";
		case categoryType::STRING_LITERAL:
			return "STRING_LITERAL";
		case categoryType::NUMERIC_LITERAL:
			return "NUMERIC_LITERAL";
		case categoryType::ASSIGNMENT_OP:
			return "ASSIGNMENT_OP";
		case categoryType::ARITH_OP:
			return "ARITH_OP";
		case categoryType::LOGICAL_OP:
			return "LOGICAL_OP";
		case categoryType::RELATIONAL_OP:
			return "RELATIONAL_OP";
		case categoryType::LEFT_PAREN:
			return "LEFT_PAREN";
		case categoryType::RIGHT_PAREN:
			return "RIGHT_PAREN";
		case categoryType::COLON:
			return "COLON";
		case categoryType::COMMA:
			return "COMMA";
		case categoryType::COMMENT:
			return "COMMENT";
		case categoryType::INDENT:
			return "INDENT";
		case categoryType::UNKNOWN:
			return "UNKNOWN";
		}
	}

public:
	void generatePair(std::vector<std::string>);
	void displayLAData();
	tokenLineType getLAData(int = 0);
	void clear();
	unsigned long size();
};

#endif