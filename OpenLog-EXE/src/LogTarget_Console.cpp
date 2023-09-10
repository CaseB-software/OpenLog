#include "LogTarget_Console.h"

// LT_Console
LT_Console::LT_Console(const std::string name) : LogTarget{ name } {

}
bool LT_Console::LogEvent(const OpenLog::Log& log) {
	std::cout << defaultFormatLog(log);
	return true;
}

void LT_Console_init()
{
	using namespace OpenLog;

	registerLogTarget(std::make_unique<LT_Console>(LT_Console{ "console" }));
	addActiveLogTarget("console");

	Tag exception{ "EXCEPT" };
	Tag warning{ "WARNING" };
	Tag log{ "LOG" };

	registerTag(exception);
	registerTag(warning);
	registerTag(log);

	return;
}