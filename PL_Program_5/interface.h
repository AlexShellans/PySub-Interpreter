#ifndef INTERFACE_H
#define INTERFACE_H

#include <vector>
#include <string>


class Interface
{
private:
	typedef std::vector<std::string> programType;
	programType programCode;

public:

	// Function declarations
	void quit();
	void help(std::string);
	void read(std::string);
	void show(std::string);
	void run();
	void clear();

	void startInterface();
};

#endif