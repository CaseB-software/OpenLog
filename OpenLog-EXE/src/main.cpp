#include <iostream>

#include "OpenLog.h"
#include "LogTarget_Console.h"


int main(int argc, char* argv[]) {

	using namespace OpenLog;

	LT_Console_init();

	Tag tag{ "tag1" };
	
	registerTag(tag);

	Log testLog{ "msg" };
	testLog.tag("LOG");
	testLog.tag("WARNING");
	testLog.tag("EXCEPT");


	if (!log(testLog)) {
		std::cout << "false" << std::endl;
	}

	return 0;
}