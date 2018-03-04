#pragma once
#include "include.h"
#include <cstdio>
#include <string>
#include <memory>
#include "__Plugin.h"

namespace MiniEngine {

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
    friend class Sound;
    friend class Music;

    friend class _internal::Plugin;
};

} /// End of namespace MiniEngine
