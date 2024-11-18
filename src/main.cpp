#include "Driver.h"
#include "Rider.h"
#include "RideRequest.h"
#include "SystemManager.h"
#include <memory> // For std::shared_ptr

int main() {
    SystemManager manager;

    // Add drivers with unique locations
    Driver driver1("D001", "Alice");
    driver1.set_location(0, 0); // Driver Alice starts at (0, 0)
    driver1.set_ride_limit(2);
    manager.add_driver(driver1);

    Driver driver2("D002", "Bob");
    driver2.set_location(10, 10); // Driver Bob starts at (10, 10)
    driver2.set_ride_limit(1);
    manager.add_driver(driver2);

    // Add riders with unique pickup and dropoff locations
    Rider rider1("R001", "Charlie");
    rider1.set_pickup_location(2, 2); // Pickup point for Charlie: (2, 2)

    Rider rider2("R002", "David");
    rider2.set_pickup_location(12, 12); // Pickup point for David: (12, 12)

    // Create ride requests with different dropoff locations
    auto request1 = std::make_shared<RideRequest>(
        "REQ001", rider1.get_rider_id(), rider1.get_pickup_location(), std::make_pair(5, 5)); // Dropoff: (5, 5)
    auto request2 = std::make_shared<RideRequest>(
        "REQ002", rider2.get_rider_id(), rider2.get_pickup_location(), std::make_pair(18, 18)); // Dropoff: (15, 15)

    manager.add_request(request1);
    manager.add_request(request2);

    // Match rides
    manager.match_ride();

    // Start rides after matching
    request1->start_ride();
    request2->start_ride();

    return 0;
}

