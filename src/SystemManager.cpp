#include "SystemManager.h"
#include "Utility.h"
#include <queue>
#include <algorithm>

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
    logger.log_event("Driver " + driver.get_driver_id() + " (" + driver.get_name() + ") added to the system.");
}

void SystemManager::add_request(std::shared_ptr<RideRequest> request) {
    active_requests.push_back(request);
    logger.log_event("Ride request " + request->get_request_id() + " added to the system by Rider " +
                     request->get_rider_id() + " (" + request->get_rider_name() + ").");
}

void SystemManager::match_ride() {
    if (active_drivers.empty() || active_requests.empty()) {
        logger.log_error("No available drivers or requests to match.");
        return;
    }

    FareCalculator fare_calculator;

    for (auto& request : active_requests) {
        if (request->get_status() != Status::Pending) {
            continue; // Skip non-pending rides
        }

        auto rider_location = request->get_pickup_location();
        std::priority_queue<DriverNode> pq;

        for (Driver& driver : active_drivers) {
            if (!driver.is_available()) continue;

            double distance = Utility::calculate_distance(rider_location, driver.get_location());
            pq.push({&driver, distance});
        }

        if (!pq.empty()) {
            Driver* best_driver = pq.top().driver;

            // Calculate pickup-to-dropoff distance
            double pickup_to_dropoff_distance = Utility::calculate_distance(request->get_pickup_location(), request->get_dropoff_location());

            // Calculate fare
            double fare = fare_calculator.calculate_fare(request->get_pickup_location(), request->get_dropoff_location());

            best_driver->accept_ride();
            request->assign_driver(best_driver->get_driver_id());
            request->update_status(Status::Assigned);
            request->set_fare(fare);

            logger.log_event("Matched RideRequest " + request->get_request_id() +
                             " with Driver " + best_driver->get_driver_id() + " (" + best_driver->get_name() + ") " +
                             " (driver-to-pickup distance: " + Utility::format_double(pq.top().distance) +
                             ", pickup-to-dropoff distance: " + Utility::format_double(pickup_to_dropoff_distance) +
                             ", fare: $" + Utility::format_double(fare) + ")");
        } else {
            logger.log_error("No available driver found for RideRequest " + request->get_request_id());
        }
    }

    // Keep all non-pending rides in active_requests
    active_requests.erase(std::remove_if(active_requests.begin(), active_requests.end(),
                                         [](std::shared_ptr<RideRequest> r) { return r->get_status() == Status::Pending; }),
                          active_requests.end());
}


void SystemManager::cancel_ride(const std::string& ride_id, const std::string& by_whom, bool after_start) {
    auto request = find_request_by_id(ride_id);
    if (!request) {
        logger.log_error("Ride ID " + ride_id + " not found.");
        return;
    }

    // Reset driver availability if assigned
    if (request->get_status() == Status::Assigned || request->get_status() == Status::Started) {
        for (Driver& driver : active_drivers) {
            if (driver.get_driver_id() == request->get_driver_id()) {
                driver.complete_ride(); // Mark driver as available
                break;
            }
        }
    }

    request->cancel_ride(by_whom, after_start);
    canceled_rides.push_back(request);

    logger.log_event("Ride " + ride_id + " cancelled by " + by_whom +
                     (after_start ? " after starting." : " before starting."));
}

void SystemManager::complete_ride(const std::string& ride_id) {
    auto request = find_request_by_id(ride_id);
    if (!request) {
        logger.log_error("Ride ID " + ride_id + " not found.");
        return;
    }

    request->complete_ride();

    // Move to completed rides
    completed_rides.push_back(request);

    // Remove from active rides
    active_requests.erase(std::remove(active_requests.begin(), active_requests.end(), request), active_requests.end());

    logger.log_event("Ride " + ride_id + " completed.");
}

std::shared_ptr<RideRequest> SystemManager::find_request_by_id(const std::string& ride_id) {
    auto it = std::find_if(active_requests.begin(), active_requests.end(),
                           [&ride_id](std::shared_ptr<RideRequest> request) { return request->get_request_id() == ride_id; });

    return (it != active_requests.end()) ? *it : nullptr;
}

