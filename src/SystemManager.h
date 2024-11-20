#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "DatabaseManager.h"
#include <vector>
#include <memory> // For std::shared_ptr
#include "Driver.h"
#include "RideRequest.h"
#include "Logger.h"
#include "FareCalculator.h"
#include <fstream>
#include <sstream>
#include <string>

class SystemManager {
private:
    std::vector<Driver> active_drivers;
    std::vector<std::shared_ptr<RideRequest>> active_requests; // Active ride requests
    DatabaseManager db_manager; // DatabaseManager instance
    std::vector<std::shared_ptr<RideRequest>> completed_rides; // Completed rides
    std::vector<std::shared_ptr<RideRequest>> canceled_rides;  // Canceled rides
    Logger logger;

public:
    SystemManager() : logger("system_log.txt"), db_manager("ridesharing.db") {
        db_manager.initialize_schema();
    }

    void add_driver(const Driver& driver);
    void add_rider(const std::string& rider_id, const std::string& name);
    void add_request(std::shared_ptr<RideRequest> request);
    void match_ride();

    // Ride lifecycle methods
    void cancel_ride(const std::string& ride_id, const std::string& by_whom, bool after_start);
    void complete_ride(const std::string& ride_id);

    void load_drivers_from_file(const std::string& filename);
    void load_riders_from_file(const std::string& filename);
    void load_rides_from_file(const std::string& filename);

    // New methods for ride history
    std::vector<std::vector<std::string>> get_driver_history(const std::string& driver_id);
    std::vector<std::vector<std::string>> get_rider_history(const std::string& rider_id);
    void display_history(const std::vector<std::vector<std::string>>& history, const std::string& header);


    // Helpers
    std::shared_ptr<RideRequest> find_request_by_id(const std::string& ride_id);

    // Add getter for active requests
    const std::vector<std::shared_ptr<RideRequest>>& get_active_requests() const {
        return active_requests;
    }


};

#endif

