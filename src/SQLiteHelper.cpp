#include "../include/SQLiteHelper.h"
#include <iostream>
using namespace std;

SQLiteHelper::SQLiteHelper(const string& dbName) {
    if (sqlite3_open(dbName.c_str(), &db)) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
    }
}

SQLiteHelper::~SQLiteHelper() {
    sqlite3_close(db);
}

bool SQLiteHelper::executeQuery(const string& query) {
    char* errMessage = 0;
    if (sqlite3_exec(db, query.c_str(), 0, 0, &errMessage) != SQLITE_OK) {
        cerr << "SQL error: " << errMessage << endl;
        sqlite3_free(errMessage);
        return false;
    }
    return true;
}

bool SQLiteHelper::executeQueryWithParams(const string& query, const vector<string>& params) {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_STATIC);
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Execution failed: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

sqlite3* SQLiteHelper::getDb() {
    return db;
}

int SQLiteHelper::executeScalarQueryWithParams(const string& query, const vector<string>& params) {
    sqlite3_stmt* stmt;
    int result = -1; 

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, 0) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return result;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_STATIC);
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        result = sqlite3_column_int(stmt, 0);
    } else {
        cerr << "Failed to fetch scalar result: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
    return result;
}
