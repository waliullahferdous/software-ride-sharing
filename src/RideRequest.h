#ifndef RIDEREQUEST_H
#define RIDEREQUEST_H

#include <string>
#include <utility> // For std::pair
#include <chrono>  // For time tracking

enum class Status {
    Pending,
    Assigned,
    Started,
    Completed,
    CancelledBeforeStart,
    CancelledAfterStart
};

class RideRequest {
private:
    std::string request_id;
    std::string rider_id;   // Rider ID
    std::string rider_name; // Rider Name
    std::string driver_id;
    std::pair<int, int> pickup_location;
    std::pair<int, int> dropoff_location;
    double fare = 0.0;
    Status status = Status::Pending;

    // Time tracking
    std::chrono::time_point<std::chrono::system_clock> start_time;
    std::chrono::time_point<std::chrono::system_clock> end_time;

public:
    // Constructor
    RideRequest(std::string req_id, std::string rider_id, std::string rider_name,
                std::pair<int, int> pickup, std::pair<int, int> dropoff)
        : request_id(req_id), rider_id(rider_id), rider_name(rider_name),
          pickup_location(pickup), dropoff_location(dropoff), status(Status::Pending) {}

    // Methods
    void assign_driver(std::string driver);
    void start_ride();
    void complete_ride();
    void cancel_ride(const std::string& by_whom, bool after_start);
    void update_status(Status new_status);

    // Setters
    void set_fare(double f) { fare = f; }

    // Getters
    std::string get_request_id() const { return request_id; }
    std::string get_rider_id() const { return rider_id; }
    std::string get_rider_name() const { return rider_name; }
    std::string get_driver_id() const { return driver_id; }
    std::pair<int, int> get_pickup_location() const { return pickup_location; }
    std::pair<int, int> get_dropoff_location() const { return dropoff_location; }
    double get_fare() const { return fare; }
    Status get_status() const { return status; }

    // Utility to convert Status enum to string
    static std::string status_to_string(Status s);
};

#endif

