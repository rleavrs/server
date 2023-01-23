#ifndef __RLEAVRS_LOG_H__
#define __RLEAVRS_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <iostream>

namespace RLEAVRS{

// Level
enum class LogLevel {
        INFO = 1,
        DEBUG = 2,
        WARNNING = 3,
        ERROR = 4,
        FATAL = 5
};

// Event
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();

private:
    const char* m_file = nullptr;
    int32_t m_line;
    uint32_t m_elapse = 0;
    uint32_t m_threadId = 0;
    uint32_t m_fiberId = 0;
    uint64_t m_time;
    std::string m_content;

};

class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter();
    std::string format(LogEvent::ptr event);

};

// Log Output 
class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;
    
    LogAppender();

    virtual ~LogAppender() {};

    virtual void log(LogLevel level,LogEvent::ptr event) = 0;

    void setFormatter(LogFormatter::ptr formatter);

protected:
    LogLevel m_level;
    LogFormatter::ptr m_formatter;

};

class StdoutLogAppender: public LogAppender {
public:
    
    typedef std::shared_ptr<StdoutLogAppender> ptr;

    void log(LogLevel level,LogEvent::ptr event) override;

};

class FileLogAppender: public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;

    FileLogAppender(const std::string& name);

    void log(LogLevel level,LogEvent::ptr event) override;

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
    
    void setLevel(LogLevel level);
    
    LogLevel getLevel();


    void addAppender(LogAppender::ptr appender);
    
    void delAppender(LogAppender::ptr appender);


    void Log(LogLevel level, LogEvent::ptr &event);
    
    void info(LogEvent::ptr &event);
    
    void debug(LogEvent::ptr &event);

    void warnning(LogEvent::ptr &event);
    
    void error(LogEvent::ptr &event);
    
    void fatal(LogEvent::ptr &event);


private:
    std::string m_name;
    LogLevel m_level;
    std::list<LogAppender::ptr> m_appenders;
    
};




}


#endif