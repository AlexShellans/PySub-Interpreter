#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include "interface.h"
#include "lexanalyzer.h"
#include "expevaluator.h"
#include "interpreter.h"

using namespace std;

// Function definition for infix to postfix conversion
std::vector<std::pair<std::string, categoryType>> expEvaluator::inToPost(std::vector<std::pair<std::string, categoryType>> tokens)
{
	// Create an empty stack to hold expression elements and an empty vector to hold converted expression
	std::stack<std::pair<std::string, categoryType>> postfixStack;
	std::vector<std::pair<std::string, categoryType>> postfixVect;

	// Read through expression, one element at a time
	for (int i = 0; i < tokens.size(); i++)
	{
		// Determine whether each element is an operand, left or right parenthesis, or operator
		if (tokens[i].second == categoryType::NUMERIC_LITERAL)
		{
			postfixVect.push_back(tokens[i]);
		}
		else if (tokens[i].second == categoryType::IDENTIFIER)
		{
			string value = expEvaluator::findVars(tokens[i].first);

			if (value == "")
			{
				std::vector<std::pair<std::string, categoryType>> emptyVect;
				return emptyVect;
			}

			std::pair<std::string, categoryType> idPair = make_pair(value, categoryType::NUMERIC_LITERAL);
			postfixVect.push_back(idPair);
		}
		else if (tokens[i].second == categoryType::LEFT_PAREN)
		{
			postfixStack.push(tokens[i]);
		}
		else if (tokens[i].second == categoryType::RIGHT_PAREN)
		{
			while (!postfixStack.empty() && (postfixStack.top()).second != categoryType::LEFT_PAREN)
			{
				postfixVect.push_back(postfixStack.top());
				postfixStack.pop();
			}

			if (postfixStack.empty())
			{
				cout << "Delimiters are unbalanced; please enter a valid expression." << endl;
				std::vector<std::pair<std::string, categoryType>> emptyVect;
				return emptyVect;
			}
			else
			{
				postfixStack.pop();
			}
			
		}
		else if (tokens[i].second == categoryType::ARITH_OP || tokens[i].second == categoryType::RELATIONAL_OP ||
			tokens[i].second == categoryType::LOGICAL_OP)
		{
			while (!postfixStack.empty() && ((postfixStack.top()).second == categoryType::ARITH_OP)
				&& (expEvaluator::precedence(tokens[i].first) <= expEvaluator::precedence((postfixStack.top()).first)))
			{
				postfixVect.push_back(postfixStack.top());
				postfixStack.pop();
			}

			postfixStack.push(tokens[i]);
		}
	}

	// After all elements have been read from original expression, pop all elements in the stack
	// and push them onto the vector
	while (!postfixStack.empty())
	{
		postfixVect.push_back(postfixStack.top());
		postfixStack.pop();
	}

	// Return vector containing postfix expression
	return postfixVect;
}

// Function definition for postfix evaluation
int expEvaluator::postEval(std::vector<std::pair<std::string, categoryType>> exp)
{
	// Create an empty stack to hold expression elements and variables to hold operands and result
	std::stack<std::pair<std::string, categoryType>> postEvalStack;
	int operand1, operand2, result = 0;

	// Read through expression, one element at a time
	for (int i = 0; i < exp.size(); i++)
	{
		// Determine whether each element is an operand or operator
		if (exp[i].second == categoryType::NUMERIC_LITERAL)
		{
			postEvalStack.push(exp[i]);
		}
		else if (exp[i].second == categoryType::ARITH_OP || exp[i].second == categoryType::RELATIONAL_OP || exp[i].second == categoryType::LOGICAL_OP)
		{
			// Verify that one operand exists on the stack in the case of unary "not" operator
			if (!postEvalStack.empty() && (postEvalStack.top()).second == categoryType::NUMERIC_LITERAL)
			{
				operand2 = stoi((postEvalStack.top()).first);
				postEvalStack.pop();

				if (exp[i].first == "not")
				{
					result = !operand2;
					postEvalStack.push(make_pair(std::to_string(result), categoryType::NUMERIC_LITERAL));
					break;
				}
			}
			else
			{
				cout << "Not enough operands; please enter a valid expression." << endl;
				//return;
			}

			// Verify that two operands exist on the stack and assign them to operand1 and operand2
			if (!postEvalStack.empty() && (postEvalStack.top()).second == categoryType::NUMERIC_LITERAL)
			{
				operand1 = stoi((postEvalStack.top()).first);
				postEvalStack.pop();

				// Evaluate expression
				if (exp[i].first == "*")
				{
					result = operand1 * operand2;
					postEvalStack.push(make_pair(std::to_string(result), categoryType::NUMERIC_LITERAL));
				}
				else if (exp[i].first == "/")
				{
					result = operand1 / operand2;
					postEvalStack.push(make_pair(std::to_string(result), categoryType::NUMERIC_LITERAL));
				}
				else if (exp[i].first == "%")
				{
					result = operand1 % operand2;
					postEvalStack.push(make_pair(std::to_string(result), categoryType::NUMERIC_LITERAL));
				}
				else if (exp[i].first == "+")
				{
					result = operand1 + operand2;
					postEvalStack.push(make_pair(std::to_string(result), categoryType::NUMERIC_LITERAL));
				}
				else if (exp[i].first == "-")
				{
					result = operand1 - operand2;
					postEvalStack.push(make_pair(std::to_string(result), categoryType::NUMERIC_LITERAL));
				}
				else if (exp[i].first == "<")
				{
					result = operand1 < operand2;
					postEvalStack.push(make_pair(std::to_string(result), categoryType::NUMERIC_LITERAL));
				}
				else if (exp[i].first == "<=")
				{
					result = operand1 <= operand2;
					postEvalStack.push(make_pair(std::to_string(result), categoryType::NUMERIC_LITERAL));
				}
				else if (exp[i].first == ">")
				{
					result = operand1 > operand2;
					postEvalStack.push(make_pair(std::to_string(result), categoryType::NUMERIC_LITERAL));
				}
				else if (exp[i].first == ">=")
				{
					result = operand1 >= operand2;
					postEvalStack.push(make_pair(std::to_string(result), categoryType::NUMERIC_LITERAL));
				}
				else if (exp[i].first == "!=")
				{
					result = operand1 != operand2;
					postEvalStack.push(make_pair(std::to_string(result), categoryType::NUMERIC_LITERAL));
				}
				else if (exp[i].first == "==")
				{
					result = operand1 == operand2;
					postEvalStack.push(make_pair(std::to_string(result), categoryType::NUMERIC_LITERAL));
				}
				else if (exp[i].first == "and")
				{
					result = (operand1 && operand2);
					postEvalStack.push(make_pair(std::to_string(result), categoryType::NUMERIC_LITERAL));
				}
				else if (exp[i].first == "or")
				{
					result = (operand1 || operand2);
					postEvalStack.push(make_pair(std::to_string(result), categoryType::NUMERIC_LITERAL));
				}
			}
			else
			{
				cout << "Not enough operands; please enter a valid expression." << endl;
				//return;
			}
		}
	}

	// Return result
	result = stoi((postEvalStack.top()).first);
	return result;
}

// Function definition for storing variables in symbolTable
void expEvaluator::storeVars(std::string var, std::string val)
{
	symbolTable[var] = val;
}

// Function definition for finding a value in symbolTable
std::string expEvaluator::findVars(std::string var)
{
	if (symbolTable.find(var) != symbolTable.end())
	{
		return symbolTable[var];
	}
	else
	{
		return "";
	}
}

// Function definition for determining precedence of a given operator
int expEvaluator::precedence(std::string op)
{
	if (op == "or")
		return 0;
	else if (op == "and")
		return 1;
	else if (op == "not")
		return 2;
	else if (op == "<" || op == "<=" || op == ">"
		|| op == ">=" || op == "!=" || op == "==")
		return 3;
	else if (op == "+" || op == "-")
		return 4;
	else if (op == "*" || op == "/" || op == "%")
		return 5;
}

// Function definition for diplaying variable information
void expEvaluator::displayVars()
{
	cout << "***** Symbol Table Variables and Values *****" << endl;

	map<string, string>::iterator it = symbolTable.begin();

	for (it; it != symbolTable.end(); it++)
	{
		cout << it->first << " = " << it->second << endl;
	}

	cout << endl << endl;
}

// Function definition for clearing symbolTable
void expEvaluator::clear()
{
	symbolTable.clear();
}