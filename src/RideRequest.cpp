#include "RideRequest.h"
#include <iostream>

// Assign a driver to the ride request
void RideRequest::assign_driver(std::string driver) {
    if (status != Status::Pending) {
        std::cerr << "Error: Cannot assign a driver to a non-pending request." << std::endl;
        return;
    }
    driver_id = driver;
    status = Status::Assigned;
    std::cout << "Driver " << driver << " assigned to ride request " << request_id << std::endl;
}

// Update the status of the ride request
void RideRequest::update_status(Status new_status) {
    if (status == Status::Completed) {
        std::cerr << "Error: Cannot update status of a completed request." << std::endl;
        return;
    }
    status = new_status;
    std::cout << "Ride request " << request_id << " status updated to " << status_to_string(status) << std::endl;
}

// Utility to convert Status enum to string
std::string RideRequest::status_to_string(Status s) {
    switch (s) {
        case Status::Pending:   return "Pending";
        case Status::Assigned:  return "Assigned";
        case Status::Completed: return "Completed";
        default:                return "Unknown";
    }
}

