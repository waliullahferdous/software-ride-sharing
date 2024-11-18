#ifndef FARECALCULATOR_H
#define FARECALCULATOR_H

class FareCalculator {
public:
    static double calculate_fare(double distance, int demand, int supply) {
        double base_fare = 5.0;
        double per_km_rate = 2.0;

        double surge_multiplier = (demand > supply) ? 1.5 + ((demand - supply) * 0.1) : 1.0;

        return (base_fare + (distance * per_km_rate)) * surge_multiplier;
    }
};

#endif

