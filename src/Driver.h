#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <utility> // For std::pair

class Driver {
private:
    std::string driver_id;
    std::string name;
    std::pair<int, int> current_location; // Coordinates (x, y)
    bool availability_status;

public:
    // Constructor
    Driver(std::string id, std::string driver_name, std::string location)
        : driver_id(id), name(driver_name), availability_status(true) {}

    // Methods
    void set_location(int x, int y) { current_location = {x, y}; }
    std::pair<int, int> get_location() const { return current_location; }
    void toggle_availability() { availability_status = !availability_status; }
    bool is_available() const { return availability_status; }

    // Getters
    std::string get_driver_id() const { return driver_id; }
    std::string get_name() const { return name; }
};

#endif

