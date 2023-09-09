#pragma once

#include "OpenLog.h"

#include <iostream>

class LT_Console : public OpenLog::LogTarget {
public:
    LT_Console(const std::string name);
    ~LT_Console() = default;

    bool LogEvent(const OpenLog::Log& log) override;

};

void LT_Console_init();

