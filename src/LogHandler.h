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

class LogHandler final {
public:
	struct Settings;

	static LogHandler& GetInstance() noexcept;
    ~LogHandler() 	{};

	void registerTag	(std::unique_ptr<Tag>& tag);
	bool containsTag	(const std::string& key) const { return m_registerdTags.contains(key); }
	Tag* getTag			(const std::string& key) const;


	// Log Target Interaction
	void		AddLogTarget			(std::unique_ptr<LogTarget> target);
	bool		RemoveLogTarget			(const std::string key);
	bool 		AddActiveLogTarget		(const std::string key);
	bool 		RemoveActiveLogTarget	(const std::string key);

    // Settings
	bool ChangeSettings(SETTINGS settings, uint16_t value);
    bool ChangeSettings(SETTINGS settings, bool value);
	const Settings& GetSettings() { return m_settings; }

	// Do the logging
	bool Log (const OpenLog::Log& log);

private:
	LogHandler();
    const LogHandler &operator=(const LogHandler& rst) = delete;

	struct Settings {
		bool m_showTime		{ true };
		bool m_showTags		{ true };
		bool m_showMsg		{ true };
		bool m_showLocation	{ true };
		uint8_t m_widthOfCodeTextBox	{ 7 };
		uint8_t m_logMsgMaxSize			{ 64 };
	};
	Settings m_settings;


	std::unordered_map<std::string, std::unique_ptr<LogTarget>> m_logTargets	{ };
	std::unordered_map<std::string, std::unique_ptr<Tag>>		m_registerdTags	{ };


	// Current Profiles
	static const int m_maxActiveLogTargets {5};
	std::vector<LogTarget*> m_activeLogTargets{};


};

// Non-Member Helper Functions
std::string 	FormatTime		(const std::chrono::time_point<std::chrono::system_clock>& time) 	noexcept; // Returns time in HH::MM::SS::XXX, XXX being milliseconds, it's in three digits
std::string     FormatLocation	(const std::source_location location, bool oneLine=true) 			noexcept;



#endif
