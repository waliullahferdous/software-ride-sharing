#include "Driver.h"
#include "Rider.h"
#include "RideRequest.h"
#include "SystemManager.h"
#include <memory> // For std::shared_ptr

int main() {
    SystemManager manager;

    // Add drivers
    Driver driver1("D001", "Alice");
    driver1.set_location(0, 0);
    manager.add_driver(driver1);

    Driver driver2("D002", "Bob");
    driver2.set_location(5, 5);
    manager.add_driver(driver2);


    // First ride: Cancelled after starting by Rider
    auto request1 = std::make_shared<RideRequest>("REQ001", "R001", "Charlie", std::make_pair(2, 2), std::make_pair(5, 5));
    manager.add_request(request1);

    manager.match_ride();
    request1->start_ride();
    manager.cancel_ride("REQ001", "Rider", true);

    // Second ride: Successfully completed
    auto request2 = std::make_shared<RideRequest>("REQ002", "R002", "Eve", std::make_pair(3, 3), std::make_pair(8, 8));
    manager.add_request(request2);

    manager.match_ride();
    request2->start_ride();
    manager.complete_ride("REQ002");




    // Third ride: Cancelled before starting by Driver
    auto request3 = std::make_shared<RideRequest>("REQ003", "R003", "David", std::make_pair(10, 10), std::make_pair(15, 15));
    manager.add_request(request3);

    manager.match_ride();
    manager.cancel_ride("REQ003", "Driver", false);

    return 0;

}

