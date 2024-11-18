#include "Driver.h"
#include "Rider.h"
#include "RideRequest.h"
#include "SystemManager.h"

int main() {
    SystemManager manager;

    Driver driver1("D001", "Alice", "Downtown");
    driver1.set_location(0, 0);
    manager.add_driver(driver1);

    Driver driver2("D002", "Bob", "Uptown");
    driver2.set_location(5, 5);
    manager.add_driver(driver2);

    Driver driver3("D003", "Charlie", "Suburb");
    driver3.set_location(10, 10);
    manager.add_driver(driver3);

    Rider rider1("R001", "David");
    rider1.set_pickup_location(1, 1);

    Rider rider2("R002", "Eve");
    rider2.set_pickup_location(6, 6);

    RideRequest request1("REQ001", rider1.get_rider_id(), rider1.get_pickup_location(), "Library");
    RideRequest request2("REQ002", rider2.get_rider_id(), rider2.get_pickup_location(), "Museum");

    manager.add_request(request1);
    manager.add_request(request2);

    manager.match_ride();

    return 0;
}

