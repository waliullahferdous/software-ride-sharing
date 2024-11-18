#ifndef FARECALCULATOR_H
#define FARECALCULATOR_H

#include <utility>
#include "Utility.h"

class FareCalculator {
public:
    double calculate_fare(const std::pair<int, int>& pickup, const std::pair<int, int>& dropoff) {
        const double base_fare = 5.0; // Fixed base fare
        const double per_unit_distance_rate = 2.0; // Cost per unit of distance

        // Calculate pickup-to-dropoff distance
        double distance = Utility::calculate_distance(pickup, dropoff);

        // Fare formula
        double fare = base_fare + (per_unit_distance_rate * distance);
        return fare;
    }
};

#endif

