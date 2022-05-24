#include <string>
#include <iostream>
#include <regex>
#include "RemoteDataHandler.h"

float RemoteDataHandler::getAzimut() const {
    return azimut;
}

float RemoteDataHandler::getXAccelerationMperS2() const {
    return static_cast<float>(xAcceleration) / 1000;
}

int32_t RemoteDataHandler::getYAccelerationMperS2() const {
    return yAcceleration / 1000.0f;
}

int32_t RemoteDataHandler::getZAccelerationMperS2() const {
    return zAcceleration / 1000.0f;
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
        std::cout << "xAcc " << xAccelerationAsString << "A" << static_cast<float>(xAcceleration) / 1000 << std::endl;
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

////////////////////////////////////

void RemoteDataHandler::extractDataFromJson() {

    std::cout<<loadedDataAsJson<<std::endl;
    std::regex fieldSearchingRegex("\\b[f][i][A-Za-z0-9\":\"]{2,19}\\b");
    std::regex fieldNrSearchingRegex("[1-4][\"]+");
    std::regex fieldValueSearchingRegex("\\b[0-9]{1,9}\\b");
    auto words_begin =
            std::sregex_iterator(loadedDataAsJson.begin(), loadedDataAsJson.end(), fieldSearchingRegex);
    auto words_end = std::sregex_iterator();

    std::cout << "Found "
              << std::distance(words_begin, words_end)
              << " words\n";

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        auto fieldNrBegin = std::sregex_iterator(match_str.begin(), match_str.end(), fieldNrSearchingRegex);
        auto fieldNrEnd = std::sregex_iterator();
        std::cout << "  " << match_str << "         ";
        for (std::sregex_iterator j = fieldNrBegin; j != fieldNrEnd; ++j) {
            std::smatch value = *j;
            std::string valueStr = value.str();
            std::cout<<"   " << valueStr;
        }
        auto valueBegin = std::sregex_iterator(match_str.begin(), match_str.end(), fieldValueSearchingRegex);
        auto valueEnd = std::sregex_iterator();
        for (std::sregex_iterator j = valueBegin; j != valueEnd; ++j) {
            std::smatch value = *j;
            std::string valueStr = value.str();
            std::cout<<"   " << valueStr;
        }
        std::cout<<std::endl;
    }
}
