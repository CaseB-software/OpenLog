#include <iostream>

#include "OpenLog.h"

int main(int argc, char* argv[]) {

	using namespace OpenLog;

	Tag tag{ "tag1" };
	
	registerTag(tag);

	log("test msg", "tag1");

	return 0;
}