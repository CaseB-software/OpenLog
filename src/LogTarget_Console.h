#pragma once

#include "OpenLog.h"

#include <iostream>

using namespace OpenLog;

class LT_Console : public LogTarget {
public:
    LT_Console(const std::string name);
    ~LT_Console() = default;

    bool LogEvent(const Log& log) override;

};

