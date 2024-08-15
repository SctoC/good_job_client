#pragma once

#ifndef SQLITEWRAPPER_H
#define SQLITEWRAPPER_H

#include "sqlite3.h"
#include <string>
#include <vector>

class SQLiteWrapper {
public:
    SQLiteWrapper(const std::string& dbName);
    ~SQLiteWrapper();

    bool execute(const std::string& query);
    bool executeP(const std::string& sql, const std::vector<std::string>& params);

    std::vector<std::vector<std::string>> query(const std::string& query);


private:
    sqlite3* db;
    char* errorMessage;

    static int callback(void* data, int argc, char** argv, char** colName);
};

#endif // SQLITEWRAPPER_H
