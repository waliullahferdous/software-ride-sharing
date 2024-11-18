#include "SystemManager.h"
#include "Utility.h"
#include <queue>

struct DriverNode {
    Driver* driver;
    double distance;

    // Comparator for priority queue (min-heap)
    bool operator<(const DriverNode& other) const {
        return distance > other.distance;
    }
};

void SystemManager::add_driver(Driver driver) {
    active_drivers.push_back(driver);
    logger.log_event("Driver " + driver.get_name() + " added to the system.");
}

void SystemManager::add_request(std::shared_ptr<RideRequest> request) {
    active_requests.push_back(request);
    logger.log_event("Ride request " + request->get_request_id() + " added to the system.");
}

void SystemManager::match_ride() {
    if (active_drivers.empty() || active_requests.empty()) {
        logger.log_error("No available drivers or requests to match.");
        return;
    }

    FareCalculator fare_calculator;

    for (auto& request : active_requests) {
        auto rider_location = request->get_pickup_location();
        std::priority_queue<DriverNode> pq;

        // Populate the priority queue with available drivers
        for (Driver& driver : active_drivers) {
            if (!driver.is_available()) continue;

            double distance = Utility::calculate_distance(rider_location, driver.get_location());
            pq.push({&driver, distance});
        }

        if (!pq.empty()) {
            Driver* best_driver = pq.top().driver;

            // Calculate fare using pickup and dropoff locations
            double fare = fare_calculator.calculate_fare(request->get_pickup_location(), request->get_dropoff_location());

            best_driver->accept_ride();
            request->assign_driver(best_driver->get_driver_id());
            request->update_status(Status::Assigned);
            request->set_fare(fare);

            logger.log_event("Matched RideRequest " + request->get_request_id() +
                             " with Driver " + best_driver->get_name() +
                             " (driver-to-pickup distance: " + Utility::format_double(pq.top().distance) +
                             ", fare: $" + Utility::format_double(fare) + ")");
        } else {
            logger.log_error("No available driver found for RideRequest " + request->get_request_id());
        }
    }

    active_requests.clear();
}

