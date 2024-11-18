#include "SystemManager.h"
#include "Utility.h"
#include <limits> // For std::numeric_limits

void SystemManager::add_driver(Driver driver) {
    active_drivers.push_back(driver);
    logger.log_event("Driver " + driver.get_name() + " added to the system.");
}

void SystemManager::add_request(RideRequest request) {
    active_requests.push_back(request);
    logger.log_event("Ride request " + request.get_request_id() + " added to the system.");
}

void SystemManager::match_ride() {
    if (active_drivers.empty() || active_requests.empty()) {
        logger.log_error("No available drivers or requests to match.");
        return;
    }

    // Iterate through all active requests
    for (RideRequest& request : active_requests) {
        auto rider_location = request.get_pickup_location();

        double min_distance = std::numeric_limits<double>::max();
        Driver* best_driver = nullptr;

        // Find the closest driver
        for (Driver& driver : active_drivers) {
            if (!driver.is_available()) continue;

            double distance = Utility::calculate_distance(rider_location, driver.get_location());
            if (distance < min_distance) {
                min_distance = distance;
                best_driver = &driver;
            }
        }

        if (best_driver) {
            best_driver->toggle_availability(); // Mark the driver as unavailable
            request.assign_driver(best_driver->get_driver_id());
            request.update_status(Status::Assigned);
            logger.log_event("Matched RideRequest " + request.get_request_id() +
                             " with Driver " + best_driver->get_driver_id() +
                             " (distance: " + std::to_string(min_distance) + ")");
        } else {
            logger.log_error("No available driver found for RideRequest " + request.get_request_id());
        }
    }

    // Clear the list of active requests
    active_requests.clear();
}

