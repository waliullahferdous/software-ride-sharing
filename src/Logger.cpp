#include "Logger.h"

// Get current timestamp as a string
std::string Logger::get_timestamp() const {
    std::time_t now = std::time(nullptr);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

// Constructor with reset option
Logger::Logger(const std::string& file, bool reset) : log_file(file) {
    // Open file in truncate mode if reset is true
    if (reset) {
        file_stream.open(log_file, std::ios::trunc);
    } else {
        file_stream.open(log_file, std::ios::app);
    }

    if (!file_stream.is_open()) {
        std::cerr << "Failed to open log file: " << log_file << std::endl;
    }
}

// Destructor
Logger::~Logger() {
    if (file_stream.is_open()) {
        file_stream.close();
    }
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


