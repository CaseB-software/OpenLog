#include <iostream>

#include "OpenLog.h"
#include "LogTarget_Console.h"


int main(int argc, char* argv[]) {

	using namespace OpenLog;

	LT_Console_init();

	Tag tag{ "tag1" };
	
	registerTag(tag);

	Log testLog{ "msg" };
	testLog.addTag("LOG");
	testLog.addTag("WARNING");
	testLog.addTag("EXCEPT");


	if (!log(testLog)) {
		std::cout << "false" << std::endl;
	}

	std::string i;
	std::cin >> i;
	return 0;
}