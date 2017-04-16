#include "MiniEngine_SQLite.h"
#include <cstring>
#include <cstdio>

namespace MiniEngine
{

namespace SQL
{

// private
void SQLStatement::_set(sqlite3_stmt* p)
{
    _st.reset(p,sqlite3_finalize);
}

// private
sqlite3_stmt* SQLStatement::_get() const
{
    return _st.get();
}

void SQLStatement::_setStmt(sqlite3_stmt* p)
{
    _set(p);
}

sqlite3_stmt* SQLStatement::_getStmt() const
{
    return _get();
}

bool SQLStatement::isReady() const
{
    return _get()!=nullptr;
}


void SQLDB::_set(sqlite3* p)
{
    _db.reset(p,sqlite3_close);
}

sqlite3* SQLDB::_get()
{
    return _db.get();
}

int SQLDB::open(const std::string& filename)
{
    sqlite3* _temp=nullptr;
    int ret=sqlite3_open(filename.c_str(),&_temp);
    _set(_temp);
    return ret;
}

SQLStatement SQLDB::prepare(const std::string& SQLCommand)
{
    SQLStatement stmt;
    sqlite3_stmt* pstmt=nullptr;
    int ret=sqlite3_prepare(_get(),SQLCommand.c_str(),SQLCommand.size(),&pstmt,NULL);
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
    return sqlite3_exec(_get(),SQLCommand.c_str(),callback,param,&_errmsg);
}

const char* SQLDB::getErrorMsg()
{
    return _errmsg;
}

void SQLDB::clearError()
{
    _errmsg=nullptr;
}

}/// End of namespace MiniEngine::SQL

}/// End of namespace MiniEngine
