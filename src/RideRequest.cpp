#include "RideRequest.h"
#include "Utility.h"
#include <iostream>

void RideRequest::assign_driver(std::string driver) {
    if (status != Status::Pending) {
        std::cerr << "Error: Cannot assign a driver to a non-pending request." << std::endl;
        return;
    }
    driver_id = driver;
    status = Status::Assigned;
    std::cout << "Driver " << driver << " assigned to ride request " << request_id << std::endl;
}

void RideRequest::update_status(Status new_status) {
    if (status == Status::Completed) {
        std::cerr << "Error: Cannot update status of a completed request." << std::endl;
        return;
    }
    status = new_status;
    std::cout << "Ride request " << request_id << " status updated to " << status_to_string(status) << std::endl;
}

void RideRequest::start_ride() {
    if (status != Status::Assigned) {
        std::cerr << "Error: Cannot start a ride that has not been assigned to a driver." << std::endl;
        return;
    }

    double distance = Utility::calculate_distance(pickup_location, dropoff_location);
    std::cout << "Ride " << request_id << " started. Distance from pickup to dropoff: "
              << Utility::format_double(distance) << " km." << std::endl;
}

std::string RideRequest::status_to_string(Status s) {
    switch (s) {
        case Status::Pending:   return "Pending";
        case Status::Assigned:  return "Assigned";
        case Status::Completed: return "Completed";
        default:                return "Unknown";
    }
}

