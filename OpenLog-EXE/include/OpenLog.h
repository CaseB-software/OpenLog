#ifndef OPEN_LOG
#define OPEN_LOG

#include <source_location>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <chrono>


namespace OpenLog {

    enum SETTINGS {
        SHOW_TIME = 0,
        SHOW_TAGS = 1,
        SHOW_MSG = 2,
        SHOW_LOCAITON = 3,
        CODE_TEXT_WIDTH = 4,
        LOG_MSG_MAX_SIZE = 5,
    };

     class Tag {
     public:
         Tag(const std::string tag="TAG");
         virtual ~Tag();

         const std::string str() const noexcept;
         friend std::ostream& operator<<(std::ostream& os, const Tag& tag);

     private:
         std::string m_tag;
     };

    class Log{
    public:
        Log(    const std::string msg,
                const std::string tag,
                const std::source_location location=std::source_location::current(), 
                const std::chrono::time_point<std::chrono::system_clock> timestamp=std::chrono::system_clock::now()
        );
        virtual ~Log() = default;

        bool tag(const std::string& tag);

        const std::string 											m_msg;
        const std::source_location									m_location;
        const std::chrono::time_point<std::chrono::system_clock> 	m_timestamp;
              std::vector<std::string>                              m_tags;
    };
    
    
    class LogTarget{
    public:
        LogTarget(const std::string name="New Log Target");
        virtual ~LogTarget();
    
        virtual const std::string str() const noexcept final;        
        virtual bool LogEvent(const Log& log);

        friend std::ostream& operator<<(std::ostream& os, const LogTarget& lc);

    private:
        const std::string m_name;
    };

    void registerTag(std::unique_ptr<Tag>& tag);
    void registerTag(Tag& tag);
    
    bool init();

    // Settings Interaction
    bool    AddActiveLogTarget	    (const std::string target,  const std::source_location location=std::source_location::current());
    bool    RemoveActiveLogTarget   (const std::string target,  const std::source_location location=std::source_location::current());
    bool    ChangeSettings          (SETTINGS settings, uint16_t value);
    bool    ChangeSettings          (SETTINGS settings, bool value);

    // LogTarget Interaction
    void        AddLogTarget        (std::unique_ptr<LogTarget> target);

    // Logging
    bool        log(std::string msg, std::string tag, const std::source_location location = std::source_location::current());

    std::string ThrowMSG        (const std::string msg, const std::source_location location=std::source_location::current());

    // Use Defualt Log Formatting Options
    std::string     FormatLog           (const Log& log);

}


#endif