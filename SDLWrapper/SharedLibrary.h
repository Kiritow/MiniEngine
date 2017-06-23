#pragma once
#include "include.h"
#include <string>
#include <memory>
#include "begin_code.h"
class SharedLibrary
{
public:
    SharedLibrary();
    SharedLibrary(const std::string& Filename);
    ~SharedLibrary()=default;
    int load(const std::string& Filename);
    int unload();

    template<typename ReturnType,typename... Arguments>
    std::function<ReturnType(Arguments...)> get(const std::string& FunctionName)
    {
        return std::function<ReturnType(Arguments...)>(reinterpret_cast<ReturnType(*)(Arguments...)>(get(FunctionName)));
    }

    void* get(const std::string& FunctionName) const;
    void release();
private:
    void* _get() const;
    void _set(void*);
    void _clear();
    std::shared_ptr<void> _obj;
};
#include "end_code.h"
