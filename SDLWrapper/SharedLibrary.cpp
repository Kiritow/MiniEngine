#include "SharedLibrary.h"
#include "begin_code.h"
//private
void* SharedLibrary::_get() const
{
    return _obj.get();
}

//private
void SharedLibrary::_set(void* ptr)
{
    _obj.reset(ptr,SDL_UnloadObject);
}

//private
void SharedLibrary::_clear()
{
    _obj.reset();
}

SharedLibrary::SharedLibrary()
{
    _obj=nullptr;
}

SharedLibrary::SharedLibrary(const std::string& Filename)
{
    _obj=nullptr;
    load(Filename);
}

int SharedLibrary::load(const std::string& Filename)
{
    if(_get()!=nullptr) return -1; /// Loaded
    else
    {
        void* ptr=SDL_LoadObject(Filename.c_str());
        if(ptr)
        {
            _set(ptr);
            return 0; /// Success
        }
        else return -2; /// Failed to load
    }
}

int SharedLibrary::unload()
{
    if(_get()!=nullptr)
    {
        _clear();
        return 0; /// Success to unload
    }
    else return -1; /// Not Loaded.
}

void* SharedLibrary::get(const std::string& FunctionName) const
{
    if(_get()==nullptr) return nullptr;
    else return SDL_LoadFunction(_get(),FunctionName.c_str());
}

void SharedLibrary::release()
{
    _clear();
}
#include "end_code.h"
