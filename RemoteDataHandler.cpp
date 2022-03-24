//
// Created by Admin on 03.03.2022.
//

#include <string>
#include <iostream>
#include "RemoteDataHandler.h"

float RemoteDataHandler::getAzimut() const {
    return azimut;
}

int32_t RemoteDataHandler::getXAcceleration() const {
    return xAcceleration;
}

int32_t RemoteDataHandler::getYAcceleration() const {
    return yAcceleration;
}

int32_t RemoteDataHandler::getZAcceleration() const {
    return zAcceleration;
}

int32_t RemoteDataHandler::getTimeIntervalMs() const {
    return timeIntervalMs;
}

void RemoteDataHandler::calculateAzimut() {
    std::string azimutAsString{};
    auto itr = std::find(buffer.begin(), buffer.end(), ' ');
    actualIndexProcessing = std::distance(buffer.begin(), itr);
    if(itr != buffer.end())
    {
        for(auto i{0}; i < actualIndexProcessing; i++)
        {
            azimutAsString += buffer.at(i);
        }
        int azimutIntegral = atoi(azimutAsString.c_str());
        if(isAzimutValid(azimutIntegral)) {
            azimut = static_cast<float>(azimutIntegral) / 10.0f;
            std::cout << "azimut " << azimutAsString << "A" << azimut << std::endl;
        }
        else{
            std::cerr<<"INCOMING AZIMUT: " << azimutIntegral <<" NOT VALID" << std::endl;
        }
    } else{
        std::cerr<<"THERE ARE NO SPACE IN THE INPUT STRING FROM REMOTE DEVICE!!!"<<std::endl;
    }
}

void RemoteDataHandler::calculateXAcceleration() {
    std::string xAccelerationAsString{};
    const auto itr = std::find(buffer.begin() + actualIndexProcessing + 1, buffer.end(), ' ');
    const uint16_t previousIndexProcessing{static_cast<uint16_t>(actualIndexProcessing)};
    actualIndexProcessing = std::distance(buffer.begin() + actualIndexProcessing, itr) + actualIndexProcessing;
    if(itr != buffer.end()) {
        for (auto i{previousIndexProcessing + 1}; i < actualIndexProcessing; i++) {
            xAccelerationAsString += buffer.at(i);
        }
        xAcceleration = atoi(xAccelerationAsString.c_str());
        std::cout << "xAcc " << xAccelerationAsString << "A" << xAcceleration << std::endl;
    } else{
        std::cerr<<"THERE ARE NO SPACE IN THE INPUT STRING FROM REMOTE DEVICE!!!"<<std::endl;
    }
}

void RemoteDataHandler::calculateYAcceleration() {
    std::string yAccelerationAsString{};
    const auto itr = std::find(buffer.begin() + actualIndexProcessing + 1, buffer.end(), ' ');
    const uint32_t previousIndexProcessing{actualIndexProcessing};
    actualIndexProcessing = std::distance(buffer.begin() + actualIndexProcessing, itr) + actualIndexProcessing;
    if(itr != buffer.end()) {
        for (auto i{previousIndexProcessing + 1}; i < actualIndexProcessing; i++) {
            yAccelerationAsString += buffer.at(i);
        }
        yAcceleration = atoi(yAccelerationAsString.c_str());
        std::cout << "yAcc " << yAccelerationAsString << "A" << yAcceleration << std::endl;
    } else{
        std::cerr<<"THERE ARE NO SPACE IN THE INPUT STRING FROM REMOTE DEVICE!!!"<<std::endl;
    }
}

void RemoteDataHandler::calculateZAcceleration() {
    std::string zAccelerationAsString{};
    const auto itr = std::find(buffer.begin() + actualIndexProcessing + 1, buffer.end(), ' ');
    const uint16_t previousIndexProcessing{static_cast<uint16_t>(actualIndexProcessing)};
    actualIndexProcessing = std::distance(buffer.begin() + actualIndexProcessing, itr) + actualIndexProcessing;
    if(itr != buffer.end()) {
        for (auto i{previousIndexProcessing + 1}; i < actualIndexProcessing; i++) {
            zAccelerationAsString += buffer.at(i);
        }
        zAcceleration = atoi(zAccelerationAsString.c_str());
        std::cout << "zAcc " << zAccelerationAsString << "A" << zAcceleration << std::endl;
    } else{
        std::cerr<<"THERE ARE NO SPACE IN THE INPUT STRING FROM REMOTE DEVICE!!!"<<std::endl;
    }
}

void RemoteDataHandler::calculateTimeIntervalMs() {
    std::string timeIntervalAsString{};
    const auto itr = std::find(buffer.begin() + actualIndexProcessing + 1,buffer.end(), ' ');
    const uint16_t previousIndexProcessing{static_cast<uint16_t>(actualIndexProcessing)};
    actualIndexProcessing = std::distance(buffer.begin() + actualIndexProcessing, itr) + actualIndexProcessing;
    if(itr != buffer.end()) {
        for (auto i{previousIndexProcessing + 1}; i < actualIndexProcessing; i++) {
            timeIntervalAsString += buffer.at(i);
        }
        timeIntervalMs = atoi(timeIntervalAsString.c_str());
    } else{
        std::cerr<<"THERE ARE NO SPACE IN THE INPUT STRING FROM REMOTE DEVICE!!!"<<std::endl;
    }
}

bool RemoteDataHandler::isAzimutValid(int incomingAzimut) {
    return incomingAzimut >= 0 && incomingAzimut <= 3600;
}
