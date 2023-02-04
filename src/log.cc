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

void LogFormatter::format(std::ostream& ofs, std::shared_ptr<LogEvent> event) {
    for(auto i : m_items) {
        i->format(ofs, event);
    }
}


class MessageFormatItem : public LogFormatter::FormatItem {
public:
    MessageFormatItem(const std::string& str = "") {}
        void format(std::ostream& ofs,std::shared_ptr<LogEvent> event) override{  
        ofs << event->getContent();
    }
};

class LevelFormatItem : public LogFormatter::FormatItem {
public:
    LevelFormatItem(const std::string& str = "") {}
        void format(std::ostream& ofs,std::shared_ptr<LogEvent> event) override{  
        ofs << LogLevel::ToString(event->getLevel());
    }
};

class ElapseFormatItem : public LogFormatter::FormatItem {
public:
    ElapseFormatItem(const std::string& str = "") {}
        void format(std::ostream& ofs,std::shared_ptr<LogEvent> event) override{  
        ofs << event->getElapse();
    }
};

class ThreadIdFormatItem : public LogFormatter::FormatItem {
public:
    ThreadIdFormatItem(const std::string& str = "") {}
        void format(std::ostream& ofs,std::shared_ptr<LogEvent> event) override{  
        ofs << event->getThreadId();
    }
};

class FiberIdFormatItem : public LogFormatter::FormatItem {
public:
    FiberIdFormatItem(const std::string& str = "") {}
        void format(std::ostream& ofs,std::shared_ptr<LogEvent> event) override{  
        ofs << event->getFiberId();
    }
};

class ThreadNameFormatItem : public LogFormatter::FormatItem {
public:
    ThreadNameFormatItem(const std::string& str = "") {}
        void format(std::ostream& ofs,std::shared_ptr<LogEvent> event) override{  
        ofs << event->getThreadName();
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

        void format(std::ostream& ofs,std::shared_ptr<LogEvent> event) override{  
        struct tm tm;
        time_t time = event->getTime();
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof(buf), m_format.c_str(), &tm);
        ofs << buf;
    }
private:
    std::string m_format;
};

class FilenameFormatItem : public LogFormatter::FormatItem {
public:
    FilenameFormatItem(const std::string& str = "") {}
        void format(std::ostream& ofs,std::shared_ptr<LogEvent> event) override{  
        // ofs << event->getFile();
    }
};

class LineFormatItem : public LogFormatter::FormatItem {
public:
    LineFormatItem(const std::string& str = "") {}
        void format(std::ostream& ofs,std::shared_ptr<LogEvent> event) override{  
        ofs << event->getLine();
    }
};

class NewLineFormatItem : public LogFormatter::FormatItem {
public:
    NewLineFormatItem(const std::string& str = "") {}
        void format(std::ostream& ofs,std::shared_ptr<LogEvent> event) override{  
        ofs << std::endl;
    }
};

class StringFormatItem : public LogFormatter::FormatItem {
public:
    StringFormatItem(const std::string& str)
        :m_string(str) {}
        void format(std::ostream& ofs,std::shared_ptr<LogEvent> event) override{  
        ofs << m_string;
    }
private:
    std::string m_string;
};

class TabFormatItem : public LogFormatter::FormatItem {
public:
    TabFormatItem(const std::string& str = "") {}
        void format(std::ostream& ofs,std::shared_ptr<LogEvent> event) override{  
        ofs << "\t";
    }
private:
    std::string m_string;
};


LogAppender::~LogAppender() {}

StdoutLogAppender::~StdoutLogAppender() {}

void StdoutLogAppender::log(std::shared_ptr<LogEvent> event){
    m_formatter->format(std::cout, event);
}


// const char* getFile() const { return m_file;}
int32_t LogEvent::getLine() const { return m_line;}
uint32_t LogEvent::getElapse() const { return m_elapse;}
uint32_t LogEvent::getThreadId() const { return m_threadId;}
uint32_t LogEvent::getFiberId() const { return m_fiberId;}
uint64_t LogEvent::getTime() const { return m_time;}
const std::string& LogEvent::getThreadName() const { return m_threadName;}
std::string LogEvent::getContent() const { return m_ss.str();}
LogLevel::Level LogEvent::getLevel() const { return m_level;}
std::stringstream& LogEvent::getSS() { return m_ss;}
void LogEvent::log(LogEvent::ptr event) {
    for(auto appender : m_appenders) {
        appender.second->log(event);
    }
}


Logger::Logger(std::string name):m_name(name){}

void Logger::log() {
    while (1)   {
        for(auto e : m_events) {
            if(e.second->getLevel() >= m_level) {
                e.second->log(e.second);
            }
        }
    }
}

void Logger::registEvent(LogEvent::ptr event){
    std::unordered_map<std::string, LogEvent::ptr>::const_iterator events = m_events.find(event->getName());
    if(events != m_events.end())       {
        m_events[event->getName()],event;
        
    } else {
        
    }
}

void Logger::delEvent(LogEvent::ptr event){
    m_events.erase(event->getName());
}

void Logger::clearEvent(){  m_events.clear(); }

void Logger::setLevel(LogLevel::Level level){
    m_level = level;
}

std::string Logger::getLevel(){
    return LogLevel::ToString(m_level);
}


}