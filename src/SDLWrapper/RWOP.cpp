#include "RWOP.h"
#include "begin_code.h"
// private
void RWOP::_set(SDL_RWops* p)
{
    _op.reset(p,[](SDL_RWops* p)
    {
        SDL_RWclose(p);
    });
}

// private
SDL_RWops* RWOP::_get() const
{
    return _op.get();
}

void RWOP::_clear()
{
    _op.reset();
}

RWOP::RWOP(FILE* fp,bool autoclose)
{
    SDL_bool b=autoclose?SDL_TRUE:SDL_FALSE;
    _set(SDL_RWFromFP(fp,b));
}

RWOP::RWOP(const std::string& filename,const std::string& openmode)
{
    _set(SDL_RWFromFile(filename.c_str(),openmode.c_str()));
}

RWOP::RWOP(const void* mem,int size)
{
    _set(SDL_RWFromConstMem(mem,size));
}

RWOP::RWOP(void* mem,int size)
{
    _set(SDL_RWFromMem(mem,size));
}

void RWOP::release()
{
    _clear();
}
#include "end_code.h"
