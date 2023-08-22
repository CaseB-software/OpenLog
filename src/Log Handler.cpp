#include "LogHandler.h"


LogHandler::LogHandler() {

}
LogHandler& LogHandler::GetInstance() noexcept{
	static LogHandler instance;

	return instance;
}

bool LogHandler::SetLogFilter	(const std::string  key)		noexcept{
	if( m_logFilters.contains(key) ){
		m_logFilter = m_logFilters.at(key).get();
		return true;
	}
	else
		return false;
}

bool LogHandler::AddActiveLogTarget	(const std::string  key)		{
	if( m_logTargets.contains(key) ){

		if(m_logTargetsActive.size() >= m_maxActiveLogTargets){
			m_logTargetsActive.resize(5);
			return false;
		}
		else{
			m_logTargetsActive.push_back(m_logTargets.at(key).get());
			return true;
		}
	}
	else
		throw std::invalid_argument("RS OpenLog: No known Log Targets could be found with key " + key);
}
bool LogHandler::RemoveActiveLogTarget	(const std::string key){
	int i{0};
	bool success{false};
	for(LogTarget* currentLogTarget : m_logTargetsActive){
		if(currentLogTarget){
			if(currentLogTarget->str() == key){
				m_logTargetsActive[i] = nullptr;
				success = true;
			}
		}
		++i;
	}

	return success;
}

bool 			LogHandler::Log				(const std::string& msg, const std::vector<Tag> tag, const std::source_location location){
	bool success {false};



	if(m_logTargetsActive.size() <= 0) { return false; }

	for(LogTarget* currentLogTarget : m_logTargetsActive){
		if(currentLogTarget){

			//if (m_logFilter) {
				 //Verify code is present in the current LogFilter
				//if (m_logFilter->IsAllowed(code)) {
					//OpenLog::Log logBuffer{ msg, location, std::chrono::system_clock::now() }; // Create the buffer to be referenced
					//logBuffer.AddTag(code);

					//success = currentLogTarget->LogEvent(logBuffer); // TODO counting of how many Log calls returned false and able to report back if needed
				//}
			//}
			//else {
				OpenLog::Log logBuffer{ msg, tag, location, std::chrono::system_clock::now() }; // Create the buffer to be referenced
				

				success = currentLogTarget->LogEvent(logBuffer); // TODO counting of how many Log calls returned false and able to report back if needed
			//}
		}
	}
	return success;
}

// Log Filter
void		LogHandler::AddLogFilter	(std::unique_ptr<LogFilter> filter){
	m_logFilters.try_emplace(filter->str(), std::move(filter));
}
bool		LogHandler::RemoveLogFilter	(const std::string key){
	try{
		auto& foundFilter { m_logFilters.at(key) };		// Reference to the logCode if found
		foundFilter.release();							// Delete LogCode
		m_logFilters.erase(key);
		return true;									
	}
	// If key could not be found, return false
	catch(std::out_of_range& e){
		return false;
	}
}
LogFilter* LogHandler::GetLogFilter 	(const std::string key){
	return FindInMap(key, m_logFilters);
}
	
// Log Target
void		LogHandler::AddLogTarget	(std::unique_ptr<LogTarget> target){
	m_logTargets.try_emplace(target->str(), std::move(target));
}
bool		LogHandler::RemoveLogTarget	(const std::string key){
	try{
		auto& foundTarget { m_logTargets.at(key) };		// Reference to the logCode if found
		foundTarget.release();							// Delete LogCode
		m_logTargets.erase(key);
		return true;									
	}
	// If key could not be found, return false
	catch(std::out_of_range& e){
		return false;
	}
}
LogTarget* LogHandler::GetLogTarget 	(const std::string key){
	return FindInMap(key, m_logTargets);
}

// Settings
bool LogHandler::ChangeSettings(SETTINGS settings, uint16_t value){
	switch(settings){
			case CODE_TEXT_WIDTH:
			m_defaultLogSettings.m_widthOfCodeTextBox = value;
			return true;
			break;
			case LOG_MSG_MAX_SIZE:
			m_defaultLogSettings.m_logMsgMaxSize = value;
			return true;
			break;
		default:
			return false;
			break;
	}
	return false;
}
bool LogHandler::ChangeSettings(SETTINGS settings, bool value){
	switch(settings){
		case SHOW_TIME:
			m_defaultLogSettings.m_showTime = value;
			return true;
			break;
		case SHOW_TAGS:
			m_defaultLogSettings.m_showTags = value;
			return true;
			break;
			case SHOW_MSG:
			m_defaultLogSettings.m_showMsg = value;
			return true;
			break;
			case SHOW_LOCAITON:
			m_defaultLogSettings.m_showLocation = value;
			return true;
			break;
		default:
			return false;
			break;
	}
	return false;
}



// Non-Member Helper Functions
std::string 	FormatTime		(const std::chrono::system_clock::time_point& time) 	noexcept {
	std::ostringstream os;

	auto localtime = std::chrono::system_clock::to_time_t(time);
	std::chrono::hh_mm_ss justForMS {std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch())};

	os << std::put_time(std::localtime(&localtime), "%H:%M:%S");
	os << ':' << std::setw(3) << std::setfill('0') << justForMS.subseconds().count();

	return os.str();
}
std::string     FormatLocation	(const std::source_location location, bool oneLine) 		noexcept {
	std::ostringstream os;

	if(!oneLine){
		os 	<< "File: " <<  location.file_name()
		<< "\nFunction: " << location.function_name() 
		<< "\nLine: " << location.line() 
		<< "\n";
	}
	else{
		os << "[" << location.file_name() <<" | " << location.function_name() << " | " << location.line() << "]";
	}

	return os.str();

}

bool    OpenLog::ChangeSettings(SETTINGS settings, uint16_t value) {
	return LogHandler::GetInstance().ChangeSettings(settings, value);
}
bool    OpenLog::ChangeSettings(SETTINGS settings, bool value) {
	return LogHandler::GetInstance().ChangeSettings(settings, value);
}

