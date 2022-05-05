#ifndef SERVER_FOR_TRACKER_MANUALMEASUREMENTS_H
#define SERVER_FOR_TRACKER_MANUALMEASUREMENTS_H

#include <string>

class ManualMeasurements {
public:
    void clearMeasurements() {
        startTime = "";
        stopTime = "";
        totalTime = "";
        distance = "";
        averagedVelocity = "";
        distanceInPeriod = "";
    }

    bool saveCsv() const{
        return true;
    }

    std::string startTime{};
    std::string stopTime{};
    std::string totalTime{};
    std::string distance{};
    std::string averagedVelocity{};
    std::string distanceInPeriod{};

    std::chrono::steady_clock::time_point start;

private:
    //CsvSaver / Reader(reader if needed should return array of manualMeasurements)
};


#endif //SERVER_FOR_TRACKER_MANUALMEASUREMENTS_H
