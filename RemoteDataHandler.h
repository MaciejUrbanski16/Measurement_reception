//
// Created by Admin on 03.03.2022.
//

#ifndef SERVER_FOR_TRACKER_REMOTEDATAHANDLER_H
#define SERVER_FOR_TRACKER_REMOTEDATAHANDLER_H


#include <array>
#include <cstdint>
#include <utility>

constexpr static unsigned bufferSize{100u};

class RemoteDataHandler {
public:
    explicit RemoteDataHandler(const std::array<char, bufferSize> &buffer) :
    buffer(buffer)
    {
        calculateAzimut();
        calculateXAcceleration();
        calculateYAcceleration();
        calculateZAcceleration();
        calculateTimeIntervalMs();
    }

    void extractDataFromJson();

    RemoteDataHandler(std::string  loadedDataAsJson) :
    loadedDataAsJson(std::move(loadedDataAsJson)){
        extractDataFromJson();
    }

    float getAzimut() const;
    int32_t getXAcceleration() const;
    int32_t getYAcceleration() const;
    int32_t getZAcceleration() const;
    int32_t getTimeIntervalMs() const;

    //this need some refactor
    float getAzimutWebServer() const;
    int32_t getXAccelerationWebServer() const;
    int32_t getYAccelerationWebServer() const;
    int32_t getZAccelerationWebServer() const;

private:
    std::array<char, bufferSize> buffer{};
    std::string loadedDataAsJson;

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

    float azimutWebServer{};
    int32_t xAccelerationWebServer{};
    int32_t yAccelerationWebServer{};
    int32_t zAccelerationWebServer{};
    bool isAzimutValid(int incomingAzimut);
};


#endif //SERVER_FOR_TRACKER_REMOTEDATAHANDLER_H
