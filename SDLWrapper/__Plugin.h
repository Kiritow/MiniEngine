#pragma once
#include "begin_code.h"
namespace _internal
{

/// This is just an empty class.
/// Most wrapper class regard this class as a friend class.
/// You can get/set raw pointers through this class.
class Plugin
{
public:
    template<typename T>
    decltype(auto) get(const T& obj)
    {
        return obj._get();
    }
    template<typename T,typename U>
    void set(T& obj,U&& value)
    {
        obj._set(value);
    }
    template<typename T>
    void clear(T& obj)
    {
        obj._clear();
    }

    template<typename T,typename U>
    void set_no_delete(T& obj,U&& value)
    {
        obj._set_no_delete(value);
    }
};

}
#include "end_code.h"
