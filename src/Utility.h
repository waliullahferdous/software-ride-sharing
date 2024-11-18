#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <utility> // For std::pair

class Utility {
public:
    static double calculate_distance(const std::pair<int, int>& point1, const std::pair<int, int>& point2) {
        return std::sqrt(std::pow(point1.first - point2.first, 2) + std::pow(point1.second - point2.second, 2));
    }
};

#endif

