//
// Created by Admin on 05.03.2022.
//

#include <iostream>
#include "VelocityCalculator.h"

long long VelocityCalculator::getActualVelocity() const {
    return actualVelocity;
}

void VelocityCalculator::calculateActualVelocity(const long long xAcc, const uint16_t timeIntervalMs = 1u) {
    //basic calculation of velocity
    const float timeIntervalSec = static_cast<float>(timeIntervalMs) / 1000.0f;
    const auto actualSample{timeIntervalSec * xAcc};
    actualVelocity += actualSample;
   std::cout<<"Timer interval s: "<< timeIntervalSec <<"actaulSample: " <<actualSample <<std::endl;
}
