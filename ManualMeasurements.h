#ifndef SERVER_FOR_TRACKER_MANUALMEASUREMENTS_H
#define SERVER_FOR_TRACKER_MANUALMEASUREMENTS_H

#include <string>

class ManualMeasurements {
public:
    std::string startTime{};
    std::string stopTime{};
    std::string totalTime{};
    std::string distance{};

    std::chrono::steady_clock::time_point start;
};


#endif //SERVER_FOR_TRACKER_MANUALMEASUREMENTS_H
