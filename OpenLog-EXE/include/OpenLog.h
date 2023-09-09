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
        SHOW_LOCATION = 3,
        TAG_TEXT_WIDTH = 4,
        LOG_MSG_MAX_SIZE = 5,
    };
    bool    changeSettings(SETTINGS settings, int value);
    bool    changeSettings(SETTINGS settings, bool value);




     class Tag {
     public:
         Tag(const std::string tag="TAG");
         virtual ~Tag();

         const std::string str() const noexcept;
         friend std::ostream& operator<<(std::ostream& os, const Tag& tag);

     private:
         std::string m_tag;
     };

     void registerTag       (Tag& tag);
     bool tagIsRegistered   (const std::string& key);
     Tag* getRegisteredTag  (const std::string& key);





    class Log{
    public:
        Log(    const std::string msg,
                const std::string tag ="",
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

    bool log                 (const Log& log);
    bool log                 (const std::ostringstream msg,  const std::string tag = "", const std::source_location location = std::source_location::current());
    bool log                 (const std::string msg,         const std::string tag = "", const std::source_location location = std::source_location::current());

    std::string defaultFormatLog    (const Log& log);

    



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

    void    addNewLogTarget         (std::unique_ptr<LogTarget> target);
    bool    addActiveLogTarget      (const std::string target, const std::source_location location = std::source_location::current());
    bool    removeActiveLogTarget   (const std::string target, const std::source_location location = std::source_location::current());
    
}


#endif