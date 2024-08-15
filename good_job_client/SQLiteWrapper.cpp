#include "SQLiteWrapper.h"
#include <iostream>

SQLiteWrapper::SQLiteWrapper(const std::string& dbName) : db(nullptr), errorMessage(nullptr) {
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    }
}

SQLiteWrapper::~SQLiteWrapper() {
    if (db) {
        sqlite3_close(db);
    }
}
bool SQLiteWrapper::execute(const std::string& query) {
    if (sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }
    return true;
}
bool SQLiteWrapper::executeP(const std::string& sql, const std::vector<std::string>& params) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(stmt, static_cast<int>(i + 1), params[i].c_str(), -1, SQLITE_STATIC);
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}
std::vector<std::vector<std::string>> SQLiteWrapper::query(const std::string& query) {
    std::vector<std::vector<std::string>> results;
    if (sqlite3_exec(db, query.c_str(), callback, &results, &errorMessage) != SQLITE_OK) {
        std::cerr << "SQL error: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
    return results;
}

int SQLiteWrapper::callback(void* data, int argc, char** argv, char** colName) {
    auto* results = static_cast<std::vector<std::vector<std::string>>*>(data);
    std::vector<std::string> row;

    for (int i = 0; i < argc; i++) {
        row.emplace_back(argv[i] ? argv[i] : "NULL");
    }

    results->push_back(row);
    return 0;
}
