#include "log.h"

namespace rleavrs {
    
const char* LogLevel::ToString(LogLevel::Level level) {
    switch(level){ 
#define xx(name) \
    case LogLevel::Level::name:     \
        return #name;    
        
        xx(INFO);
        xx(DEBUG);
        xx(WARNNING);
        xx(ERROR);
        xx(FATAL);
#undef xx
    default:    return "UNKNOW LEVEL";
    }
}


LogEvent::LogEvent(Logger::ptr logger,
                LogLevel::Level level,
                const char* file,
                int32_t line,
                int32_t elapse,
                uint32_t thread_id,
                uint32_t fiber_id,
                uint64_t time,
                const std::string& thread_name)
                :m_logger(logger),
                m_level(level),
                m_file(file),
                m_line(m_line),
                m_elapse(elapse),
                m_threadId(thread_id),
                m_fiberId(fiber_id),
                m_time(time),
                m_thread_name(thread_name)  {}

const char*  LogEvent::getFile() const {return m_file;}

int32_t LogEvent::getLine() const { return m_line;}

uint32_t LogEvent::getElapse() const { return m_elapse;}

uint32_t LogEvent::getThreadId() const { return m_threadId;}

uint32_t LogEvent::getFiberId() const { return m_fiberId;}

uint64_t LogEvent::getTime() const { return m_time;}

const std::string& LogEvent::getThreadName() const { return m_thread_name;}

std::string LogEvent::getContent() const { return m_ss.str();}

LogLevel::Level LogEvent::getLevel() const { return m_level;}

std::stringstream& LogEvent::getSS() { return m_ss;}

Logger::ptr LogEvent::getLogger() { return m_logger; }

void LogEvent::setLevel(LogLevel::Level level) {
    m_level = level;
}

LogEventWrap::LogEventWrap(LogEvent::ptr e)
    :m_event(e) {}

LogEventWrap::~LogEventWrap() {
    m_event->getLogger()->log(m_event->getLevel(),m_event);
}

std::stringstream& LogEventWrap::getSS(){
    return m_event->getSS();
}

LogFormatter::LogFormatter(std::string pattern):m_pattern(pattern)  {}

void LogFormatter::init() {

}

std::ostream& LogFormatter::format(std::ostream& os, LogEvent::ptr event){
    // for(auto &i : m_items) {
    //     i->format(os,event);
    // }
    os << event->getContent();
    return os;
}

std::string LogFormatter::format(LogEvent::ptr event) {
    std::stringstream ss;
    for(auto &i : m_items) {
        i->format(ss, event);
    }
    return ss.str();
}

class MessageFormatItem : public LogFormatter::FormatItem {
public:
    MessageFormatItem(const std::string& str = "") {}
    std::ostream& format(std::ostream& os,LogEvent::ptr event) override{  
        os << event->getContent();
        return os;
    }
};

class LevelFormatItem : public LogFormatter::FormatItem {
public:
    LevelFormatItem(const std::string& str = "") {}
    std::ostream& format(std::ostream& os,LogEvent::ptr event) override{  
        os << LogLevel::ToString(event->getLevel());
        return os;
    }
};

class ElapseFormatItem : public LogFormatter::FormatItem {
public:
    ElapseFormatItem(const std::string& str = "") {}
    std::ostream& format(std::ostream& os,LogEvent::ptr event) override{  
        os << event->getElapse();
        return os;
    }
};

class ThreadIdFormatItem : public LogFormatter::FormatItem {
public:
    ThreadIdFormatItem(const std::string& str = "") {}
    std::ostream& format(std::ostream& os,LogEvent::ptr event) override{  
        os << event->getThreadId();
        return os;
    }
};

class FiberIdFormatItem : public LogFormatter::FormatItem {
public:
    FiberIdFormatItem(const std::string& str = "") {}
    std::ostream& format(std::ostream& os,LogEvent::ptr event) override{  
        os << event->getFiberId();
        return os;
    }
};

class ThreadNameFormatItem : public LogFormatter::FormatItem {
public:
    ThreadNameFormatItem(const std::string& str = "") {}
        std::ostream& format(std::ostream& os,LogEvent::ptr event) override{  
        os << event->getThreadName();
        return os;
    }
};

class LoggerNameFormatItem : public LogFormatter::FormatItem {
public:
    LoggerNameFormatItem(const std::string& str = "");
    std::ostream& format(std::ostream& os,LogEvent::ptr event) override {
        os << event->getLogger()->getName();
        return os;
    }
};

class DateTimeFormatItem : public LogFormatter::FormatItem {
public:
    DateTimeFormatItem(const std::string& format = "%Y-%m-%d %H:%M:%S")
        :m_format(format) {
        if(m_format.empty()) {
            m_format = "%Y-%m-%d %H:%M:%S";
        }
    }

    std::ostream& format(std::ostream& os,LogEvent::ptr event) override{  
        struct tm tm;
        time_t time = event->getTime();
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof(buf), m_format.c_str(), &tm);
        os << buf;
        return os;
    }
private:
    std::string m_format;
};

class FilenameFormatItem : public LogFormatter::FormatItem {
public:
    FilenameFormatItem(const std::string& str = "") {}
    std::ostream& format(std::ostream& os,LogEvent::ptr event) override{  
        os << event->getFile();
        return os;
    }
};

class LineFormatItem : public LogFormatter::FormatItem {
public:
    LineFormatItem(const std::string& str = "") {}
    std::ostream& format(std::ostream& os,LogEvent::ptr event) override{  
        os << event->getLine();
        return os;
    }
};

class NewLineFormatItem : public LogFormatter::FormatItem {
public:
    NewLineFormatItem(const std::string& str = "") {}
    std::ostream& format(std::ostream& os,LogEvent::ptr event) override{  
        os << std::endl;
        return os;
    }
};

class StringFormatItem : public LogFormatter::FormatItem {
public:
    StringFormatItem(const std::string& str)
        :m_string(str) {}
    std::ostream& format(std::ostream& os,LogEvent::ptr event) override{  
        os << m_string;
        return os;
    }
private:
    std::string m_string;
};

class TabFormatItem : public LogFormatter::FormatItem {
public:
    TabFormatItem(const std::string& str = "") {}
        std::ostream& format(std::ostream& os,LogEvent::ptr event) override{  
        os << "\t";
        return os;
    }
private:
    std::string m_string;
};

void StdoutAppender::log(LogLevel::Level level,LogEvent::ptr event) {
    event->getLogger()->getFormat()->format(std::cout, event);
}

Logger::Logger(std::string name):m_name(name){}

std::string Logger::getName() {
    return m_name;
}

LogLevel::Level Logger::getLevel(){
    return m_level;
}

void Logger::setLevel(LogLevel::Level level) {
    m_level = level;
}

void Logger::addAppender(LogAppender::ptr appender) {
    m_appenders.emplace(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {}

void Logger::clearAppender() {
    m_appenders.clear();
}

void Logger::setFormat(LogFormatter::ptr fmt) {
}

void Logger::setFormat(const std::string& val)    {

}

LogFormatter::ptr Logger::getFormat() {
    return m_formatter;
}

void Logger::log(LogLevel::Level level,LogEvent::ptr event) {
    event->setLevel(level);
    if(level >= m_level) {
        auto self = shared_from_this();
        if(!m_appenders.empty()) {
            for(auto &i : m_appenders) {
                i->log(level,event);
            }
        } else if(m_root) {
            m_root->log(level,event);
        }
    }
}

void Logger::info(LogEvent::ptr event) {
    log(LogLevel::Level::INFO, event);
}

void Logger::debug(LogEvent::ptr event) {
    log(LogLevel::Level::DEBUG, event);
}

void Logger::warn(LogEvent::ptr event) {
    log(LogLevel::Level::WARNNING, event);
}

void Logger::error(LogEvent::ptr event) {
    log(LogLevel::Level::ERROR, event);
}

void Logger::fatal(LogEvent::ptr event) {
    log(LogLevel::Level::FATAL, event);
}

LoggerManage::LoggerManage() {
    m_root.reset(new Logger);
    m_root->addAppender(LogAppender::ptr(new StdoutAppender));
    m_logger[m_root->m_name] = m_root;
    init();
}

void LoggerManage::init() {}

Logger::ptr LoggerManage::getLogger(const std::string &name) {
    auto it = m_logger.find(name);
    if(it != m_logger.end())    return it->second;
    
    Logger::ptr logger(new Logger(name));
    logger->m_root = m_root;
    m_logger[name] = logger;
    return logger;   
}




}