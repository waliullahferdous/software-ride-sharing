#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <vector>
#include <memory> // For std::shared_ptr
#include "Driver.h"
#include "RideRequest.h"
#include "Logger.h"
#include "FareCalculator.h"

class SystemManager {
private:
    std::vector<Driver> active_drivers;
    std::vector<std::shared_ptr<RideRequest>> active_requests; // Use shared_ptr for requests
    Logger logger;

public:
    SystemManager() : logger("system_log.txt") {}

    void add_driver(Driver driver);
    void add_request(std::shared_ptr<RideRequest> request);
    void match_ride();
};

#endif

