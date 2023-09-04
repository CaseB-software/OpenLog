#include "OpenLog.h"
#include "LogHandler.h"
#include "LogTarget_Console.h"

namespace OpenLog{

	Tag::Tag(const std::string tag) : m_tag{ tag } 	{	}
	Tag::~Tag()										{	}
	const std::string Tag::str() const noexcept		{ return m_tag; }
	std::ostream& operator<<(std::ostream& os, const Tag& tag){
		os << tag.str();
		return os;
	}



	Log::Log(	const std::string msg, 
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
	}
	

	LogTarget::LogTarget(const std::string name) : m_name{name} {
		
	}
	LogTarget::~LogTarget(){
		
	}
	bool LogTarget::LogEvent (const Log& log){
		return false;
	}
	const std::string 	LogTarget::str	() const noexcept {
		return m_name; 
	} 
	std::ostream& operator<<(std::ostream& os, const LogTarget& lt){
		os << lt.str();
		return os;
	}


	void registerTag(std::unique_ptr<Tag>& tag) {
		LogHandler::GetInstance().registerTag(tag);
	}
	void registerTag(Tag& tag) {
		std::unique_ptr<Tag> t{ std::make_unique<Tag>(tag) };
		LogHandler::GetInstance().registerTag(t);
	}


	bool init(){
		AddLogTarget(std::make_unique<LT_Console>(LT_Console{ "CONSOLE" }));
		AddActiveLogTarget("CONSOLE");
		
		Tag exception 	{ "EXCEPT" 	};
		Tag warning 	{ "WARNING" };
		Tag log			{ "LOG" 	};

		registerTag(exception);
		registerTag(warning);
		registerTag(log);

		return true;
	}

	void    AddLogTarget(std::unique_ptr<LogTarget> target) {
		LogHandler::GetInstance().AddLogTarget(std::move(target));
	}


	bool	AddActiveLogTarget	(const std::string target,  const std::source_location location)  {
			return LogHandler::GetInstance().AddActiveLogTarget(target);
	}
	bool    RemoveActiveLogTarget   (const std::string target,  const std::source_location location){
		return LogHandler::GetInstance().RemoveActiveLogTarget(target);
	}


std::string     FormatLog       (const Log& log){
	std::ostringstream os {};

	auto settings = LogHandler::GetInstance().GetSettings();

	if(settings.m_showTime)
		os << '[' << FormatTime(log.m_timestamp) << "]  ";


	if(settings.m_showTags) {
		
		// Iterate through tags for the log and display each tag
		for (const std::string& t : log.m_tags) {
			Tag* tagBuf{ LogHandler::GetInstance().getTag(t) };

			if (tagBuf) {
				os << '[' << std::setw(settings.m_widthOfCodeTextBox) << std::left;

				// No Centering if too long
				if (t.size() >= settings.m_widthOfCodeTextBox)
					os << t;

				else {
					// Center the log code
					uint64_t spaceLeftover{ settings.m_widthOfCodeTextBox - tagBuf->str().size() };
					int paddingFromLeft{ static_cast<int>(spaceLeftover / 2) };

					std::string paddedMsg{};
					for (int i{ 0 }; i < paddingFromLeft; ++i) {
						paddedMsg += ' ';
					}
					paddedMsg = paddedMsg + tagBuf->str();

					os << paddedMsg;
				}
				os << "]  ";
			}
			else {

			}
		}
		
	}
	if(settings.m_showMsg){
			// Take the first 64 characters and align correctly
		if(log.m_msg.length() < settings.m_logMsgMaxSize) 
			os << std::setw(settings.m_logMsgMaxSize) << std::left << log.m_msg;
		// If the msg is long, add spacing between next msg for readability
		else if(log.m_msg.length() >= settings.m_logMsgMaxSize) {
			os << log.m_msg;
			if(settings.m_showLocation) 
				os << std::endl << " > ";
		}	
	}
	if(settings.m_showLocation)
		os << FormatLocation(log.m_location);
	
	// Final steps
	os << std::endl;

	return os.str();
}


	// Logging
	bool	log	(std::string msg, std::string tag, const std::source_location location) {
		return LogHandler::GetInstance().Log(Log{ msg, tag, location });
	}

	std::string 	ThrowMSG	(const std::string msg, const std::source_location location){
		OpenLog::log(msg, "ERROR", location);
		return msg;
	}
}

