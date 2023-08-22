#ifndef LOGGER_H
#define LOGGER_H

#pragma warning(disable : 4996) // ignore localtime usage
#pragma warning(disable : 2397) // ignore uint64 to int narrowing


#include "OpenLog.h"

#include <unordered_map>
#include <source_location>
#include <iomanip>
#include <memory>
#include <vector>
#include <algorithm>

using namespace OpenLog;


template<typename T>
T* FindInMap(const std::string key, std::unordered_map<std::string, std::unique_ptr<T>>& map){
	try{
		auto& buffer { map.at(key) };
		return buffer.get();									
	}
	// If key could not be found, return false
	catch(std::out_of_range& e){
		return nullptr;
	}
}

class LogHandler final {
public:
	static LogHandler& GetInstance() noexcept;
    ~LogHandler() 	{};


	bool SetLogFilter			(const std::string key)		noexcept;

	// Log Filter Interaction
	void		AddLogFilter	(std::unique_ptr<LogFilter> filter);
	bool		RemoveLogFilter	(const std::string key);
	LogFilter* 	GetLogFilter 	(const std::string key);

	// Log Target Interaction
	void		AddLogTarget			(std::unique_ptr<LogTarget> target);
	bool		RemoveLogTarget			(const std::string key);
	LogTarget* 	GetLogTarget 			(const std::string key);
	bool 		AddActiveLogTarget		(const std::string key);
	bool 		RemoveActiveLogTarget	(const std::string key);

    // Settings
	bool ChangeSettings(SETTINGS settings, uint16_t value);
    bool ChangeSettings(SETTINGS settings, bool value);

	// Do the logging
	bool Log (const std::string& msg, const std::vector<Tag> tag, const std::source_location location=std::source_location::current());

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


	// Variables
	std::unordered_map<std::string, std::unique_ptr<LogFilter>> m_logFilters 	{ };
	std::unordered_map<std::string, std::unique_ptr<LogTarget>> m_logTargets	{ };

	// Current Profiles
	static const int m_maxActiveLogTargets {5};
	std::vector<LogTarget*> m_logTargetsActive	{ nullptr };
	LogFilter*				m_logFilter 		{ nullptr };


};

// Non-Member Helper Functions
std::string 	FormatTime		(const std::chrono::time_point<std::chrono::system_clock>& time) 	noexcept; // Returns time in HH::MM::SS::XXX, XXX being milliseconds, it's in three digits
std::string     FormatLocation	(const std::source_location location, bool oneLine=true) 			noexcept;



#endif