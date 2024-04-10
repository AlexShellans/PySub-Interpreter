#include <iostream>
#include <string>
#include <fstream>
#include "interface.h"
#include "lexanalyzer.h"
#include "expevaluator.h"
#include "interpreter.h"

using namespace std;

LexicalAnalyzer lexAnalysis;
expEvaluator expEvaluation;
Interpreter pysubi;

// Definition of startInterface() function
void Interface::startInterface()
{
    cout << "PySUB Interpreter 1.0 on Windows (September 2020)" << endl;
    cout << "Enter program lines or read(<filename>.py) at command line interface" << endl;
    cout << "Type 'help' for more information or 'quit' to exit" << endl;
    cout << ">>> ";

    // Get input from user
    string input;
    getline(cin, input);

    while (input != "quit")
    {
        // Generate tokens from user input
        vector<string> infixVect{ input };
        lexAnalysis.generatePair(infixVect);
        vector<pair<string, categoryType>> tokens = lexAnalysis.getLAData();

        // If user enters an interface command, evaluate it and call the corresponding function
        if (tokens[0].first == "quit" || tokens[0].first == "help" || tokens[0].first == "read" ||
            tokens[0].first == "show" || tokens[0].first == "clear" || tokens[0].first == "run")
        {
            // Variables for holding commands, arguments, and for evaluating each one
            string command = "";
            string argument = "";
            bool insidePar = false;
            int i = 0;

            // Read through the input
            while (i != input.length())
            {
                // Read command and argument if provided
                if (input[i] == '(')
                {
                    insidePar = true;
                }
                else if (input[i] == ')')
                {
                    break;
                }
                else if (insidePar == false)
                {
                    command += input[i];
                }
                else if (insidePar == true)
                {
                    argument += input[i];
                }

                i++;
            }

            // Evaluate command and pass argument
            if (command != "")
            {
                if (command == "quit")
                {
                    Interface::quit();
                }
                else if (command == "help")
                {
                    Interface::help(argument);

                    cout << ">>> ";
                    cin.ignore();
                    getline(cin, input);
                    continue;
                }
                else if (command == "read")
                {
                    Interface::read(argument);
                    lexAnalysis.generatePair(programCode);
                    cout << endl;
                }
                else if (command == "show")
                {
                    Interface::show(argument);
                    cout << endl;
                }
                else if (command == "clear")
                {
                    Interface::clear();
                }
                else if (command == "run")
                {
                    Interface::run();

                    cout << ">>> ";
                    getline(cin, input);
                    continue;
                }
                else
                {
                    cout << "Command not recognized. Please enter a valid command." << endl << endl;
                }
            }
        }

        // If user enters an expression or program code, read and evaluate it
        else
        {
            // If input is an expression, evaluate and print its result
            
            if (tokens.size() > 2 && (tokens[0].second == categoryType::NUMERIC_LITERAL || tokens[0].second == categoryType::IDENTIFIER
                || tokens[0].second == categoryType::LEFT_PAREN) && tokens[1].second != categoryType::ASSIGNMENT_OP)
            {
                vector<pair<string, categoryType>> postfixExp = expEvaluation.inToPost(tokens);

                // Expression error handling
                if (postfixExp.empty())
                {
                    cout << ">>> ";
                    getline(cin, input);
                    continue;
                }

                // Pass postfix conversion to be evaluated, then print the result
                int result = expEvaluation.postEval(postfixExp);
                cout << result << endl;
            }
            // If input is program code, use the run() function to evaluate it
            else
            {
                // Check if line is a while or conditional statement
                if (tokens[0].first == "if" || tokens[0].first == "elif" || tokens[0].first == "else" || tokens[0].first == "while")
                {
                    // Check for incorrect syntax (missing colon)
                    if (tokens[tokens.size() - 1].second != categoryType::COLON)
                    {
                        cout << "Error: missing colon at end of loop header." << endl << endl;
                    }
                    else
                    {
                        int i = 0;
                        
                        // Clear previous program code and start adding if- or while-block code to programCode
                        programCode.clear();
                        programCode.push_back(input);

                        // Read code in silent mode
                        while (input != "")
                        {
                            cout << "...";
                            getline(cin, input);
                            programCode.push_back(input);
                        }

                        run();
                    }
                }

                // If line contains a comment, print statement, or assignment, send it to be interpreted with run()
                else
                {
                    programCode.clear();
                    programCode.push_back(input);
                    run();
                }
            }
        }

        cout << ">>> ";
        getline(cin, input);
    }
}

// Definiton of quit() function
void Interface::quit()
{
    exit(0);
}

// Definition of help() function
void Interface::help(string command)
{
    // Enter help utility if function is called with no argument
    if (command == "")
    {
        cout << "Welcome to the help utility!" << endl;
        cout << "* To exit and return to the interpreter, type 'exit'" << endl;
        cout << "* To get a list of commands, type 'commands'" << endl;
        cout << "* To get a description of any command, just type the command at the help> prompt" << endl << endl;
        cout << "help> ";

        cin >> command;

        // Stay within help utility unless exited by user
        while (command != "exit")
        {
            if (command == "quit")
            {
                cout << "This command exits the command line interpreter." << endl;
                cout << "To exit the help utility and return to the interpreter, enter 'exit'" << endl << endl;
                cout << "help> ";

                cin >> command;
            }
            else if (command == "help")
            {
                cout << "This command enters the help utility and lists available commands and their descriptions." << endl << endl;
                cout << "help> ";

                cin >> command;
            }
            else if (command == "read")
            {
                cout << "This command clears any lines that are stored in the program data structure, and then reads" << endl;
                cout << "program lines from the file given as a parameter and stores the lines into the program data structure." << endl << endl;
                cout << "help> ";

                cin >> command;
            }
            else if (command == "show")
            {
                cout << "This command shows the lines of the program that are stored in the program data structure." << endl << endl;
                cout << "help> ";

                cin >> command;
            }
            else if (command == "run")
            {
                cout << "This command runs the program that is stored in the program data structure." << endl << endl;
                cout << "help> ";

                cin >> command;
            }
            else if (command == "clear")
            {
                cout << "This command clears out or deletes and lines that are stored in the program data structure." << endl << endl;
                cout << "help> ";

                cin >> command;
            }
            else if (command == "commands")
            {
                cout << "Below is a list of commands. Enter any command at the prompt to get more help" << endl << endl;
                cout << "clear\thelp\tquit\nread\tshow\trun" << endl << endl;
                cout << "NOTE: All commands can also be entered as functions:" << endl << endl;
                cout << "clear()\thelp()\tquit()\nread()\tshow()\trun()" << endl << endl;
                cout << "help> ";

                cin >> command;
            }
            else
            {
                cout << "Command not recognized. Please enter a valid command." << endl << endl;
                cout << "help> ";

                cin >> command;
            }
        }
    }

    // List description called for by user if argument is provided
    else
    {
        if (command == "quit")
        {
            cout << "This command exits the command line interpreter." << endl;
            cout << "To exit the help utility and return to the interpreter, enter 'exit'" << endl << endl;
        }
        else if (command == "help")
        {
            cout << "This command enters the help utility and lists available commands and their descriptions." << endl << endl;
        }
        else if (command == "read")
        {
            cout << "This command clears any lines that are stored in the program data structure, and then reads" << endl;
            cout << "program lines from the file given as a parameter and stores the lines into the program data structure." << endl << endl;
        }
        else if (command == "show")
        {
            cout << "This command shows the lines of the program that are stored in the program data structure." << endl << endl;
        }
        else if (command == "run")
        {
            cout << "This command runs the program that is stored in the program data structure." << endl << endl;
        }
        else if (command == "clear")
        {
            cout << "This command clears out or deletes and lines that are stored in the program data structure." << endl << endl;
        }
        else if (command == "commands")
        {
            cout << "Below is a list of commands. Enter any command at the prompt to get more help" << endl << endl;
            cout << "clear\thelp\tquit\nread\tshow\trun" << endl << endl;
            cout << "NOTE: All commands can also be entered as functions:" << endl << endl;
            cout << "clear()\thelp()\tquit()\nread()\tshow()\trun()" << endl << endl;
        }
        else
        {
            cout << "Command not recognized. Please enter a valid command." << endl << endl;
        }
    }

    cout << endl;
}

// Definition of read() function
void Interface::read(string filename)
{
    programCode.clear();

    ifstream infile;
    string line;
    infile.open(filename);

    // Check that file exists
    if (!infile)
    {
        cout << "Error: file not found." << endl;
        return;
    }

    // Read lines from file and store in programCode
    while (true)
    {
        if (getline(infile, line))
        {
            programCode.push_back(line);
        }
        else
        {
            break;
        }
    }

    // Close the file
    infile.close();
}

// Definition of show() function
void Interface::show(string arg)
{
    // Print each line stored in programFunction
    if (arg == "")
    {
        for (int i = 0; i < programCode.size(); i++)
        {
            cout << "[" << i << "] " << programCode[i] << endl;
        }
    }
    else if (arg == "tokens")
    {
        // Display tokens
        lexAnalysis.displayLAData();
    }
    else if (arg == "variables")
    {
        expEvaluation.displayVars();
    }
}

// Definition of run() function
void Interface::run()
{
    // Generate token info
    lexAnalysis.generatePair(programCode);
    vector<pair<string, categoryType>> tokens;

    // Variables for checking various conditions in programCode
    bool error;
    bool whileHeader = false;
    bool inBlock = false;

    // Interpret each line of code, breaking if an error is encountered
    for (int i = 0; i < programCode.size(); i++)
    {
        tokens = lexAnalysis.getLAData(i);
        error = pysubi.interpret(&expEvaluation, tokens, &inBlock, &whileHeader);

        // Enter into inner while-loops if "while" statement is encountered
        if (whileHeader)
        {
            int j;

            // Loop until expression in while header is no longer true
            while (whileHeader)
            {
                inBlock = true;
                j = 0;

                // Loop over code block that is inside while-loop
                while (inBlock && (i+j < programCode.size()))
                {
                    tokens = lexAnalysis.getLAData(i + j);
                    error = pysubi.interpret(&expEvaluation, tokens, &inBlock, &whileHeader);

                    if (error)
                        break;

                    j++;
                }

                if (error)
                    break;
            }
        }

        if (error)
            break;
    }

    cout << endl;
}

// Definition of clear() function
void Interface::clear()
{
    programCode.clear();
    lexAnalysis.clear();
    expEvaluation.clear();
}