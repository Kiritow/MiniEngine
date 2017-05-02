#pragma once
#include "sqlite/sqlite3.h"
#include <string>
#include <memory>

namespace MiniEngine
{

namespace SQL
{

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

/// Global Executor
int _global_sqldb_executor(void* ExParam,int colNum,char** colVal,char** colName);

class SQLDB
{
public:
    SQLDB()=default;
    ~SQLDB()=default;

    bool isOpened();
    int open(const std::string& filename);
    SQLStatement prepare(const std::string& SQLCommand);
    int step(const SQLStatement& Statement);

    template<typename Callable,typename... Args>
    int exec(const std::string& SQLCommand,Callable&& callable,Args&&... args)
    {
        auto realCall=[&](int colNum,char** colVal,char** colName)->int{return callable(colNum,colVal,colName,args...);};
        std::function<int(int,char**,char**)> func=realCall;
        return _exec_real(SQLCommand,_global_sqldb_executor,&func);
    }

    int exec(const std::string& SQLCommand);

    int exec_raw(const std::string& SQLCommand,SQLCallback callback,void* param);

    const char* getErrorMsg();
    void clearError();
private:
    int _exec_real(const std::string& SQLCommand,SQLCallback callback,void* param);

    sqlite3* _get();
    void _set(sqlite3*);
    std::shared_ptr<sqlite3> _db;
    char* _errmsg;
};

}/// End of namespace MiniEngine::SQL

}/// End of namespace MiniEngine
