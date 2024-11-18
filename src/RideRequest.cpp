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

void RideRequest::start_ride() {
    if (status != Status::Assigned) {
        std::cerr << "Error: Cannot start a ride that has not been assigned to a driver." << std::endl;
        return;
    }

    status = Status::Started;
    start_time = std::chrono::system_clock::now();
    std::cout << "Ride " << request_id << " started." << std::endl;
}

void RideRequest::complete_ride() {
    if (status != Status::Started) {
        std::cerr << "Error: Cannot complete a ride that has not been started." << std::endl;
        return;
    }

    status = Status::Completed;
    end_time = std::chrono::system_clock::now();

    // Calculate time taken
    std::chrono::duration<double> elapsed_time = end_time - start_time;

    // Calculate pickup-to-dropoff distance
    double pickup_to_dropoff_distance = Utility::calculate_distance(pickup_location, dropoff_location);

    std::cout << "Ride " << request_id << " completed. "
              << "Pickup-to-dropoff distance: " << pickup_to_dropoff_distance << " units. "
              << "Time taken: " << elapsed_time.count() << " seconds." << std::endl;
}


void RideRequest::cancel_ride(const std::string& by_whom, bool after_start) {
    if (after_start && status != Status::Started) {
        std::cerr << "Error: Cannot cancel a ride after it has not started." << std::endl;
        return;
    }
    if (!after_start && status != Status::Assigned) {
        std::cerr << "Error: Cannot cancel a ride before it has been assigned." << std::endl;
        return;
    }

    status = after_start ? Status::CancelledAfterStart : Status::CancelledBeforeStart;

    std::cout << "Ride " << request_id << " cancelled by " << by_whom
              << (after_start ? " after starting." : " before starting.") << std::endl;
}

void RideRequest::update_status(Status new_status) {
    if (status == Status::Completed || status == Status::CancelledBeforeStart || status == Status::CancelledAfterStart) {
        std::cerr << "Error: Cannot update the status of a completed or canceled ride." << std::endl;
        return;
    }
    status = new_status;
    std::cout << "Ride request " << request_id << " status updated to " << status_to_string(status) << std::endl;
}

std::string RideRequest::status_to_string(Status s) {
    switch (s) {
        case Status::Pending:             return "Pending";
        case Status::Assigned:            return "Assigned";
        case Status::Started:             return "Started";
        case Status::Completed:           return "Completed";
        case Status::CancelledBeforeStart:return "Cancelled (Before Start)";
        case Status::CancelledAfterStart: return "Cancelled (After Start)";
        default:                          return "Unknown";
    }
}

