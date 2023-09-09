#ifndef LOGGER_H
#define LOGGER_H

#include "OpenLog.h"

#include <unordered_map>
#include <source_location>
#include <iomanip>
#include <memory>
#include <vector>
#include <algorithm>

using namespace OpenLog;

struct Settings {
	bool showTime;
	bool showTags;
	bool showMsg;
	bool showLocation;
	uint8_t widthOfTagTextBox;
	uint8_t logMsgMaxSize;
};

class LogHandler final {
public:

	static LogHandler& GetInstance() noexcept;
    ~LogHandler() 	{};

	void registerTag	(std::unique_ptr<Tag>& tag);
	bool containsTag	(const std::string& key) const { return m_registerdTags.contains(key); }
	Tag* getTag			(const std::string& key) const;


	// Log Target Interaction
	void		addNewLogTarget			(std::unique_ptr<LogTarget> target);
	bool		removeLogTarget			(const std::string key);
	bool 		addActiveLogTarget		(const std::string key);
	bool 		removeActiveLogTarget	(const std::string key);

    // Settings
	bool changeSettings(SETTINGS settings, int value);
    bool changeSettings(SETTINGS settings, bool value);
	const Settings& getSettings() { return m_settings; }

	// Do the logging
	bool log (const OpenLog::Log& log);

private:
	LogHandler();
    const LogHandler &operator=(const LogHandler& rst) = delete;

	Settings m_settings{ true,true,true,true,7,64 };


	std::unordered_map<std::string, std::unique_ptr<LogTarget>> m_logTargets	{ };
	std::unordered_map<std::string, std::unique_ptr<Tag>>		m_registerdTags	{ };


	// Current Profiles
	static const int		m_maxActiveLogTargets {5};
	std::vector<LogTarget*> m_activeLogTargets{};


};

// Non-Member Helper Functions
std::string 	defaultFormatTime		(const std::chrono::time_point<std::chrono::system_clock>& time) 	noexcept; // Returns time in HH::MM::SS::XXX, XXX being milliseconds, it's in three digits
std::string     defaultFormatLocation	(const std::source_location location, bool oneLine=true) 			noexcept;



#endif
