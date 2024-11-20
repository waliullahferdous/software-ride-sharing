#include "SystemManager.h"
#include <memory>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdlib> // For rand()
#include <ctime>   // For seeding rand()

// Simulates delay for better readability of logs
void delay() {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

// Processes a ride randomly (complete, cancel before/after starting)
void process_random_ride(SystemManager& manager, std::shared_ptr<RideRequest> request) {
    if (!request) return;

    auto status = request->get_status();
    if (status == Status::Completed || status == Status::CancelledBeforeStart || status == Status::CancelledAfterStart) {
        return; // Skip already processed rides
    }

    // Randomize actions: 0 = cancel before starting, 1 = cancel after starting, 2 = complete
    int action = rand() % 3; // Random number between 0 and 2
    std::string by_whom = (rand() % 2 == 0) ? "Rider" : "Driver"; // Randomly decide who cancels

    if (status == Status::Started) {
        if (action == 0) {
            // Cancel before starting
            delay();
            manager.cancel_ride(request->get_request_id(), by_whom, false);
        } else if (action == 1) {
            // Start and then cancel after starting
            request->start_ride();
            delay();
            manager.cancel_ride(request->get_request_id(), by_whom, true);
        } else {
            // Complete the ride
            request->start_ride();
            delay();
            manager.complete_ride(request->get_request_id());
        }
    }
}



int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // Seed for randomness

    SystemManager manager;

    // Load data
    manager.load_drivers_from_file("drivers.txt");
    manager.load_riders_from_file("riders.txt");
    manager.load_rides_from_file("rides.txt");

    // Automatically process all rides
    while (!manager.get_active_requests().empty()) {
        delay();
        manager.match_ride();

        // Get all active ride requests
        std::vector<std::shared_ptr<RideRequest>> active_requests = manager.get_active_requests();

        if (active_requests.empty()) break; // Exit loop if no active rides remain

        for (auto& request : active_requests) {
            delay();
            process_random_ride(manager, request);
        }
    }

    return 0;
}

