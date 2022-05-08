#ifndef SERVER_FOR_TRACKER_MANUALMEASUREMENTS_H
#define SERVER_FOR_TRACKER_MANUALMEASUREMENTS_H

#include <string>
#include <chrono>
#include <ostream>

class ManualMeasurements {
public:
    ManualMeasurements() = default;
    ManualMeasurements(const std::string& startTime, const std::string& stopTime, const std::string totalTime,
                       const std::string& distance, const std::string& averagedVelocity, const std::string& distanceInPeriod) :
            startTime(startTime),
            stopTime(stopTime),
            totalTime(totalTime),
            distance(distance),
            averagedVelocity(averagedVelocity),
            distanceInPeriod(distanceInPeriod)
    {}

    friend std::ostream& operator<<(std::ostream &outStream, const ManualMeasurements& manualMeasurements);

    void clearMeasurements() {
        startTime = "";
        stopTime = "";
        totalTime = "";
        distance = "";
        averagedVelocity = "";
        distanceInPeriod = "";
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

inline std::ostream &operator<<(std::ostream &outStream, const ManualMeasurements &manualMeasurements) {
    return outStream <<"\n///////////////////////\nStart time: " << manualMeasurements.startTime.c_str() << "\nStop time: " << manualMeasurements.stopTime.c_str() <<
                      "\nTotal time: " << manualMeasurements.totalTime.c_str() << "\nDistance: " <<manualMeasurements.distance.c_str() << "\nAveraged velocity: " <<
                      manualMeasurements.averagedVelocity.c_str() << "\nDistance in period: " <<manualMeasurements.distanceInPeriod.c_str() << "\nWasMeasurementsSaved: "
                      <<manualMeasurements.wasMeasurementsSaved << '\n' << "///////////////////////\n";
}

#endif //SERVER_FOR_TRACKER_MANUALMEASUREMENTS_H
