#ifndef RIDER_H
#define RIDER_H

#include <string>

class Rider {
private:
    std::string rider_id;
    std::string name;
    std::string pickup_location;
    std::string dropoff_location;

public:
    // Constructor
    Rider(std::string id, std::string rider_name)
        : rider_id(id), name(rider_name) {}

    // Methods
    void request_ride(std::string pickup, std::string dropoff);

    // Getters
    std::string get_rider_id() const { return rider_id; }
    std::string get_name() const { return name; }
    std::string get_pickup_location() const { return pickup_location; }
    std::string get_dropoff_location() const { return dropoff_location; }
};

#endif

