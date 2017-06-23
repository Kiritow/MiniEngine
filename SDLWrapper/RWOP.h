#pragma once
#include "include.h"
#include <cstdio>
#include <string>
#include <memory>
#include "begin_code.h"
class RWOP
{
public:
    RWOP(FILE* fp,bool autoclose);
    RWOP(const std::string& filename,const std::string& openmode);
    RWOP(const void* mem,int size);
    RWOP(void* mem,int size);
    RWOP()=default;
    ~RWOP()=default;

    void release();
private:
    std::shared_ptr<SDL_RWops> _op;
    SDL_RWops* _get() const;
    void _clear();
    void _set(SDL_RWops*);
    friend class Surface;
    friend class Renderer;
};
#include "end_code.h"