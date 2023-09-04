#include "LogHandler.h"


LogHandler::LogHandler() {

}
LogHandler& LogHandler::GetInstance() noexcept{
	static LogHandler instance;

	return instance;
}


bool LogHandler::AddActiveLogTarget	(const std::string  key)		{
	try {
		if (m_activeLogTargets.size() < m_maxActiveLogTargets) {
			m_activeLogTargets.push_back(m_logTargets.at(key).get());
			return true;
		}
		else {
			return false;
		}
	}
	catch(...){
		return false;
	}
}
bool LogHandler::RemoveActiveLogTarget(const std::string key) {
	bool success{ false };

	int i{ 0 };
	for (LogTarget* c_tgt : m_activeLogTargets) {
		if (c_tgt) {
			if (c_tgt->str() == key) {
				m_activeLogTargets[i] = nullptr;
				success = true;
			}
		}
		++i;
	}
	return success;
}

bool 			LogHandler::Log				(const OpenLog::Log& log){
	bool success {false};

	if(m_activeLogTargets.size() <= 0) { return false; }

	for(auto& currentLogTarget : m_activeLogTargets){
		if(currentLogTarget){
			success = currentLogTarget->LogEvent(log); // TODO counting of how many Log calls returned false and able to report back if needed			
		}
	}
	return success;
}

void LogHandler::registerTag(std::unique_ptr<Tag>& tag) {
	m_registerdTags.try_emplace(tag->str(), std::move(tag));
}
Tag* LogHandler::getTag(const std::string& key) const{
	if (containsTag(key)) {
		return m_registerdTags.at(key).get();
	}
	else
		return nullptr;
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

// Settings
bool LogHandler::ChangeSettings(SETTINGS settings, uint16_t value){
	switch(settings){
			case CODE_TEXT_WIDTH:
				m_settings.m_widthOfCodeTextBox = value;
			return true;
			break;
			case LOG_MSG_MAX_SIZE:
				m_settings.m_logMsgMaxSize = value;
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
				m_settings.m_showTime = value;
			return true;
			break;
		case SHOW_TAGS:
				m_settings.m_showTags = value;
			return true;
			break;
			case SHOW_MSG:
				m_settings.m_showMsg = value;
			return true;
			break;
			case SHOW_LOCAITON:
				m_settings.m_showLocation = value;
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

	return std::format("{:%OH:%OM:%OS}", time);
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

