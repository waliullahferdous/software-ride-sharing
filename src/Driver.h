#ifndef DRIVER_H
#define DRIVER_H

#include <string>

class Driver {
private:
    std::string driver_id;
    std::string name;
    std::string current_location;
    bool availability_status;

public:
    // Constructor
    Driver(std::string id, std::string driver_name, std::string location)
        : driver_id(id), name(driver_name), current_location(location), availability_status(true) {}

    // Methods
    void update_location(std::string new_location);
    void toggle_availability();
    
    // Getters
    std::string get_driver_id() const { return driver_id; }
    std::string get_name() const { return name; }
    std::string get_location() const { return current_location; }
    bool is_available() const { return availability_status; }
};

#endif

