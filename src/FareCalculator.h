#ifndef FARECALCULATOR_H
#define FARECALCULATOR_H

#include <utility> // For std::pair
#include "Utility.h"

class FareCalculator {
private:
    double base_fare;
    double per_km_rate;

public:
    FareCalculator(double base = 5.0, double per_km = 2.0)
        : base_fare(base), per_km_rate(per_km) {}

    double calculate_fare(const std::pair<int, int>& start, const std::pair<int, int>& end) const {
        double distance = Utility::calculate_distance(start, end);
        return base_fare + (distance * per_km_rate);
    }
};

#endif

