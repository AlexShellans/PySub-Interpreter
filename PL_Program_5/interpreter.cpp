#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include "interface.h"
#include "lexanalyzer.h"
#include "expevaluator.h"
#include "interpreter.h"

using namespace std;

// Function for interpreting one line of code; returns a boolean value indicating whether or not the 
// line could be interpreted without any errors
bool Interpreter::interpret(expEvaluator *expEvaluation, vector<pair<string, categoryType>> tokens, bool *inBlock, bool *whileHeader)
{
    // Variable for skipping indents during interpretation; increases whenever a conditional or while block
    // is being interpreted
    int k = 0;

    // Return if line is blank
    if (tokens.empty())
        return false;

    // Set inBlock to false if block of true if-segment has been executed
    if (tokens[0].second != categoryType::INDENT && tokens[0].first != "while" && trueStmt)
        *inBlock = false;

    // Set inBlock to false and return if line is outside of a while-block that is still being processed
    if (tokens[0].second != categoryType::INDENT && tokens[0].first != "while" && *whileHeader)
    {
        *inBlock = false;
        return false;
    }

    // Return if line is in a block of a skipped if-segment
    else if (tokens[k].second == categoryType::INDENT && !trueStmt)
        return false;
    else if (tokens[k].second == categoryType::INDENT && trueStmt && !*inBlock)
        return false;
    
    // Increase number of first token evaluated (skip the indent) if inside the block of a true if-segment or while-loop
    else if ((k+1 < tokens.size()) && (tokens[k].second == categoryType::INDENT) && trueStmt && *inBlock)
        k++;

    // Return if line contains a comment
    if (tokens[k].second == categoryType::COMMENT)
        return false;

    // Block for evaluating print statement, if/elif/else statement or while-loop
	else if (tokens[k].second == categoryType::KEYWORD)
	{
        // If keyword is "print"
		if (tokens[k].first == "print")
		{
            // Containers and variables for obtaining print statement
            vector<pair<string, categoryType>> infixExp;
            vector<pair<string, categoryType>> postfixExp;
            string printStmt = "";
            bool insidePar = false;
            int i = k+1;

            // Check for incorrect syntax (missing right parenthesis)
            if (tokens[tokens.size() - 1].second != categoryType::RIGHT_PAREN)
            {
                cout << "Error: unbalanced or missing delimiters." << endl << endl;
                return true;
            }

            // Read through the entire line
            while (i < tokens.size())
            {
                // Check whether we are inside print function parentheses
                if (tokens[i].second == categoryType::LEFT_PAREN)
                {
                    insidePar = true;
                }
                else if (tokens[i].second == categoryType::RIGHT_PAREN)
                {
                    insidePar = false;
                    break;
                }
                // Read print statement
                else if (insidePar == true)
                {
                    if (tokens[i].second == categoryType::STRING_LITERAL)
                    {
                        printStmt += tokens[i].first;
                    }
                    else if (tokens[i].second == categoryType::COMMA)
                    {
                        i++;
                        continue;
                    }
                    else
                    {
                        // Read each value that is not a comma or the end of the print statement (a right parenthesis)
                        while (tokens[i].second != categoryType::COMMA && tokens[i].second != categoryType::RIGHT_PAREN)
                        {
                            infixExp.push_back(tokens[i]);
                            i++;
                        }

                        postfixExp = expEvaluation->inToPost(infixExp);

                        // If postfixExp is empty (error returned from expEvaluator::inToPost), print error message
                        // and return true to indicate that an error was encountered
                        if (postfixExp.empty())
                        {
                            cout << "Error: variable is either undefined or not uninitialized." << endl << endl;
                            return true;
                        }

                        int result = expEvaluation->postEval(postfixExp);
                        printStmt += to_string(result);
                    }

                    printStmt += " ";
                }
                else
                {
                    cout << "Error: incorrect syntax." << endl << endl;
                    return true;
                }
                
                i++;
            }

            // Print the statement into the console
            cout << printStmt << endl;
            return false;
		}
        // If keyword is "if/elif"
        else if ((tokens[0].first == "if") || (tokens[0].first == "elif" && !trueStmt))
        {
            // Containers and variables for obtaining header expression
            vector<pair<string, categoryType>> infixExp;
            vector<pair<string, categoryType>> postfixExp;
            int i = 1;

            // Check for incorrect syntax (missing colon)
            if (tokens[tokens.size() - 1].second != categoryType::COLON)
            {
                cout << "Error: missing colon at end of if-statement." << endl << endl;
                return true;
            }

            // Read and evaluate header expression
            while (i < tokens.size() && (tokens[i].second != categoryType::COLON || tokens[i].second != categoryType::LEFT_PAREN
                || tokens[i].second != categoryType::RIGHT_PAREN))
            {
                infixExp.push_back(tokens[i]);
                i++;
            }

            postfixExp = expEvaluation->inToPost(infixExp);

            if (postfixExp.empty())
            {
                cout << "Error: variable is either not defined or uninitialized." << endl << endl;
                return true;
            }

            int result = expEvaluation->postEval(postfixExp);

            // Evaluate result; if true, set inBlock and trueStmt to true
            if (result)
            {
                *inBlock = true;
                trueStmt = true;
            }

            return false;
        }
        // If keyword is "else"
        else if (tokens[0].first == "else" && !trueStmt)
        {
            *inBlock = true;
            trueStmt = true;
            return false;
        }
        // If keyword is "while"
        else if (tokens[0].first == "while")
        {
            // Containers and variables for obtaining header expression
            vector<pair<string, categoryType>> infixExp;
            vector<pair<string, categoryType>> postfixExp;
            int i = 1;

            // Check for incorrect syntax (missing colon)
            if (tokens[tokens.size() - 1].second != categoryType::COLON)
            {
                cout << "Error: missing colon at end of loop header." << endl << endl;
                return true;
            }

            // Read and evaluate header expression
            while (i < tokens.size() && (tokens[i].second != categoryType::COLON || tokens[i].second != categoryType::LEFT_PAREN
                || tokens[i].second != categoryType::RIGHT_PAREN))
            {
                infixExp.push_back(tokens[i]);
                i++;
            }

            postfixExp = expEvaluation->inToPost(infixExp);

            if (postfixExp.empty())
            {
                cout << "Error: variable is either not defined or uninitialized." << endl << endl;
                return true;
            }

            int result = expEvaluation->postEval(postfixExp);

            // Evaluate result; if true, set inBlock, trueStmt and whileHeader to true, otherwise
            // set them to false
            if (result)
            {
                *inBlock = true;
                trueStmt = true;
                *whileHeader = true;
            }
            else
            {
                *inBlock = false;
                trueStmt = false;
                *whileHeader = false;
            }

            return false;
        }
	}

    // Block for evaluating assignment
    else if (tokens[k].second == categoryType::IDENTIFIER)
    {
        // Containers and variables for holding variables and expressions
        string var = tokens[k].first;
        vector<pair<string, categoryType>> infixExp;
        vector<pair<string, categoryType>> postfixExp;
        int i = k+2;

        // If right-hand side of assignment is an expression, evaluate the expression
        if (k+2 < tokens.size() && (tokens[k+2].second == categoryType::NUMERIC_LITERAL || 
            tokens[k+2].second == categoryType::IDENTIFIER || tokens[k+2].first == "not"))
        {
            while (i < tokens.size() && tokens[i].second != categoryType::COMMENT)
            {
                infixExp.push_back(tokens[i]);
                i++;
            }

            postfixExp = expEvaluation->inToPost(infixExp);

            if (postfixExp.empty())
            {
                cout << "Error: variable is either not defined or not initialized." << endl << endl;
                return true;
            }

            int val = expEvaluation->postEval(postfixExp);
            expEvaluation->storeVars(var, to_string(val));
        }
        // If right-hand side of assignment is an input statement, print prompt and obtain input from user
        else if (k+2 < tokens.size() && tokens[k+2].second == categoryType::KEYWORD)
        {
            if (tokens[k+2].first == "int" && (k+4 < tokens.size() && tokens[k+4].first == "input"))
            {
                string prompt = "";
                string input = "";
                int i = k+6;

                // Check for incorrect syntax (missing right parenthesis)
                if (tokens[tokens.size() - 1].second != categoryType::RIGHT_PAREN)
                {
                    cout << "Error: unbalanced or missing delimiters." << endl << endl;
                    return true;
                }

                // Obtain prompt
                while (i < tokens.size() && tokens[i].second == categoryType::STRING_LITERAL)
                {
                    prompt += tokens[i].first;
                    prompt += " ";
                    i++;
                }

                // Print prompt and obtain input from user
                cout << prompt;
                cin >> input;

                expEvaluation->storeVars(tokens[0].first, input);
            }
            else
            {
                cout << "Error: incorrect syntax." << endl << endl;
                return true;
            }
        }
        else
        {
            cout << "Error: invalid command or incorrect syntax." << endl << endl;
            return true;
        }
    }
    // Print error message if line cannot be interpreted with any of the above blocks and is not a sole 
    // indent (blank line)
    else if (tokens[k].second != categoryType::INDENT)
    {
        cout << "Error: incorrect syntax." << endl << endl;
        return true;
    }
    
    // If the end of the function is reached, return false to indicate that the line was read without
    // encountering any errors
    return false;
}