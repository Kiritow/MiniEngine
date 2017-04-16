#pragma once
#include "sqlite/sqlite3.h"
#include <string>
#include <memory>

class SQLStatement
{
public:
    bool isReady() const;
    SQLStatement()=default;
    ~SQLStatement()=default;

    void _setStmt(sqlite3_stmt*);
    sqlite3_stmt* _getStmt() const;
private:
    sqlite3_stmt* _get() const;
    void _set(sqlite3_stmt*);
    std::shared_ptr<sqlite3_stmt> _st;
};


using SQLCallback = int (*)(void* ExParam,int colNum,char** colVal,char** colName);

class SQLDB
{
public:
    SQLDB()=default;
    ~SQLDB()=default;

    bool isOpened();
    int open(const std::string& filename);
    SQLStatement prepare(const std::string& SQLCommand);
    int step(const SQLStatement& Statement);
    int exec(const std::string& SQLCommand,SQLCallback callback,void* param);
private:
    sqlite3* _get();
    void _set(sqlite3*);
    std::shared_ptr<sqlite3> _db;
};


