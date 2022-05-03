#include <iostream>
#include <sstream>
#include "TimeFormatter.h"

std::string TimeFormatter::getCurrentTimeAsString() {
    time_t timetoday;
    time(&timetoday);
    tm *local_time = localtime(&timetoday);
    std::stringstream timeForPerformingMeasurements;

    if(local_time->tm_hour < 10)
    {
        timeForPerformingMeasurements << "0" << std::to_string(local_time->tm_hour) << ":";
    } else{
        timeForPerformingMeasurements << std::to_string(local_time->tm_hour) << ":";
    }
    if(local_time->tm_min < 10)
    {
        timeForPerformingMeasurements << "0" << std::to_string(local_time->tm_min) << ":";
    } else{
        timeForPerformingMeasurements << std::to_string(local_time->tm_min) << ":";
    }
    if(local_time->tm_sec < 10)
    {
        timeForPerformingMeasurements << "0" << std::to_string(local_time->tm_sec);
    } else{
        timeForPerformingMeasurements << std::to_string(local_time->tm_sec);
    }

    return timeForPerformingMeasurements.str();
}

std::string TimeFormatter::getTotalTimeAsString(uint32_t durationMs) {
    std::stringstream durationConvertedToString;

    if(durationMs < 1000)
    {
        durationConvertedToString << "00:00:00." << durationMs % 1000;
    }
    else if(durationMs >= 1000 && durationMs < 60000)
    {
        if(durationMs < 10000)
        {
            durationConvertedToString << "00:00:0" << (durationMs  - durationMs % 1000) / 1000 << "." << durationMs % 1000;
        } else{
            durationConvertedToString << "00:00:" << (durationMs  - durationMs % 1000) / 1000 << "." << durationMs % 1000;
        }
    }
    else if(durationMs >= 60000 && durationMs < 3600000)
    {
        if(durationMs < 600000)
        {
            durationConvertedToString << "00:0" << (durationMs  - durationMs % 60000) / 60000 << ":"
            << (durationMs  - durationMs % 1000) / 1000 << "." << durationMs % 1000;
        } else{
            durationConvertedToString << "00:" << (durationMs  - durationMs % 60000) / 60000 << ":"
            << (durationMs  - durationMs % 1000) / 1000 << "." << durationMs % 1000;
        }
    }

    return durationConvertedToString.str();
}
