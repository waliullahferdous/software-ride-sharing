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


void SystemManager::load_drivers_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string driver_id, name;
        int location_x, location_y, availability;

        std::getline(ss, driver_id, ',');
        std::getline(ss, name, ',');
        ss >> location_x;
        ss.ignore(1); // Ignore comma
        ss >> location_y;
        ss.ignore(1); // Ignore comma
        ss >> availability;

        Driver driver(driver_id, name);
        driver.set_location(location_x, location_y);
        driver.set_availability(availability == 1); // Use set_availability
        add_driver(driver);
    }
    file.close();
}


void SystemManager::load_riders_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string rider_id, name;

        std::getline(ss, rider_id, ',');
        std::getline(ss, name, ',');

        add_rider(rider_id, name);
    }
    file.close();
}

void SystemManager::load_rides_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string request_id, rider_id;
        int pickup_x, pickup_y, dropoff_x, dropoff_y;

        std::getline(ss, request_id, ',');
        std::getline(ss, rider_id, ',');
        ss >> pickup_x;
        ss.ignore(1); // Ignore comma
        ss >> pickup_y;
        ss.ignore(1); // Ignore comma
        ss >> dropoff_x;
        ss.ignore(1); // Ignore comma
        ss >> dropoff_y;

	// Fetch rider name from database
        std::string rider_name;
        std::string query = "SELECT name FROM Riders WHERE rider_id = '" + rider_id + "';";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db_manager.get_db(), query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
    	if (sqlite3_step(stmt) == SQLITE_ROW) {
        rider_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }
    sqlite3_finalize(stmt);
}  else {
            db_manager.log_error("Failed to fetch rider name for Rider ID: " + rider_id);
        }

        auto request = std::make_shared<RideRequest>(request_id, rider_id, rider_name, std::make_pair(pickup_x, pickup_y), std::make_pair(dropoff_x, dropoff_y));
        add_request(request);
    }
    file.close();
}


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

void SystemManager::add_rider(const std::string& rider_id, const std::string& name) {
    // Insert the rider into the database
    std::string query = "INSERT OR REPLACE INTO Riders (rider_id, name) VALUES ('" +
                        rider_id + "', '" + name + "');";

    if (!db_manager.execute_query(query)) {
        db_manager.log_error("Failed to insert rider " + rider_id);
    } else {
        logger.log_event("Rider " + rider_id + " (" + name + ") added to the system and database.");
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
            request->set_fare(fare);

            // Automatically start the ride
            request->update_status(Status::Started);
            logger.log_event("Ride " + request->get_request_id() + " started for Rider " +
                             request->get_rider_id() + " (" + request->get_rider_name() + ").");

            logger.log_event("Matched RideRequest " + request->get_request_id() +
                             " with Driver " + best_driver->get_driver_id() + " (" + best_driver->get_name() + ") " +
                             " (driver-to-pickup distance: " + Utility::format_double(pq.top().distance) +
                             ", pickup-to-dropoff distance: " + Utility::format_double(pickup_to_dropoff_distance) +
                             ", fare: $" + Utility::format_double(fare) + ")");
        } else {
            logger.log_error("No available driver found for RideRequest " + request->get_request_id());
        }
    }

    // Remove completed or canceled requests from active_requests
    active_requests.erase(std::remove_if(active_requests.begin(), active_requests.end(),
                                         [](const std::shared_ptr<RideRequest>& r) {
                                             return r->get_status() == Status::Completed ||
                                                    r->get_status() == Status::CancelledBeforeStart ||
                                                    r->get_status() == Status::CancelledAfterStart;
                                         }),
                          active_requests.end());
}

void SystemManager::cancel_ride(const std::string& ride_id, const std::string& by_whom, bool after_start) {
    auto request = find_request_by_id(ride_id);
    if (!request) {
        logger.log_error("Ride ID " + ride_id + " not found.");
        return;
    }

    // Handle Pending rides
    if (request->get_status() == Status::Pending) {
        request->update_status(Status::CancelledBeforeStart);
        logger.log_event("Ride " + ride_id + " cancelled by " + by_whom + " before being assigned a driver.");
        return;
    }

    // Validate Assigned or Started states for cancellation
    if (request->get_status() != Status::Assigned && request->get_status() != Status::Started) {
        logger.log_event("Ride " + ride_id + " could not be canceled in its current state.");
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
        // Update ride status
        request->update_status(after_start ? Status::CancelledAfterStart : Status::CancelledBeforeStart);

	// Always set the database status as "Cancelled"
        std::string ride_update_query = "UPDATE RideRequests SET status = 'Cancelled' WHERE request_id = '" + request->get_request_id() + "';";

        if (!db_manager.execute_query(ride_update_query)) {
            db_manager.log_error("Failed to update ride request " + ride_id);
        }

        // Log the cancellation event
        logger.log_event("Ride " + ride_id + " cancelled by " + by_whom +
                         (after_start ? " after starting." : " before starting."));

        // Handle location update after cancellation
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

        // Mark driver as available again
        assigned_driver->complete_ride();

        // Update driver location in the database
        std::string driver_update_query = "UPDATE Drivers SET location_x = " +
                                          std::to_string(assigned_driver->get_location().first) + ", location_y = " +
                                          std::to_string(assigned_driver->get_location().second) + ", availability = 1 WHERE driver_id = '" +
                                          assigned_driver->get_driver_id() + "';";
        if (!db_manager.execute_query(driver_update_query)) {
            logger.log_error("Failed to update driver " + assigned_driver->get_driver_id());
        }
    } else {
        logger.log_error("No assigned driver found for Ride " + ride_id + ".");
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

	logger.log_event("Ride " + ride_id + " completed.");

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
    } else {
        logger.log_error("No assigned driver found for Ride " + ride_id);
    }
}

std::shared_ptr<RideRequest> SystemManager::find_request_by_id(const std::string& ride_id) {
    auto it = std::find_if(active_requests.begin(), active_requests.end(),
                           [&ride_id](std::shared_ptr<RideRequest> request) {
                               return request->get_request_id() == ride_id;
                           });

    return (it != active_requests.end()) ? *it : nullptr;
}


