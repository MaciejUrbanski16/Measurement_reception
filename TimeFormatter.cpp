#include <iostream>
#include <sstream>
#include "TimeFormatter.h"

std::string TimeFormatter::formatCurrentTimeToString() {
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
