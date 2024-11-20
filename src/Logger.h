#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>

class Logger {
private:
    std::string log_file;
    std::ofstream file_stream;

    // Get current timestamp as a string
    std::string get_timestamp() const;

public:
    // Constructor
    Logger(const std::string& file, bool reset = true);

    // Destructor
    ~Logger();

    // Log message
    void log_event(const std::string& event);
    void log_error(const std::string& error);
};

#endif


