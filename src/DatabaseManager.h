#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <sqlite3.h>
#include <string>
#include <iostream>

class DatabaseManager {
private:
    sqlite3* db;

public:
    DatabaseManager(const std::string& db_name);
    ~DatabaseManager();

    bool execute_query(const std::string& query);
    bool initialize_schema();

    // Utility to log errors
    void log_error(const std::string& msg);
};

#endif

