#ifndef RIDER_H
#define RIDER_H

#include <string>
#include <utility> // For std::pair

class Rider {
private:
    std::string rider_id;
    std::string name;
    std::pair<int, int> pickup_location; // Coordinates (x, y)

public:
    // Constructor
    Rider(std::string id, std::string rider_name)
        : rider_id(id), name(rider_name) {}

    // Methods
    void set_pickup_location(int x, int y) { pickup_location = {x, y}; }
    std::pair<int, int> get_pickup_location() const { return pickup_location; }

    // Getters
    std::string get_rider_id() const { return rider_id; }
    std::string get_name() const { return name; }
};

#endif

