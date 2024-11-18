#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <vector>
#include "Driver.h"
#include "RideRequest.h"
#include "Logger.h"

class SystemManager {
private:
    std::vector<Driver> active_drivers;
    std::vector<RideRequest> active_requests;
    Logger logger;

public:
    SystemManager() : logger("system_log.txt") {}

    void add_driver(Driver driver);
    void add_request(RideRequest request);
    void match_ride();
};

#endif

