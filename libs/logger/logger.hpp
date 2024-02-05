#pragma once
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>

namespace NLogger {
    class TLogHelper {
    public:

        TLogHelper() {
            out << "INFO TIME: " << std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() }
                << " Thread id: " << std::this_thread::get_id()
                << " Message: ";
        }
        
        ~TLogHelper() {
            std::cerr << out.str();
        }

        template<class T>
        friend TLogHelper&& operator<<(TLogHelper&& out, const T& mes) {
            out.out << mes;
            return std::move(out);
        }

    private:

        std::ostringstream out;
    };
}


#define INFO_LOG NLogger::TLogHelper{}
#define DEBUG_LOG NLogger::TLogHelper{}
#define ERROR_LOG NLogger::TLogHelper{}
#define Endl '\n'