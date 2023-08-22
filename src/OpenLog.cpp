#include "OpenLog.h"
#include "Logger.h"
#include "LogTarget_Console.h"

namespace OpenLog{

	Tag::Tag(const std::string tag) : m_tag{ tag } 	{   }
	Tag::~Tag()										{	}
	const std::string Tag::str() const noexcept		{ return m_tag; }
	std::ostream& operator<<(std::ostream& os, const Tag& tag){
		os << tag.str();
		return os;
	}

	Log::Log(const std::string msg, const std::vector<Tag> tags, const std::source_location location, const std::chrono::time_point<std::chrono::system_clock> timestamp) : m_msg{ msg }, m_location{ location }, m_timestamp{ timestamp }, m_tags{ tags } {

	}

	



	// Log Fillter
	LogFilter::LogFilter    	(const std::string name) : m_name {name} 	{	}
	LogFilter::~LogFilter		()											{	}
	
	void LogFilter::AddTag(const Tag& tag){
		m_allowedTags.try_emplace(tag.str(), tag);
		return;
	}

	bool 	LogFilter::RemoveTag 	(const std::string key){
		try{
			auto& foundLogCode { m_allowedTags.at(key) };	// Reference to the logCode if found
			m_allowedTags.erase(key);
			return true;									
		}
		// If key could not be found, return false
		catch(std::out_of_range& e){
			return false;
		}
	}
	bool	LogFilter::IsAllowed  		( const std::string key  ) const {
		return m_allowedTags.contains(key);
	}
	const std::string 	LogFilter::str	() const noexcept {
		return m_name; 
	} 
	std::ostream& operator<<(std::ostream& os, const LogFilter& lf){
		os << lf.str();
		return os;
	}



	// Log Target
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


	bool init(){
		auto console = std::make_unique<LT_Console>("CONSOLE");
		AddLogTarget(std::move(console));
		AddActiveLogTarget("CONSOLE");
		
		Tag exception 	{ "EXCEPT" 	};
		Tag warning 	{ "WARNING" };
		Tag log			{ "LOG" 	};

		auto defaultFilter = std::make_unique<LogFilter>("DEFAULT");
		defaultFilter->AddTag(exception);
		defaultFilter->AddTag(warning);
		defaultFilter->AddTag(log);

		AddLogFilter(std::move(defaultFilter));
		SetLogFilter("DEFAULT");

		return true;
	}

	void 	AddLogFilter 	(const LogFilter& filter){
		Logger::GetInstance().AddLogFilter(std::move(filter));
		return;
	}
    void    AddLogTarget    (const LogTarget& target){
		Logger::GetInstance().AddLogTarget(std::move(target));
		return;
	}
	bool 	SetLogFilter	(const std::string filter, const std::source_location location){
		if(!Logger::GetInstance().SetLogFilter(filter)){
			throw std::invalid_argument("No filter exists");
		}
		else
			return true;
		return false;
	}
	bool	AddActiveLogTarget	(const std::string target,  const std::source_location location)  {
			return Logger::GetInstance().AddActiveLogTarget(target);
	}
	bool    RemoveActiveLogTarget   (const std::string target,  const std::source_location location){
		return Logger::GetInstance().RemoveActiveLogTarget(target);
	}


std::string     FormatLog       (const Log& log){
	std::ostringstream os {};

	auto settings = Logger::GetInstance().GetSettings();

	if(settings.m_showTime)
		os << '[' << FormatTime(log.m_timestamp) << "]  ";


	if(settings.m_showTags) {
		

		// Iterate through tags for the log and display each tag
		for (const Tag& t : log.m_tags) {
			os << '[' << std::setw(settings.m_widthOfCodeTextBox) << std::left;

			// No Centering if too long
			if (t.str().size() >= settings.m_widthOfCodeTextBox)
				os << t.str();

			else {
				// Center the log code
				uint64_t spaceLeftover{ settings.m_widthOfCodeTextBox - t.str().size()};
				int paddingFromLeft{ static_cast<int>(spaceLeftover / 2) };

				std::string paddedMsg{};
				for (int i{ 0 }; i < paddingFromLeft; ++i) {
					paddedMsg += ' ';
				}
				paddedMsg = paddedMsg + t.str();

				os << paddedMsg;
			}

			os << "]  ";
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

	LogFilter*  GetLogFilter   (const std::string key){
		return Logger::GetInstance().GetLogFilter(key);
	}
	LogTarget*  GetLogTarget   (const std::string key){
		return Logger::GetInstance().GetLogTarget(key);
	}


	// Logging
	bool 			log			(const std::string msg, const std::vector<Tag> tag, const std::source_location location){
		return Logger::GetInstance().Log(msg, tag, location);
	}
	bool        log				(const std::string msg, const std::vector<std::string>  tag, const std::source_location location) {
		std::vector<Tag> tagBuf;
		for (const std::string& s : tag) {
			Tag t{ s };
			tagBuf.push_back(t);
		}
		return Logger::GetInstance().Log(msg, tagBuf, location);
	}

	std::string 	ThrowMSG	(const std::string msg, const std::source_location location){
		OpenLog::log(msg, std::vector<std::string>{"ERROR"}, location);
		return msg;
	}
	

}

