#ifndef SERVER_FOR_TRACKER_VELOCITYCALCULATOR_H
#define SERVER_FOR_TRACKER_VELOCITYCALCULATOR_H


#include <cstdint>

class VelocityCalculator {
public:
    void calculateActualVelocity(float xAcc, uint16_t timeIntervalMs);
    float getActualVelocity() const;
private:
    float actualVelocity{0.f};
    float previousxAcc{0.f};
};


#endif //SERVER_FOR_TRACKER_VELOCITYCALCULATOR_H
