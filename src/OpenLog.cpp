#include "OpenLog.h"
#include "LogHandler.h"

namespace OpenLog {

	Tag::Tag(const std::string tag) : m_tag{ tag } {	}
	Tag::~Tag() {	}
	const std::string Tag::str() const noexcept { return m_tag; }
	std::ostream& operator<<(std::ostream& os, const Tag& tag) {
		os << tag.str();
		return os;
	}

	void registerTag(Tag& tag) {
		std::unique_ptr<Tag> t{ std::make_unique<Tag>(tag) };
		LogHandler::GetInstance().registerTag(t);
	}
	bool tagIsRegistered(const std::string& key) {
		return  LogHandler::GetInstance().containsTag(key);
	}
	Tag* getRegisteredTag(const std::string& key) {
		return  LogHandler::GetInstance().getTag(key);
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
	bool Log::tag(const std::string& tag) {
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
		return LogHandler::GetInstance().log(Log{ msg.str(), tag, location});
	}
	bool log(const std::string msg, const std::string tag, const std::source_location location) {
		return LogHandler::GetInstance().log(Log{ msg, tag, location });
	}
	bool log(const Log& log) {
		return LogHandler::GetInstance().log(log);
	}

	std::string defaultFormatLog(const Log& log) {
		std::ostringstream os{};

		auto settings = LogHandler::GetInstance().getSettings();

		if (settings.showTime)
			os << '[' << defaultFormatTime(log.m_timestamp) << "]  ";


		if (settings.showTags) {

			// Iterate through tags for the log and display each tag
			for (const std::string& t : log.m_tags) {
				Tag* tagBuf{ LogHandler::GetInstance().getTag(t) };

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
	void    addNewLogTarget(std::unique_ptr<LogTarget> target) {
		LogHandler::GetInstance().addNewLogTarget(std::move(target));
	}
	bool	addActiveLogTarget(const std::string target, const std::source_location location) {
		return LogHandler::GetInstance().addActiveLogTarget(target);
	}
	bool    removeActiveLogTarget(const std::string target, const std::source_location location) {
		return LogHandler::GetInstance().removeActiveLogTarget(target);
	}
}
