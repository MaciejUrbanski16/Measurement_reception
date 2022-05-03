#ifndef SERVER_FOR_TRACKER_TIMEFORMATTER_H
#define SERVER_FOR_TRACKER_TIMEFORMATTER_H

#include <time.h>
#include <string>

class TimeFormatter {
public:
    static std::string getCurrentTimeAsString();

    static std::string getTotalTimeAsString(uint32_t ms);
};

#endif //SERVER_FOR_TRACKER_TIMEFORMATTER_H
