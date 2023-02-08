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
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <ostream>
#include <map>
#include <syscall.h>
#include "singleton.h"
#include "thread.h"
#include "utility.h"

#define RLEAVRS_LOG(level, logger) \
    if(level > logger->getLevel()) \ 
        rleavrs::LogEventWrap(rleavrs::LogEvent::ptr(new rleavrs::LogEvent(     \
            logger, level, __FILE__, __LINE__, 0,   \
            rleavrs::GetThreadId(), rleavrs::Fiber::GetFiberId(),  \
            time(0), rleavrs::Thread::GetName()))).getSS()

#define RLEAVRS_LOG_INFO(logger)    RLEAVRS_LOG(rleavrs::LogLevel::Level::INFO,logger)
#define RLEAVRS_LOG_DEBUG(logger)    RLEAVRS_LOG(rleavrs::LogLevel::Level::DEBUG,logger)
#define RLEAVRS_LOG_WARN(logger)    RLEAVRS_LOG(rleavrs::LogLevel::Level::WARNNING,logger)
#define RLEAVRS_LOG_ERROR(logger)    RLEAVRS_LOG(rleavrs::LogLevel::Level::ERROR,logger)
#define RLEAVRS_LOG_FATAL(logger)    RLEAVRS_LOG(rleavrs::LogLevel::Level::FATAL,logger)


#define RLEAVRS_LOG_ROOT()  rleavrs::LoggerMgr::GetInstance()->getRoot()
#define RLEAVRS_LOG_NAME(name)  rleavrs::LoggerMgr::GetInstance()->getLogger(name)

namespace rleavrs {

class LogEvent;
class LogAppender;
class Logger;
class LoggerManage;

class LogLevel {
public:
    enum Level {
        INFO = 1,
        DEBUG = 2,
        WARNNING = 3,
        ERROR = 4,
        FATAL = 5
    };
    static const char* ToString(LogLevel::Level level);
};

class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    explicit LogEvent(std::shared_ptr<Logger> logger,LogLevel::Level level,
                        const char* file,
                        int32_t line,
                        int32_t elapse,
                        uint32_t thread_id,
                        uint32_t fiber_id,
                        uint64_t time,
                        const std::string& thread_name);

    const char*  getFile() const;
    int32_t getLine() const;
    uint32_t getElapse() const;
    uint32_t getThreadId() const;
    uint32_t getFiberId() const;
    uint64_t getTime() const;
    std::shared_ptr<Logger> getLogger();
    const std::string& getThreadName() const;
    std::string getContent() const;
    LogLevel::Level getLevel() const; 
    std::stringstream& getSS();
    void setLevel(LogLevel::Level);


    void format(const char* fmt, ...);
    void format(const char* fmt, va_list al);

private:
    std::shared_ptr<Logger> m_logger;
    LogLevel::Level m_level;
    const char* m_file = nullptr;
    int32_t m_line = 0;
    int32_t m_elapse = 0;
    uint32_t m_threadId = 0;
    uint32_t m_fiberId = 0;
    uint64_t m_time = 0;
    std::string m_thread_name;
    std::stringstream m_ss;
        

};

class LogEventWrap{
public:
    LogEventWrap(LogEvent::ptr e);
    ~LogEventWrap();
    LogEvent::ptr getEvent() const { return m_event; }
    std::stringstream& getSS();
    
private:
    LogEvent::ptr m_event;
    
};

class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;    

    LogFormatter(std::string pattern);

    void init();

    std::ostream& format(std::ostream& os, LogEvent::ptr event);
    
    std::string format(LogEvent::ptr event);

    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem>  ptr;
        
        virtual std::ostream& format(std::ostream& ofs, LogEvent::ptr event) = 0;
    };

private:
    std::vector<FormatItem::ptr> m_items;
    std::string m_pattern;
};

class LogAppender {
public:
   typedef std::shared_ptr<LogAppender> ptr;
   
   virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;
};

class StdoutAppender : public LogAppender { 
public:
    typedef std::shared_ptr<StdoutAppender> ptr;
    
    void log(LogLevel::Level level,LogEvent::ptr event) override;
};

class Logger : public std::enable_shared_from_this<Logger> {
public:
    friend LoggerManage;
    typedef std::shared_ptr<Logger> ptr;
    Logger(std::string name = "root");
    std::string getName();
    LogLevel::Level getLevel();
    void setLevel(LogLevel::Level level);
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    void clearAppender();
    void setFormat(LogFormatter::ptr fmt);
    void setFormat(const std::string& val);    
    LogFormatter::ptr getFormat();
    void log(LogLevel::Level level,LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void debug(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

private:    
    std::string m_name;
    LogLevel::Level m_level = LogLevel::Level::INFO;
    LogFormatter::ptr m_formatter;
    std::unordered_set<LogAppender::ptr> m_appenders;
    Logger::ptr m_root;
};

class LoggerManage {
public:
    LoggerManage();
    void init();
    Logger::ptr getRoot() { return m_root;}
    Logger::ptr getLogger(const std::string &name);

private:
    Logger::ptr m_root;
    std::map<std::string, Logger::ptr> m_logger;
};

typedef rleavrs::Singleton<LoggerManage> LoggerMgr;


}

#endif 