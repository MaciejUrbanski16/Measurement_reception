//
// Created by Admin on 24.03.2022.
//

#ifndef SERVER_FOR_TRACKER_RELATIVEPOSITIONCALCULATOR_H
#define SERVER_FOR_TRACKER_RELATIVEPOSITIONCALCULATOR_H


#include <cstdint>
#include <utility>
#include <optional>

struct QuadraticEquationCoefficients
{
    double a;
    double b;
    double c;

    double x1;
    double x2;
    double y1;
    double y2;
};

class RelativePositionCalculator {
public:
    void calculateActualRelativePosition(const uint64_t velocity, const int timeIntervalMs, const float azimutDegree);
    void setPreviousRelativePosition(std::pair<double, double> previousRelativePosition);
    [[nodiscard]] std::pair<double, double> getCalculatedRelativePosition() const;
private:
    std::pair<double, double> calculatedRelativePosition{0.0, 0.0};
    std::pair<double, double> previousRelativePosition{0.0, 0.0};

    std::optional<std::pair<double, double>>
    calculateRelativePositionToActualPosition(const QuadraticEquationCoefficients &coefficients, const float d);

    void setNewCalculatedRelativePosition(const std::optional<std::pair<double, double>> relativePosition);

    std::optional<QuadraticEquationCoefficients> calculateEquationCoefficients(const double radius, const float degree);

    std::optional<std::pair<double, double>> calculateAcoefficient(const float degree);
    double invalidAzimutDegree{65535.0};
};


#endif //SERVER_FOR_TRACKER_RELATIVEPOSITIONCALCULATOR_H
