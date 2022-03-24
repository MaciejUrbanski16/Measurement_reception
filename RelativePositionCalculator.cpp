#include <cmath>
#include <iostream>
#include "RelativePositionCalculator.h"

void RelativePositionCalculator::calculateActualRelativePosition(const uint64_t velocity, const int timeIntervalMs,
                                                                 const float azimutDegree) {
    //y = tg(alpha) * x
    //x^2 + y^2 = r^2
    calculatedRelativePosition.first = previousRelativePosition.first;
    calculatedRelativePosition.second = previousRelativePosition.second;
    const uint64_t lengthOfRoadFromLastMeasurementTimeInterval = velocity * timeIntervalMs;
    const auto radius = pow(lengthOfRoadFromLastMeasurementTimeInterval,2);
    QuadraticEquationCoefficients quadraticEquationCoefficients = calculateEquationCoefficients(radius, azimutDegree);
    std::optional<std::pair<double, double>> relativePositionToReceivedPosition =
            calculateRelativePositionToActualPosition(quadraticEquationCoefficients, azimutDegree);
    setNewCalculatedRelativePosition(relativePositionToReceivedPosition);
}

std::pair<double, double> RelativePositionCalculator::getCalculatedRelativePosition() const {
    return calculatedRelativePosition;
}

void RelativePositionCalculator::setPreviousRelativePosition(std::pair<double, double> previousRelativePosition) {
    this->previousRelativePosition = previousRelativePosition;
}

QuadraticEquationCoefficients
RelativePositionCalculator::calculateEquationCoefficients(const double radius, const float azimutDegree) {
    const double a = std::tan(azimutDegree);
    QuadraticEquationCoefficients quadraticEquationCoefficients{1.0, a, radius*(-1.0)};
    const double delta = pow(quadraticEquationCoefficients.b, 2) - 4 * quadraticEquationCoefficients.a * quadraticEquationCoefficients.c;
    quadraticEquationCoefficients.x1 = (-quadraticEquationCoefficients.b - sqrt(delta)) / (2 * quadraticEquationCoefficients.a);
    quadraticEquationCoefficients.x2 = (-quadraticEquationCoefficients.b + sqrt(delta)) / (2 * quadraticEquationCoefficients.a);
    quadraticEquationCoefficients.y1 = quadraticEquationCoefficients.b * quadraticEquationCoefficients.x1;
    quadraticEquationCoefficients.y2 = quadraticEquationCoefficients.b * quadraticEquationCoefficients.x2;
    return quadraticEquationCoefficients;
}

std::optional<std::pair<double, double>>
RelativePositionCalculator::calculateRelativePositionToActualPosition(const QuadraticEquationCoefficients &coefficients,
                                                                      const float azimut) {
    std::pair<double, double> relativePosition{0.0, 0.0};
   if(azimut > 0.0 && azimut <= 90.0){
       coefficients.x1 > 0.0 ? relativePosition.first = coefficients.x1 :
                               relativePosition.first = coefficients.x2;
       coefficients.y1 > 0.0 ? relativePosition.second = coefficients.y1 :
                               relativePosition.second = coefficients.y2;
       return std::optional<std::pair<double, double>>(relativePosition);
   }
   else if(azimut > 90.0 && azimut <= 180.0){
       coefficients.x1 > 0.0 ? relativePosition.first = coefficients.x1 :
                               relativePosition.first = coefficients.x2;
       coefficients.y1 < 0.0 ? relativePosition.second = coefficients.y1 :
                               relativePosition.second = coefficients.y2;
       return std::optional<std::pair<double, double>>(relativePosition);
   }
   else if(azimut > 180.0 && azimut <= 270.0){
       coefficients.x1 < 0.0 ? relativePosition.first = coefficients.x1 :
                               relativePosition.first = coefficients.x2;
       coefficients.y1 < 0.0 ? relativePosition.second = coefficients.y1 :
                               relativePosition.second = coefficients.y2;
       return std::optional<std::pair<double, double>>(relativePosition);
   }
   else if(azimut > 270.0 && azimut <= 360.0){
       coefficients.x1 < 0.0 ? relativePosition.first = coefficients.x1 :
                               relativePosition.first = coefficients.x2;
       coefficients.y1 > 0.0 ? relativePosition.second = coefficients.y1 :
                               relativePosition.second = coefficients.y2;
       return std::optional<std::pair<double, double>>(relativePosition);
   }
   else{
       std::cerr << "Azimut " << azimut << "out of allowed range!" << std::endl;
       return std::nullopt;
   }
}

void RelativePositionCalculator::setNewCalculatedRelativePosition(
        const std::optional<std::pair<double, double>> relativePosition) {

    if(relativePosition){
        calculatedRelativePosition.first += relativePosition.value().first;
        calculatedRelativePosition.second += relativePosition.value().second;
    } else{
        std::cerr <<"Calculation of new relative position is not successful! nullopt returned" << std::endl;
    }
}
