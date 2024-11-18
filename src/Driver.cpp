#include "Driver.h"
#include <iostream>

// Update driver's location
void Driver::update_location(std::string new_location) {
    current_location = new_location;
    std::cout << "Driver's location updated to: " << current_location << std::endl;
}

// Toggle availability status
void Driver::toggle_availability() {
    availability_status = !availability_status;
    std::cout << "Driver is now " << (availability_status ? "available" : "not available") << std::endl;
}

