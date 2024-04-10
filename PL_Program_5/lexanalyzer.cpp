#include <iostream>
#include <string>
#include <vector>
#include "interface.h"
#include "lexanalyzer.h"
#include "expevaluator.h"
#include "interpreter.h"

using namespace std;

// Function to generate token information
void LexicalAnalyzer::generatePair(vector<string> code)
{
	// Clear previously-stored token information if any exists
	tokenInfo.clear();
	
	// Outer for-loop: iterates over entire file read into programCode line by line
	for (string line:code)
	{
		tokenLineType myLine;
		
		// Inner for-loop: iterates over each line in programCode character by character
		for (int i = 0; i < line.size(); i++)
		{
			pairType myPair;
			
			// Variables for keeping track of current character, next character, and next index to read from
			int ni = i + 1;
			char c = line[i];
			char nc = line[ni];

			// Series of if/else if statements to evaluate current character/set of characters,
			// which is then created as a pair with its corresponding token category type

			// Checks if a space or tab exists at the beginning of the line
			if (i == 0 && (c == '\t' || c == ' '))
			{
				string item = "";
				item += c;

				while (isspace(nc))
				{
					item += nc;
					ni++;
					nc = line[ni];
				}

				myPair = make_pair(item, categoryType::INDENT);
			}
			// Checks if c is a digit
			else if (isdigit(c))
			{
				string item = "";
				item += c;

				while (isdigit(nc))
				{
					item += nc;
					ni++;
					nc = line[ni];
				}

				myPair = make_pair(item, categoryType::NUMERIC_LITERAL);
			}
			// Checks if c is an alphabetic character
			else if (isalpha(c))
			{
				string item = "";
				item += c;

				while (isalpha(nc))
				{
					item += nc;
					ni++;
					nc = line[ni];
				}

				// Assigns alphabetic item to correct category type
				if (item == "print" || item == "if" || item == "elif" || item == "else" ||
					item == "while" || item == "int" || item == "input")
				{
					myPair = make_pair(item, categoryType::KEYWORD);
				}
				else if (item == "and" || item == "or" || item == "not")
				{
					myPair = make_pair(item, categoryType::LOGICAL_OP);
				}
				else
				{
					myPair = make_pair(item, categoryType::IDENTIFIER);
				}
			}
			// Checks if c is an opening quote
			else if (c == '"' || c == '\'')
			{
				string item = "";
				
				while (nc != '"' && nc != '\'')
				{
					item += nc;
					ni++;
					if (line[ni])
					{
						nc = line[ni];
					}
				}
				
				myPair = make_pair(item, categoryType::STRING_LITERAL);
				ni++;
			}
			// Checks if c is an opening parenthesis
			else if (c == '(')
			{
				myPair = make_pair(c, categoryType::LEFT_PAREN);
			}
			// Checks if c is an closing parenthesis
			else if (c == ')')
			{
				myPair = make_pair(c, categoryType::RIGHT_PAREN);
			}
			// Checks if c is a colon
			else if (c == ':')
			{
				myPair = make_pair(c, categoryType::COLON);
			}
			// Checks if c is a comma
			else if (c == ',')
			{
				myPair = make_pair(c, categoryType::COMMA);
			}
			// Checks if c is the beginning of a comment
			else if (c == '#')
			{
				string substr = "";
				substr += c;

				while (nc)
				{
					substr += nc;
					ni++;
					nc = line[ni];
				}

				myPair = make_pair(substr, categoryType::COMMENT);
				myLine.push_back(myPair);
				break;
			}
			// Checks if c is a whitespace
			else if (c == ' ' || c == '\t')
			{
				c++;
				continue;
			}
			// Checks if c is an arithmetic operater
			else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
			{
				myPair = make_pair(c, categoryType::ARITH_OP);
			}
			// Checks if c is a relational operator
			else if ((c == '=' && nc == '=') || (c == '<' && nc == '=')
				|| (c == '>' && nc == '=') || (c == '!' && nc == '='))
			{
				string item = "";
				item = item + c + nc;
				myPair = make_pair(item, categoryType::RELATIONAL_OP);
				ni++;
			}
			else if (c == '<' || c == '>')
			{
				myPair = make_pair(c, categoryType::RELATIONAL_OP);
			}
			// Checks if c is an assignment operator
			else if (c == '=')
			{
				myPair = make_pair(c, categoryType::ASSIGNMENT_OP);
			}
			// If c cannot be identified, classify it as unknown
			else
			{
				myPair = make_pair(c, categoryType::UNKNOWN);
			}
			
			// Prevents characters from being skipped over due to the i++ step in for-loop
			i = ni-1;

			// Push back each pair onto current line
			myLine.push_back(myPair);
		}
		
		// Push back each line onto tokenInfo
		tokenInfo.push_back(myLine);
	}
}

// Function to display token information
void LexicalAnalyzer::displayLAData()
{
	cout << "***** TOKEN INFORMATION *****" << endl;

	// For-loop to display each line
	for (int i = 0; i < tokenInfo.size(); i++)
	{
		cout << "Line #" << i << ":" << endl;

		if (tokenInfo[i].empty())
		{
			cout << endl;
		}

		// For-loop to display each pair type
		for (int j = 0; j < tokenInfo[i].size(); j++)
		{
			cout << "Token[" << j << "]: ";
			cout << tokenInfo[i][j].first << " - " << enumToStr(tokenInfo[i][j].second) << endl;
		}

		cout << "-----------------------------------------------------------" << endl;
	}

	cout << endl;
}

LexicalAnalyzer::tokenLineType LexicalAnalyzer::getLAData(int idx)
{
	return tokenInfo[idx];
}

// Function to clear tokenInfo when called from user interface
void LexicalAnalyzer::clear()
{
	tokenInfo.clear();
}

unsigned long LexicalAnalyzer::size()
{
	return tokenInfo.size();
}