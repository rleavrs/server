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

#include "utility/singleton.h"


namespace rleavrs {
class LogEvent;
class LogAppender;
class Logger;

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

class LogFormatter {
public:
    friend LogAppender;


    typedef std::shared_ptr<LogFormatter> ptr;    

    void init();

    void format(std::ostream& ofs, std::shared_ptr<LogEvent> event);

    std::string format(std::shared_ptr<LogEvent> event);    

    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem>  ptr;
        
        virtual void format(std::ostream& ofs,std::shared_ptr<LogEvent> event) = 0;
    };

private:
    std::vector<FormatItem::ptr> m_items;

};

class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;

    virtual ~LogAppender() = 0;
    
    virtual void log(std::shared_ptr<LogEvent> event) = 0;

    virtual std::string getFileName() = 0;
protected:
    LogFormatter::ptr m_formatter;
};

class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;

    ~StdoutLogAppender() override;

    void log(std::shared_ptr<LogEvent> event) override;

    std::string getFileName()  { return m_fileName; }

private:

    const std::string m_fileName = "stdout";

};

class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;

    explicit FileLogAppender(const std::string& file):m_fileName(file) {}

    virtual void log(std::shared_ptr<LogEvent> event) override;

    std::string getFileName()  { return m_fileName; }
private:
    std::string m_fileName;

};

class LogEvent {
public:
    friend LogAppender;
    friend Logger;  
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent() {}
    LogEvent(std::string name,
                uint64_t time,
                uint32_t elapse,
                uint32_t line,
                uint32_t threadId,
                uint32_t fiberId,
                std::string threadName,
                std::string ss,
                LogLevel::Level level);

    std::string getName() const;
    int32_t getLine() const;
    uint32_t getElapse() const;
    uint32_t getThreadId() const;
    uint32_t getFiberId() const;
    uint64_t getTime() const;
    const std::string& getThreadName() const;
    std::string getContent() const;
    LogLevel::Level getLevel() const; 
    std::stringstream& getSS();
    std::unordered_set<std::string> getOutFile();

    void format(const char* fmt, ...);
    void format(const char* fmt, va_list al);
    bool isUpdate();
    void update();

private:
    bool m_update;
    std::string m_name = "root_event";
    uint64_t m_time = 0;
    uint32_t m_elapse = 0;
    uint32_t m_line = 0;
    uint32_t m_threadId = 0;
    uint32_t m_fiberId = 0;
    std::string m_threadName = "thread_name";
    std::stringstream m_ss;
    LogLevel::Level m_level = LogLevel::INFO;
    std::unordered_set<std::string> m_outFiles = {"stdout"};
};

class Logger  {
public:
    typedef std::shared_ptr<Logger> ptr;
    Logger(std::string name = "root");
    void registEvent(LogEvent::ptr event);
    void delEvent(LogEvent::ptr event);
    void clearEvent();
    std::string getLevel();
    void setLevel(LogLevel::Level level);
    void log();
private:    
    std::string m_name;
    LogLevel::Level m_level;
    std::unordered_map<LogAppender::ptr, std::unordered_set<LogEvent::ptr>> m_appenders;
    Logger::ptr m_root;
};

typedef rleavrs::Singleton<Logger> LoggerMgr;

}

#endif 