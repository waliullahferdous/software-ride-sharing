#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <utility> // For std::pair
#include <sstream>
#include <iomanip>

class Utility {
public:
    // Calculate Euclidean distance between two points
    static double calculate_distance(const std::pair<int, int>& point1, const std::pair<int, int>& point2) {
        return std::sqrt(std::pow(point1.first - point2.first, 2) + std::pow(point1.second - point2.second, 2));
    }

    // Format a double value to the specified number of decimal places
    static std::string format_double(double value, int precision = 2) {
        std::ostringstream out;
        out << std::fixed << std::setprecision(precision) << value;
        return out.str();
    }
};

#endif

