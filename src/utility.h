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



}

#endif