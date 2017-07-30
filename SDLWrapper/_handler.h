#pragma once
#include <functional>

namespace MiniEngine
{

template<typename T>
class res_ptr
{
public:
    res_ptr() : _ptr(nullptr) {}
    res_ptr(T* ptr,const std::function<void(T*)>& delFunc) : _ptr(ptr), _delfunc(delFunc) {}
    res_ptr(const res_ptr&)=delete;
    res_ptr& operator = (const res_ptr&)=delete;
    res_ptr(res_ptr&& t)
    {
        _ptr=t._ptr;
        _delfunc=t._delfunc;
        t._ptr=nullptr;
    }
    res_ptr& operator = (res_ptr&& t)
    {
        release();
        _ptr=t._ptr;
        _delfunc=t._delfunc;
        t._ptr=nullptr;
        return *this;
    }
    ~res_ptr()
    {
        release();
    }

    void release()
    {
        if(_ptr)
        {
            _delfunc(_ptr);
            _ptr=nullptr;
        }
    }
    void reset(T* ptr,const std::function<void(T*)>& delFunc)
    {
        release();
        _ptr=ptr;
        _delfunc=delFunc;
    }
    T* get()
    {
        return _ptr;
    }
    const T* get() const
    {
        return _ptr;
    }
    T* operator -> ()
    {
        return _ptr;
    }

private:
    T* _ptr;
    std::function<void(T*)> _delfunc;
};

}/// End of namespace MiniEngine
