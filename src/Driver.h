#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <utility> // For std::pair

class Driver {
private:
    std::string driver_id;
    std::string name;
    std::pair<int, int> current_location; // Coordinates (x, y)
    bool availability_status = true;
    int ride_limit = 1; // Maximum rides a driver can handle simultaneously
    int current_rides = 0;

public:
    // Constructor
    Driver(std::string id, std::string driver_name)
        : driver_id(id), name(driver_name) {}

    // Methods
    void set_location(int x, int y) { current_location = {x, y}; }
    std::pair<int, int> get_location() const { return current_location; }
    void toggle_availability() { availability_status = !availability_status; }
    void set_availability(bool available) { availability_status = available; }
    bool is_available() const { return availability_status && can_accept_ride(); }
    bool can_accept_ride() const { return current_rides < ride_limit; }
    void accept_ride() { current_rides++; }
    void complete_ride() { if (current_rides > 0) current_rides--; }

    // Setters
    void set_ride_limit(int limit) { ride_limit = limit; }

    // Getters
    std::string get_driver_id() const { return driver_id; }
    std::string get_name() const { return name; }
};

#endif

