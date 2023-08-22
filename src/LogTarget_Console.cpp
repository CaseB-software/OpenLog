#include "LogTarget_Console.h"

// LT_Console
LT_Console::LT_Console(const std::string name) : LogTarget{ name } {

}
bool LT_Console::LogEvent(const Log& log) {
	std::cout << FormatLog(log);
	return true;
}
