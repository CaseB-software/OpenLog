#include "Application.h"

namespace OpenLog {

	Application::Application() {

	}
	Application& Application::getInstance() noexcept {
		static Application instance;

		return instance;
	}

	std::string Application::version()		const {
		return m_VERSION;
	}
	std::string Application::compiledDate()	const {
		return m_COMPILED_DATE;
	}


	bool Application::addActiveLogTarget(const std::string  key) {
		try {
			if (m_activeLogTargets.size() < m_maxActiveLogTargets) {
				m_activeLogTargets.push_back(m_logTargets.at(key).get());
				return true;
			}
			else {
				return false;
			}
		}
		catch (...) {
			return false;
		}
	}
	bool Application::removeActiveLogTarget(const std::string key) {
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
	std::ostringstream Application::getAllActiveLogTargets() const {
		std::ostringstream os;
		for (auto& lt : m_activeLogTargets) {
			os << ' [' << lt->str() << ']';
		}
		return os;
	}
	LogTarget* Application::getLogTarget(const std::string key) const {
		if (m_logTargets.contains(key)) {
			return m_logTargets.at(key).get();
		}
		else
			return nullptr;
	}




	bool 			Application::log(const OpenLog::Log& log) const {
		bool success{ false };

		std::chrono::system_clock::time_point adjustedTime{ log.timestamp() + std::chrono::hours(m_settings.timeOffset)};

		Log revisedLog{ std::string{log.msg()}, "", log.location(), adjustedTime };
		for (auto& tag : log.tags()) {
			revisedLog.addTag(tag);
		}

		if (m_activeLogTargets.size() <= 0) { return false; }

		for (auto& currentLogTarget : m_activeLogTargets) {
			if (currentLogTarget) {
				success = currentLogTarget->LogEvent(revisedLog); // TODO counting of how many Log calls returned false and able to report back if needed			
			}
		}
		return success;
	}

	void Application::registerTag(std::unique_ptr<Tag>& tag) {
		m_registerdTags.try_emplace(tag->str(), std::move(tag));
	}
	Tag* Application::getTag(const std::string& key) const {
		if (containsTag(key)) {
			return m_registerdTags.at(key).get();
		}
		else
			return nullptr;
	}



	// Log Target
	void		Application::registerLogTarget(std::unique_ptr<LogTarget> target) {
		m_logTargets.try_emplace(target->str(), std::move(target));
	}
	bool		Application::removeLogTarget(const std::string key) {
		try {
			auto& foundTarget{ m_logTargets.at(key) };		// Reference to the logCode if found
			foundTarget.release();							// Delete LogCode
			m_logTargets.erase(key);
			return true;
		}
		// If key could not be found, return false
		catch (...) {
			// The above throws out of range if not caught
			return false;
		}
	}

	// Settings
	bool Application::changeSettings(SETTINGS settings, int value) {
		switch (settings) {
		case TAG_TEXT_WIDTH:
			m_settings.widthOfTagTextBox = value;
			return true;
			break;
		case LOG_MSG_MAX_SIZE:
			m_settings.logMsgMaxSize = value;
			return true;
			break;
		case TIME_OFFSET:
			m_settings.timeOffset = value;
			return true;
			break;
		default:
			return false;
			break;
		}
		return false;
	}
	bool Application::changeSettings(SETTINGS settings, bool value) {
		switch (settings) {
		case SHOW_TIME:
			m_settings.showTime = value;
			return true;
			break;
		case SHOW_TAGS:
			m_settings.showTags = value;
			return true;
			break;
		case SHOW_MSG:
			m_settings.showMsg = value;
			return true;
			break;
		case SHOW_LOCATION:
			m_settings.showLocation = value;
			return true;
			break;
		default:
			return false;
			break;
		}
		return false;
	}



	// Non-Member Helper Functions
	std::string 	printTime(const std::chrono::system_clock::time_point& time) noexcept {
		return std::format("{:%OH:%OM:%OS}", time);
	}
	std::string     defaultFormatLocation(const std::source_location location, bool oneLine)	noexcept {
		std::ostringstream os;

		if (!oneLine) {
			os << "File: " << location.file_name()
				<< "\nFunction: " << location.function_name()
				<< "\nLine: " << location.line()
				<< "\n";
		}
		else {
			os << "[" << location.file_name() << " | " << location.function_name() << " | " << location.line() << "]";
		}

		return os.str();

	}

	bool    OpenLog::changeSettings(SETTINGS settings, int value) {
		return Application::getInstance().changeSettings(settings, value);
	}
	bool    OpenLog::changeSettings(SETTINGS settings, bool value) {
		return Application::getInstance().changeSettings(settings, value);
	}

}