#include "sqlite_wrapper.h"
#include <cstring>
#include <cstdio>

SQLStatement::SQLStatement()
{
    _st=nullptr;
}

SQLStatement::~SQLStatement()
{
    sqlite3_finalize(_st);
}

void SQLStatement::_setStmt(sqlite3_stmt* p)
{
    _st=p;
}

sqlite3_stmt* SQLStatement::_getStmt() const
{
    return _st;
}


SQLDB::SQLDB()
{
    _db=nullptr;
}

SQLDB::~SQLDB()
{
    sqlite3_close(_db);
}

int SQLDB::open(const std::string& filename)
{
    return sqlite3_open(filename.c_str(),&_db);
}

SQLStatement SQLDB::prepare(const std::string& SQLCommand)
{
    SQLStatement stmt;
    sqlite3_stmt* pstmt=nullptr;
    int ret=sqlite3_prepare(_db,SQLCommand.c_str(),SQLCommand.size(),&pstmt,NULL);
    if(ret<0) return stmt;
    stmt._setStmt(pstmt);
    return stmt;
}

int SQLDB::step(const SQLStatement& Statement)
{
    return sqlite3_step(Statement._getStmt());
}

int SQLDB::exec(const std::string& SQLCommand,SQLCallback callback,void* param)
{
    char* errmsg=nullptr;
    int ret=sqlite3_exec(_db,SQLCommand.c_str(),callback,param,&errmsg);
    printf("ErrMsg: %s\n",errmsg);
    return ret;
}
