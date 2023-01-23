#ifndef __RLEAVRS_LOG_H__
#define __RLEAVRS_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <functional>

namespace RLEAVRS{

// Level
class LogLevel {
public:
    enum Level{
        INFO = 1,
        DEBUG = 2,
        WARNNING = 3,
        ERROR = 4,
        FATAL = 5
    };
    static const char* ToString(Level level);

};

// Event
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();
    const char* getFile() const;
    int32_t getLine() const;
    uint32_t getElapse() const;
    uint32_t getThreadId() const;
    uint32_t getFiberId() const;
    uint64_t getTime() const;
    const std::string& getContent() const;

private:
    const char* m_file = nullptr;
    int32_t m_line = 0;
    uint32_t m_elapse = 0;
    uint32_t m_threadId = 0;
    uint32_t m_fiberId = 0;
    uint64_t m_time = 0;
    std::string m_content;
    LogLevel::Level m_level;

};

class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    
    void init();

    LogFormatter();
    
    std::string format(LogEvent::ptr event);

    

public: 
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        virtual ~FormatItem() = 0;
        virtual void format(std::ostream& os,LogEvent::ptr event) = 0;
    };

private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
    bool m_error;

};

// Log Output 
class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;
    
    LogAppender();

    virtual ~LogAppender() {};

    virtual void log(LogLevel::Level level,LogEvent::ptr event) = 0;

    void setFormatter(LogFormatter::ptr formatter);

protected:
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;

};

class StdoutLogAppender: public LogAppender {
public:
    
    typedef std::shared_ptr<StdoutLogAppender> ptr;

    void log(LogLevel::Level level,LogEvent::ptr event) override;

};

class FileLogAppender: public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;

    FileLogAppender(const std::string& name);

    void log(LogLevel::Level Level,LogEvent::ptr event) override;

    bool reopen();

private:
    std::string m_filename;

    std::ofstream m_filestream;
};



// Logger
class Logger{
public:
    typedef std::shared_ptr<Logger> ptr;
    
    Logger(const std::string &name = "root");
    
    void setLevel(LogLevel::Level level);
    
    LogLevel::Level getLevel();


    void addAppender(LogAppender::ptr appender);
    
    void delAppender(LogAppender::ptr appender);


    void Log(LogLevel::Level level, LogEvent::ptr &event);
    
    void info(LogEvent::ptr &event);
    
    void debug(LogEvent::ptr &event);

    void warnning(LogEvent::ptr &event);
    
    void error(LogEvent::ptr &event);
    
    void fatal(LogEvent::ptr &event);


private:
    std::string m_name;
    LogLevel::Level m_level;
    std::list<LogAppender::ptr> m_appenders;
    
};

}


#endif