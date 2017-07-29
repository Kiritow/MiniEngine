#pragma once
#include <functional>

namespace MiniEngine
{

class ResourceHandler
{
public:
    using RawDeleter = std::function<void(void*)>;

    template<typename T>
    ResourceHandler(T* ResourcePtr,RawDeleter RawDelFunc) :
        _ptr(ResourcePtr),
        _delfunc(RawDelFunc) {}

    ~ResourceHandler()
    {
        _delfunc(_ptr);
    }

    template<typename T>
    T* get() const
    {
        return (T*)(_ptr);
    }

    template<typename T>
    const T* cget() const
    {
        return (const T*)(_ptr);
    }
private:
    void* _ptr;
    RawDeleter _delfunc;
};

}/// End of namespace MiniEngine
