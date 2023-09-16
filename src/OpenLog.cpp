#include "../Public Headers/OpenLog.h"
#include "Application.h"


namespace OpenLog {

	Application& app{ Application::getInstance() };


	std::string version() {
		return app.version();
	}
	std::string compiledDate() {
		return app.compiledDate();
	}


	Tag::Tag(const std::string tag) : m_tag{ tag } {	}
	Tag::~Tag() {	}
	const std::string Tag::str() const noexcept { return m_tag; }
	std::ostream& operator<<(std::ostream& os, const Tag& tag) {
		os << tag.str();
		return os;
	}

	void registerTag(Tag& tag) {
		std::unique_ptr<Tag> t{ std::make_unique<Tag>(tag) };
		app.registerTag(t);
	}
	bool isTagRegistered(const std::string key) {
		return  app.containsTag(key);
	}
	Tag* getRegisteredTag(const std::string key) {
		return  app.getTag(key);
	}





	Log::Log(const std::string msg,
		const std::string tag,
		const std::source_location location,
		const std::chrono::time_point<std::chrono::system_clock> timestamp
	)
		: m_msg{ msg }, m_location{ location }, m_timestamp{ timestamp }
	{
		m_tags.push_back(tag);
	}
	bool Log::addTag(const std::string& tag) {
		// Check if a tag of the same string has been added already, if not, add it.
		for (const Tag& t : m_tags) {
			if (t.str() == tag) {
				return false;
			}
			else {
				m_tags.push_back(tag);
				return true;
			}
		}

		return false;
	}
	bool log(const std::ostringstream msg, const std::string tag, const std::source_location location) {
		return app.log(Log{ msg.str(), tag, location });
	}
	bool log(const std::string msg, const std::string tag, const std::source_location location) {
		return app.log(Log{ msg, tag, location });
	}
	bool log(const Log& log) {
		return app.log(log);
	}

	std::string printLog(const Log& log) {
		std::ostringstream os{};

		auto settings = app.getSettings();

		if (settings.showTime)
			os << '[' << printTime(log.m_timestamp) << "]  ";


		if (settings.showTags) {

			// Iterate through tags for the log and display each tag
			for (const std::string& t : log.m_tags) {
				Tag* tagBuf{ app.getTag(t) };

				if (tagBuf) {
					os << '[' << std::setw(settings.widthOfTagTextBox) << std::left;

					// No Centering if too long
					if (t.size() >= settings.widthOfTagTextBox)
						os << t;

					else {
						os << std::format("{:^{}}", tagBuf->str(), settings.widthOfTagTextBox);
					}
					os << "]  ";
				}
				else {

				}
			}

		}
		if (settings.showMsg) {
			// Take the first 64 characters and align correctly
			if (log.m_msg.length() < settings.logMsgMaxSize)
				os << std::setw(settings.logMsgMaxSize) << std::left << log.m_msg;
			// If the msg is long, add spacing between next msg for readability
			else if (log.m_msg.length() >= settings.logMsgMaxSize) {
				os << log.m_msg;
				if (settings.showLocation)
					os << std::endl << " > ";
			}
		}
		if (settings.showLocation)
			os << defaultFormatLocation(log.m_location);

		// Final steps
		os << std::endl;

		return os.str();
	}






	LogTarget::LogTarget(const std::string name) : m_name{ name } {

	}
	LogTarget::~LogTarget() {

	}
	bool LogTarget::LogEvent(const Log& log) {
		return false;
	}
	const std::string 	LogTarget::str() const noexcept {
		return m_name;
	}
	std::ostream& operator<<(std::ostream& os, const LogTarget& lt) {
		os << lt.str();
		return os;
	}
	void    registerLogTarget(std::unique_ptr<LogTarget> target) {
		app.registerLogTarget(std::move(target));
	}
	bool	addActiveLogTarget(const std::string target, const std::source_location location) {
		return app.addActiveLogTarget(target);
	}
	bool    removeActiveLogTarget(const std::string target, const std::source_location location) {
		return app.removeActiveLogTarget(target);
	}
	std::ostringstream getAllActiveLogTargets() {
		return app.getAllActiveLogTargets();
	}
	LogTarget* getLogTarget(const std::string key) {
		return app.getLogTarget(key);
	}

}
