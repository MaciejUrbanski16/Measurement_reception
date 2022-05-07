#ifndef SERVER_FOR_TRACKER_MANUALMEASUREMENTS_H
#define SERVER_FOR_TRACKER_MANUALMEASUREMENTS_H

#include <string>
#include <chrono>

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

    void setSavingStatusOfMeasurements(const bool wasMeasurementsSaved) noexcept
    {
        this->wasMeasurementsSaved = wasMeasurementsSaved;
    }

    bool wasAlreadyMeasurementsSaved() const noexcept
    {
        return wasMeasurementsSaved;
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
    bool wasMeasurementsSaved{false};
};


#endif //SERVER_FOR_TRACKER_MANUALMEASUREMENTS_H
