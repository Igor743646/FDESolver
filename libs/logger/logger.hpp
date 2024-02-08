#pragma once
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

static unsigned char UserLogLevel = 0;

namespace NLogger {

    template<unsigned char LogLevel = 0>
    class TLogHelper {
    public:

        static_assert(LogLevel == 0 || LogLevel == 1 || LogLevel == 2);

        TLogHelper(const char* name, const char* color) {
            if (LogLevel <= UserLogLevel)
                out << "[ " << color << name << "\033[0m ]\t"
                    << " TIME: " << std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() }
                    << " Thread id: " << std::this_thread::get_id()
                    << " Message: ";
        }

        TLogHelper& operator=(const TLogHelper&) = delete;
        TLogHelper& operator=(TLogHelper&&) = delete;
        
        ~TLogHelper() {
            if (LogLevel <= UserLogLevel)
                std::cerr << out.str();
        }

        static void ChangeLogLevel(unsigned char logLevel) {
            UserLogLevel = logLevel;
        }

        template<class T>
        friend TLogHelper&& operator<<(TLogHelper&& out, const T& mes) {
            if (LogLevel <= UserLogLevel)
                out.out << mes;
            return std::move(out);
        }

    private:

        std::ostringstream out;
    };
}


#define INFO_LOG NLogger::TLogHelper<1>{"INFO", "\033[1;93m"}
#define DEBUG_LOG NLogger::TLogHelper<2>{"DEBUG", "\033[1;94m"}
#define ERROR_LOG NLogger::TLogHelper<0>{"ERROR", "\033[1;91m"}
#define Endl '\n'