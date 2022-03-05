//
// Created by Admin on 03.03.2022.
//

#ifndef SERVER_FOR_TRACKER_REMOTEDATAHANDLER_H
#define SERVER_FOR_TRACKER_REMOTEDATAHANDLER_H


#include <array>
#include <cstdint>

constexpr static unsigned bufferSize{100u};

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

    float getAzimut() const;
    int32_t getXAcceleration() const;
    int32_t getYAcceleration() const;
    int32_t getZAcceleration() const;
    int32_t getTimeIntervalMs() const;

private:
    std::array<char, bufferSize> buffer{};

    void calculateAzimut();
    void calculateXAcceleration();
    void calculateYAcceleration();
    void calculateZAcceleration();
    void calculateTimeIntervalMs();

    float azimut{};
    int32_t xAcceleration{};
    int32_t yAcceleration{};
    int32_t zAcceleration{};
    int32_t timeIntervalMs{};
    uint32_t actualIndexProcessing;

    bool isAzimutValid(int incomingAzimut);
};


#endif //SERVER_FOR_TRACKER_REMOTEDATAHANDLER_H
