#include "Logger.h"

// Get current timestamp as a string
std::string Logger::get_timestamp() const {
    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

// Log a general event
void Logger::log_event(const std::string& event) {
    std::string message = "[EVENT] " + get_timestamp() + " - " + event;
    std::cout << message << std::endl;

    if (file_stream.is_open()) {
        file_stream << message << std::endl;
    }
}

// Log an error
void Logger::log_error(const std::string& error) {
    std::string message = "[ERROR] " + get_timestamp() + " - " + error;
    std::cerr << message << std::endl;

    if (file_stream.is_open()) {
        file_stream << message << std::endl;
    }
}

