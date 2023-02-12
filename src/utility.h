#ifndef __RLEAVRS_UTILITY_H__
#define __RLEAVRS_UTILITY_H__

#include <string>
#include <assert.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <unistd.h>
#include "log.h"
#include <execinfo.h>
#include <cxxabi.h>
#include <syscall.h>
#include "fiber.h"

#if defined __GNUC__ || defined __llvm__
#define RLEAVRS_LIKELY(x)     __builtin_expect(!!(x),1)    
#define RLEAVRS_UNLIKELY(x)   __builtin_expect(!!(x),0)
#else
#define RLEAVRS_LIKELY(x)     (x)    
#define RLEAVRS_UNLIKELY(x)   (x)
#endif

#define RLEAVRS_ASSERT(x) \
    if(RLEAVRS_UNLIKELY(!(x))) { \
        RLEAVRS_LOG_ERROR(RLEAVRS_LOG_ROOT()) << "ASSERTION: " #x \
            << "\nbacktrace:\n"    \ 
            << rleavrs::BacktraceToString(100,2 ,"    "); \
            assert(x); \
    }
            
#define RLEAVRS_ASSERT_W(x,w) \
    if(RLEAVRS_UNLIKELY(!(x))) { \
        RLEAVRS_LOG_ERROR(RLEAVRS_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w \
            << "\nbacktrace:\n" \
            << rleavrs::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

namespace rleavrs {


static std::string demangle(const char* str);
void Backtrace(std::vector<std::string>& bt, int size = 64, int skip = 1);
std::string BacktraceToString(int size, int skip, const std::string& prefix);
pid_t GetThreadId();
uint32_t GetFiberId();
uint64_t GetCurrentMS();
uint64_t GetCurrentUS();



class FSUtil {
public:
    static void ListAllFile(std::vector<std::string>& files
                            ,const std::string& path
                            ,const std::string& subfix);
    static bool Mkdir(const std::string& dirname);
    static bool IsRunningPidfile(const std::string& pidfile);
    static bool Rm(const std::string& path);
    static bool Mv(const std::string& from, const std::string& to);
    static bool Realpath(const std::string& path, std::string& rpath);
    static bool Symlink(const std::string& frm, const std::string& to);
    static bool Unlink(const std::string& filename, bool exist = false);
    static std::string Dirname(const std::string& filename);
    static std::string Basename(const std::string& filename);
    static bool OpenForRead(std::ifstream& ifs, const std::string& filename
                    ,std::ios_base::openmode mode);
    static bool OpenForWrite(std::ofstream& ofs, const std::string& filename
                    ,std::ios_base::openmode mode);
};

}

#endif