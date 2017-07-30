#pragma once
namespace MiniEngine
{
namespace _internal
{

/// This is just an empty class.
/// Most wrapper class regard this class as a friend class.
/// You can get/set raw pointers through this class.
class Plugin
{
public:
    template<typename T>
    static decltype(auto) get(const T& obj)
    {
        return obj._get();
    }
    template<typename T,typename U>
    static void set(T& obj,U&& value)
    {
        obj._set(value);
    }
    template<typename T>
    static void clear(T& obj)
    {
        obj._clear();
    }

    template<typename T,typename U>
    static void set_no_delete(T& obj,U&& value)
    {
        obj._set_no_delete(value);
    }
};

}
} /// End of namespace MiniEngine 

