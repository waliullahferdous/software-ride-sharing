#include "DatabaseManager.h"

DatabaseManager::DatabaseManager(const std::string& db_name) {
    if (sqlite3_open(db_name.c_str(), &db)) {
        log_error("Failed to open database: " + std::string(sqlite3_errmsg(db)));
        db = nullptr;
    } else {
        std::cout << "Database connected: " << db_name << std::endl;
    }
}

DatabaseManager::~DatabaseManager() {
    if (db) {
        sqlite3_close(db);
        std::cout << "Database connection closed." << std::endl;
    }
}

bool DatabaseManager::execute_query(const std::string& query) {
    char* err_msg = nullptr;
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK) {
        log_error("SQL Error: " + std::string(err_msg));
        sqlite3_free(err_msg);
        return false;
    }
    return true;
}

bool DatabaseManager::initialize_schema() {
    std::string create_drivers_table = R"(
        CREATE TABLE IF NOT EXISTS Drivers (
            driver_id TEXT PRIMARY KEY,
            name TEXT NOT NULL,
            location_x INTEGER NOT NULL,
            location_y INTEGER NOT NULL,
            availability BOOLEAN NOT NULL
        );
    )";

    std::string create_riders_table = R"(
        CREATE TABLE IF NOT EXISTS Riders (
            rider_id TEXT PRIMARY KEY,
            name TEXT NOT NULL
        );
    )";

    std::string create_riderequests_table = R"(
        CREATE TABLE IF NOT EXISTS RideRequests (
            request_id TEXT PRIMARY KEY,
            rider_id TEXT NOT NULL,
            driver_id TEXT,
            pickup_x INTEGER NOT NULL,
            pickup_y INTEGER NOT NULL,
            dropoff_x INTEGER NOT NULL,
            dropoff_y INTEGER NOT NULL,
            status TEXT NOT NULL,
            fare REAL,
            FOREIGN KEY (rider_id) REFERENCES Riders (rider_id),
            FOREIGN KEY (driver_id) REFERENCES Drivers (driver_id)
        );
    )";

    return execute_query(create_drivers_table) &&
           execute_query(create_riders_table) &&
           execute_query(create_riderequests_table);
}

void DatabaseManager::log_error(const std::string& msg) {
    std::cerr << "Database Error: " << msg << std::endl;
}

