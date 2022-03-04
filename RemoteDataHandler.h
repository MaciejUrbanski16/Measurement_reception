//
// Created by Admin on 03.03.2022.
//

#ifndef SERVER_FOR_TRACKER_REMOTEDATAHANDLER_H
#define SERVER_FOR_TRACKER_REMOTEDATAHANDLER_H


#include <array>
#include <cstdint>

constexpr static unsigned bufferSize{25u};

class RemoteDataHandler {
public:
    explicit RemoteDataHandler(const std::array<char, bufferSize> &buffer) : buffer(buffer)
    {
        calculateAzimut();
        calculateXAcceleration();
        calculateYAcceleration();
        calculateZAcceleration();
        calculateTimeIntervalMs();
    }

    uint16_t getAzimut() const;
    uint16_t getXAcceleration() const;
    uint16_t getYAcceleration() const;
    uint16_t getZAcceleration() const;
    uint16_t getTimeIntervalMs() const;

private:
    std::array<char, bufferSize> buffer{};

    void calculateAzimut();
    void calculateXAcceleration();
    void calculateYAcceleration();
    void calculateZAcceleration();
    void calculateTimeIntervalMs();

    uint16_t azimut{};
    uint16_t xAcceleration{};
    uint16_t yAcceleration{};
    uint16_t zAcceleration{};
    uint16_t timeIntervalMs{};
    uint16_t actualIndexProcessing;
};


#endif //SERVER_FOR_TRACKER_REMOTEDATAHANDLER_H
