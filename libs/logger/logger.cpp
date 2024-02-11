#include <logger.hpp>

namespace NLogger { 

    unsigned char ChangeLogLevel(unsigned char logLevel) {
        static unsigned char UserLogLevel = 1;
        if (logLevel != 0)
            UserLogLevel = logLevel;
        return UserLogLevel;
    }

    int GetUserLogLevel() {
        return ChangeLogLevel(0);
    }

}
