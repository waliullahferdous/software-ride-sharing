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

void SystemManager::add_driver(const Driver& driver) {
    active_drivers.push_back(driver);

    std::string query = "INSERT OR REPLACE INTO Drivers (driver_id, name, location_x, location_y, availability) VALUES ('" +
                        driver.get_driver_id() + "', '" + driver.get_name() + "', " +
                        std::to_string(driver.get_location().first) + ", " +
                        std::to_string(driver.get_location().second) + ", " +
                        (driver.is_available() ? "1" : "0") + ");";

    if (!db_manager.execute_query(query)) {
        db_manager.log_error("Failed to insert driver " + driver.get_driver_id());
    } else {
        logger.log_event("Driver " + driver.get_driver_id() + " (" + driver.get_name() + ") added to the system and database.");
    }
}


void SystemManager::add_request(std::shared_ptr<RideRequest> request) {
    active_requests.push_back(request);

    // Insert the rider if not already in the database
    std::string rider_query = "INSERT OR REPLACE INTO Riders (rider_id, name) VALUES ('" +
                              request->get_rider_id() + "', '" + request->get_rider_name() + "');";

    if (!db_manager.execute_query(rider_query)) {
        db_manager.log_error("Failed to insert rider " + request->get_rider_id());
    }

    // Insert the ride request into the database
    std::string ride_query = "INSERT OR REPLACE INTO RideRequests (request_id, rider_id, pickup_x, pickup_y, dropoff_x, dropoff_y, status) VALUES ('" +
                             request->get_request_id() + "', '" + request->get_rider_id() + "', " +
                             std::to_string(request->get_pickup_location().first) + ", " +
                             std::to_string(request->get_pickup_location().second) + ", " +
                             std::to_string(request->get_dropoff_location().first) + ", " +
                             std::to_string(request->get_dropoff_location().second) + ", 'Pending');";

    if (!db_manager.execute_query(ride_query)) {
        db_manager.log_error("Failed to insert ride request " + request->get_request_id());
    } else {
        logger.log_event("Ride request " + request->get_request_id() + " added to the system and database by " + request->get_rider_name() + ".");
    }
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

    Driver* assigned_driver = nullptr;

    for (Driver& driver : active_drivers) {
        if (driver.get_driver_id() == request->get_driver_id()) {
            assigned_driver = &driver;
            break;
        }
    }

    if (assigned_driver) {
        request->cancel_ride(by_whom, after_start);

        std::string status = after_start ? "CancelledAfterStart" : "CancelledBeforeStart";
        std::string ride_update_query = "UPDATE RideRequests SET status = '" + status +
                                        "' WHERE request_id = '" + request->get_request_id() + "';";

        if (!db_manager.execute_query(ride_update_query)) {
            db_manager.log_error("Failed to update ride request " + ride_id);
        }

        if (after_start) {
            assigned_driver->set_location(request->get_pickup_location().first, request->get_pickup_location().second);
            logger.log_event("Driver " + assigned_driver->get_driver_id() + " (" + assigned_driver->get_name() +
                             ") location updated to " + ride_id + " pickup point (" +
                             std::to_string(request->get_pickup_location().first) + ", " +
                             std::to_string(request->get_pickup_location().second) + ") after cancellation.");
        } else {
            logger.log_event("Driver " + assigned_driver->get_driver_id() + " (" + assigned_driver->get_name() +
                             ") location remains unchanged after cancellation.");
        }

        assigned_driver->complete_ride(); // Mark driver as available

        std::string driver_update_query = "UPDATE Drivers SET location_x = " +
                                          std::to_string(assigned_driver->get_location().first) + ", location_y = " +
                                          std::to_string(assigned_driver->get_location().second) + ", availability = 1 WHERE driver_id = '" +
                                          assigned_driver->get_driver_id() + "';";

        if (!db_manager.execute_query(driver_update_query)) {
            db_manager.log_error("Failed to update driver " + assigned_driver->get_driver_id());
        }

        logger.log_event("Ride " + ride_id + " cancelled by " + by_whom +
                         (after_start ? " after starting." : " before starting."));
    } else {
        logger.log_error("No assigned driver found for Ride " + ride_id);
    }
}


void SystemManager::complete_ride(const std::string& ride_id) {
    auto request = find_request_by_id(ride_id);
    if (!request) {
        logger.log_error("Ride ID " + ride_id + " not found.");
        return;
    }

    Driver* assigned_driver = nullptr;

    for (Driver& driver : active_drivers) {
        if (driver.get_driver_id() == request->get_driver_id()) {
            assigned_driver = &driver;
            break;
        }
    }

    if (assigned_driver) {
        request->complete_ride();

        std::string ride_update_query = "UPDATE RideRequests SET status = 'Completed', fare = " +
                                        std::to_string(request->get_fare()) +
                                        " WHERE request_id = '" + request->get_request_id() + "';";
        if (!db_manager.execute_query(ride_update_query)) {
            db_manager.log_error("Failed to update ride request " + ride_id);
        }

        assigned_driver->set_location(request->get_dropoff_location().first, request->get_dropoff_location().second);
        logger.log_event("Driver " + assigned_driver->get_driver_id() + " (" + assigned_driver->get_name() + 
                         ") location updated to " + ride_id + " drop-off point (" +
                         std::to_string(request->get_dropoff_location().first) + ", " +
                         std::to_string(request->get_dropoff_location().second) + ") after ride completion.");

        assigned_driver->complete_ride(); // Mark driver as available

        std::string driver_update_query = "UPDATE Drivers SET location_x = " +
                                          std::to_string(assigned_driver->get_location().first) + ", location_y = " +
                                          std::to_string(assigned_driver->get_location().second) + ", availability = 1 WHERE driver_id = '" +
                                          assigned_driver->get_driver_id() + "';";

        if (!db_manager.execute_query(driver_update_query)) {
            db_manager.log_error("Failed to update driver " + assigned_driver->get_driver_id());
        }

        logger.log_event("Ride " + ride_id + " completed.");
    } else {
        logger.log_error("No assigned driver found for Ride " + ride_id);
    }
}


std::shared_ptr<RideRequest> SystemManager::find_request_by_id(const std::string& ride_id) {
    auto it = std::find_if(active_requests.begin(), active_requests.end(),
                           [&ride_id](std::shared_ptr<RideRequest> request) { return request->get_request_id() == ride_id; });

    return (it != active_requests.end()) ? *it : nullptr;
}

