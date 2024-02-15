#pragma once
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

#ifdef __GNU__ 
    #define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#else
    #define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

namespace NLogger {

    unsigned char ChangeLogLevel(unsigned char logLevel);
    int GetUserLogLevel();

    template<unsigned char LogLevel = 0>
    class TLogHelper {
    public:

        static_assert(LogLevel == 1 || LogLevel == 2 || LogLevel == 3);

        TLogHelper(const char* name, const char* file, int line, const char* color) {
            if (LogLevel <= GetUserLogLevel())
                out << "[ " << color << std::setw(5) << name << "\033[0m ] " << std::setw(30) << file << "(" << line << ")"
                    << " Thread id: " << std::this_thread::get_id()
                    << " Message: ";
        }

        TLogHelper& operator=(const TLogHelper&) = delete;
        TLogHelper& operator=(TLogHelper&&) = delete;
        
        ~TLogHelper() {
            if (LogLevel <= GetUserLogLevel())
                std::cerr << out.str();
        }

        template<class T>
        friend TLogHelper&& operator<<(TLogHelper&& out, const T& mes) {
            if (LogLevel <= GetUserLogLevel())
                out.out << mes;
            return std::move(out);
        }

    private:

        std::ostringstream out;
    };
}


#define BASE_LOG(level, file, line, ...) NLogger::TLogHelper<level>{file, line, __VA_ARGS__}
#define INFO_LOG  BASE_LOG(2, "INFO", (__FILENAME__), (__LINE__), "\033[1;93m")
#define DEBUG_LOG BASE_LOG(3, "DEBUG", (__FILENAME__), (__LINE__), "\033[1;94m")
#define ERROR_LOG BASE_LOG(1, "ERROR", (__FILENAME__), (__LINE__), "\033[1;91m")
#define WARNING_LOG BASE_LOG(1, "WARN", (__FILENAME__), (__LINE__), "\033[38;5;207m")
#define Endl '\n'