#include "Rider.h"
#include <iostream>

// Request a new ride

void Rider::request_ride (std::string pickup, std::string dropoff) {
	pickup_location = pickup;
	dropoff_location = dropoff;
	std::cout << "Ride requested from " << pickup << " to " << dropoff << std::endl;
}
