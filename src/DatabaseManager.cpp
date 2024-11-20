#include "DatabaseManager.h"
#include <vector>
#include <sstream> 

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

std::vector<std::vector<std::string>> DatabaseManager::fetch_query_results(const std::string& query) {
    std::vector<std::vector<std::string>> results;
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::vector<std::string> row;
            for (int i = 0; i < sqlite3_column_count(stmt); ++i) {
                const char* val = reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
                row.push_back(val ? val : "NULL");
            }
            results.push_back(row);
        }
    } else {
        log_error("Failed to fetch query results: " + query);
    }

    sqlite3_finalize(stmt);
    return results;
}

void DatabaseManager::log_error(const std::string& msg) {
    std::cerr << "Database Error: " << msg << std::endl;
}

