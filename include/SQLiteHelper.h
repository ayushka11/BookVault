#ifndef SQLITEHELPER_H
#define SQLITEHELPER_H

#include <sqlite3.h>
#include <string>
#include <vector>
using namespace std;

class SQLiteHelper {
private:
    sqlite3* db;

public:
    SQLiteHelper(const string& dbName);
    ~SQLiteHelper();
    bool executeQuery(const string& query);
    bool executeQueryWithParams(const string& query, const vector<string>& params);
    sqlite3* getDb();
    int executeScalarQueryWithParams(const string& query, const vector<string>& params);
};

#endif 
