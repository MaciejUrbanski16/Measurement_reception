#ifndef SERVER_FOR_TRACKER_VELOCITYCALCULATOR_H
#define SERVER_FOR_TRACKER_VELOCITYCALCULATOR_H


#include <cstdint>

class VelocityCalculator {
public:
    void calculateActualVelocity(long long xAcc, uint16_t timeIntervalMs);
    long long getActualVelocity() const;
private:
    long long actualVelocity{0};
    long long int previousxAcc{0};
};


#endif //SERVER_FOR_TRACKER_VELOCITYCALCULATOR_H
