#ifndef OPEN_LOG
#define OPEN_LOG

#include <source_location>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <memory>
#include <chrono>

// Testing commiting part 2

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
                const std::vector<Tag> tags,
                const std::source_location location=std::source_location::current(), 
                const std::chrono::time_point<std::chrono::system_clock> timestamp=std::chrono::system_clock::now()
        );
        
        virtual ~Log() = default;

        const std::string 											m_msg;
        const std::source_location									m_location;
        const std::chrono::time_point<std::chrono::system_clock> 	m_timestamp;
              std::vector<Tag>                                      m_tags;
    };
    

    class LogFilter {
    public:
        LogFilter       ( const std::string name);
        virtual ~LogFilter   ();

        void AddTag		( const Tag& tag );

        bool RemoveTag  ( const std::string key  );
        bool IsAllowed  ( const std::string tag  ) const;

        virtual const std::string str() const noexcept final;
        friend std::ostream& operator<<(std::ostream& os, const LogFilter& lc);

    private:
        std::string m_name { "FILTER" };
        std::unordered_map<std::string, const Tag> m_allowedTags;
    };
    
    class LogTarget{
    public:
        LogTarget(const std::string name);
        virtual ~LogTarget();
    
        virtual const std::string str() const noexcept final;        
        virtual bool LogEvent(const Log& log);

        friend std::ostream& operator<<(std::ostream& os, const LogFilter& lc);

    private:
        const std::string m_name {"LOG TARGET"};
    };

    
    bool init();

    // Settings Interaction
    bool    SetLogFilter            (const std::string filter,  const std::source_location location=std::source_location::current());
    bool    AddActiveLogTarget	    (const std::string target,  const std::source_location location=std::source_location::current());
    bool    RemoveActiveLogTarget   (const std::string target,  const std::source_location location=std::source_location::current());
    bool    ChangeSettings          (SETTINGS settings, uint16_t value);
    bool    ChangeSettings          (SETTINGS settings, bool value);

    // LogTarget and LogFilter Interaction
    void        AddLogFilter    (const LogFilter& filter);
    void        AddLogTarget    (const LogTarget& target);

    LogFilter&  GetLogFilter    (const std::string key);
    LogTarget&  GetLogTarget    (const std::string key);



    // Logging
    bool        log             (const std::string msg, const std::vector<Tag>          tag,    const std::source_location location = std::source_location::current());
    bool        log             (const std::string msg, const std::vector<std::string>  tag,    const std::source_location location = std::source_location::current());

    std::string ThrowMSG        (const std::string msg, const std::source_location location=std::source_location::current());

    // Use Defualt Log Formatting Options
    std::string     FormatLog           (const Log& log);

}


#endif