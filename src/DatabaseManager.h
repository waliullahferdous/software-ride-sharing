#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>

class DatabaseManager {
private:
    sqlite3* db;

public:
    DatabaseManager(const std::string& db_name);
    ~DatabaseManager();

    bool execute_query(const std::string& query);
    bool initialize_schema();

    sqlite3* get_db() const { return db; }

    // Fetch data from the database
    std::vector<std::vector<std::string>> fetch_query_results(const std::string& query);

    // Utility to log errors
    void log_error(const std::string& msg);
};

#endif

