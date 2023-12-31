#ifndef APP_H
#define APP_H

#include "../Public Headers/OpenLog.h"

#include <unordered_map>
#include <source_location>
#include <iomanip>
#include <memory>
#include <vector>
#include <algorithm>

namespace OpenLog {


	struct Settings {
		bool showTime;
		bool showTags;
		bool showMsg;
		bool showLocation;
		uint8_t widthOfTagTextBox;
		uint8_t logMsgMaxSize;
		int timeOffset;
	};

	class Application final {
	public:

		static Application& getInstance() noexcept;
		~Application() {};

		std::string version()		const;
		std::string compiledDate()	const;

		void registerTag		(std::unique_ptr<Tag>& tag);
		bool containsTag		(const std::string& key) const { return m_registerdTags.contains(key); }
		Tag* getTag				(const std::string& key) const;


		// Log Target Interaction
		void	registerLogTarget		(std::unique_ptr<LogTarget> target);
		bool	removeLogTarget			(const std::string key);
		bool 	addActiveLogTarget		(const std::string key);
		bool 	removeActiveLogTarget	(const std::string key);

		std::ostringstream	getAllActiveLogTargets	() const;
		LogTarget*			getLogTarget			(const std::string key) const;

		// Settings
		bool changeSettings			(SETTINGS settings, int value);
		bool changeSettings			(SETTINGS settings, bool value);
		const Settings& getSettings	() const { return m_settings; }

		// Do the logging
		bool log(const OpenLog::Log& log) const;

	private:
		Application();
		const Application& operator=(const Application& rst) = delete;

		Settings m_settings{ true,true,true,true,7,64, -7 };

		std::string m_VERSION		{ "v1.0.0" };
		std::string m_COMPILED_DATE	{ __DATE__ + std::string(" at ") + __TIME__ };

		std::unordered_map<std::string, std::unique_ptr<LogTarget>> m_logTargets{ };
		std::unordered_map<std::string, std::unique_ptr<Tag>>		m_registerdTags{ };

		// Current Profiles
		const int		m_maxActiveLogTargets{ 5 };
		std::vector<LogTarget*> m_activeLogTargets{};


	};

	// Non-Member Helper Functions
	std::string 	printTime(const std::chrono::time_point<std::chrono::system_clock>& time) 	noexcept; // Returns time in HH::MM::SS::XXX, XXX being milliseconds, it's in three digits
	std::string     defaultFormatLocation(const std::source_location location, bool oneLine = true) 	noexcept;


}
#endif
