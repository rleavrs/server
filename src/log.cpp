#include "log.h"

namespace RLEAVRS {


void StdoutLogAppender::log(LogLevel level,LogEvent::ptr event) {
    if(level >= m_level) {
        std::cout << m_formatter->format(event);
    }
}
 
FileLogAppender::FileLogAppender(const std::string& name)
    :m_filename(name){    
}

bool FileLogAppender::reopen() {
    if(m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return !!m_filestream;
}

void FileLogAppender::log(LogLevel level,LogEvent::ptr event) {
    if(level >= m_level) {
        m_filestream << m_formatter->format(event);

    }
}


Logger::Logger(const std::string &name)
    :m_name(name) {
}

void Logger::addAppender(LogAppender::ptr appender) {
    m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {
    for(auto it = m_appenders.begin(); it != m_appenders.end(); it++) {
        if(*it == appender) {
            m_appenders.erase(it);
            break;
        }
    }
}


void Logger::Log(LogLevel level, LogEvent::ptr &event) {
    if(level >= m_level) {
        for(auto& it : m_appenders) {
            it -> log(level,event);
        }
    }
}


void Logger::info(LogEvent::ptr &event){
    Log(LogLevel::INFO,event);
}

void Logger::debug(LogEvent::ptr &event){
    Log(LogLevel::DEBUG,event);

}

void Logger::warnning(LogEvent::ptr &event){
    Log(LogLevel::WARNNING,event);

}

void Logger::error(LogEvent::ptr &event){
    Log(LogLevel::ERROR,event);

}

void Logger::fatal(LogEvent::ptr &event){
    Log(LogLevel::FATAL,event);
    
}
}