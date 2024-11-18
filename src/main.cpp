#include <iostream>
#include <string>
#include "Driver.h"
#include "Rider.h"
#include "RideRequest.h"

int main() {
    // Create Drivers
    Driver driver1("DR001", "Alice", {0, 0});
    Driver driver2("DR002", "Bob", {5, 5});

    // Create Riders
    Rider rider1("RD001", "Charlie", {1, 1});
    Rider rider2("RD002", "Dave", {10, 10});

    // Create Ride Requests
    RideRequest request1("REQ001", rider1.get_rider_id(), rider1.get_pickup_location(), {2, 2});
    RideRequest request2("REQ002", rider2.get_rider_id(), rider2.get_pickup_location(), {8, 8});

    // Display Drivers
    std::cout << "Drivers:" << std::endl;
    std::cout << "Driver 1: " << driver1.get_name() << " at " << driver1.get_location().first
              << ", " << driver1.get_location().second << std::endl;
    std::cout << "Driver 2: " << driver2.get_name() << " at " << driver2.get_location().first
              << ", " << driver2.get_location().second << std::endl;

    // Display Riders
    std::cout << "\nRiders:" << std::endl;
    std::cout << "Rider 1: " << rider1.get_name() << " at " << rider1.get_pickup_location().first
              << ", " << rider1.get_pickup_location().second << std::endl;
    std::cout << "Rider 2: " << rider2.get_name() << " at " << rider2.get_pickup_location().first
              << ", " << rider2.get_pickup_location().second << std::endl;

    // Process Ride Requests
    std::cout << "\nRide Requests:" << std::endl;
    std::cout << "Request 1: Rider " << request1.get_rider_id() << " from ("
              << request1.get_pickup_location().first << ", " << request1.get_pickup_location().second
              << ") to (" << request1.get_dropoff_location().first << ", "
              << request1.get_dropoff_location().second << ")" << std::endl;

    std::cout << "Request 2: Rider " << request2.get_rider_id() << " from ("
              << request2.get_pickup_location().first << ", " << request2.get_pickup_location().second
              << ") to (" << request2.get_dropoff_location().first << ", "
              << request2.get_dropoff_location().second << ")" << std::endl;

    // Update and Display Driver Locations
    driver1.update_location({2, 2});
    driver2.update_location({8, 8});

    std::cout << "\nUpdated Driver Locations:" << std::endl;
    std::cout << "Driver 1: " << driver1.get_name() << " at " << driver1.get_location().first
              << ", " << driver1.get_location().second << std::endl;
    std::cout << "Driver 2: " << driver2.get_name() << " at " << driver2.get_location().first
              << ", " << driver2.get_location().second << std::endl;

    return 0;
}

