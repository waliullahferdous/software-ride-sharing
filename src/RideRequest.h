#ifndef RIDEREQUEST_H
#define RIDEREQUEST_H

#include <string>
#include <utility> // For std::pair
#include "Rider.h"

// Enum class for ride status
enum class Status { Pending, Assigned, Completed };

class RideRequest {
private:
    std::string request_id;
    std::string rider_id;
    std::string driver_id;
    std::pair<int, int> pickup_location;
    std::string dropoff_location;
    Status status;

public:
    // Constructor
    RideRequest(std::string req_id, std::string rider, std::pair<int, int> pickup, std::string dropoff)
        : request_id(req_id), rider_id(rider), pickup_location(pickup), dropoff_location(dropoff), status(Status::Pending) {}

    // Methods
    void assign_driver(std::string driver);
    void update_status(Status new_status);

    // Getters
    std::string get_request_id() const { return request_id; }
    std::string get_rider_id() const { return rider_id; }
    std::pair<int, int> get_pickup_location() const { return pickup_location; }
    std::string get_dropoff_location() const { return dropoff_location; }
    Status get_status() const { return status; }

    // Utility to convert Status enum to string
    static std::string status_to_string(Status s);
};

#endif

