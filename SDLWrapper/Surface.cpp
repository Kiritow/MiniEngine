#include "Surface.h"
#include "begin_code.h"
//private
void Surface::_set(SDL_Surface* p)
{
    _surf.reset(p,SDL_FreeSurface);
}

//private
void Surface::_clear()
{
    _surf.reset();
}

//private
SDL_Surface* Surface::_get() const
{
    return _surf.get();
}

//private
void Surface::_set_no_delete(SDL_Surface* p)
{
    _surf.reset(p,[](SDL_Surface*) {});
}

Surface::Surface(int width,int height,int depth,int Rmask,int Gmask,int Bmask,int Amask) throw(ErrorViewer)
{
    if(createAs(width,height,depth,Rmask,Gmask,Bmask,Amask)!=0)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
}

Surface::Surface(int width,int height,int depth,RGBA maskPack) throw (ErrorViewer)
    : Surface(width,height,depth,maskPack.r,maskPack.g,maskPack.b,maskPack.a)
{

}

Surface::Surface(int width,int height,int depth,Uint32 surfaceFormat) throw(ErrorViewer)
{
    if(createAs(width,height,depth,surfaceFormat)!=0)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
}

Surface::Surface(const std::string& filename) throw(ErrorViewer)
{
    if(loadAs(filename)!=0)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
}

Surface::Surface(const RWOP& rwop) throw (ErrorViewer)
{
    if(loadAs(rwop)!=0)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
}

//static
Surface Surface::load(const std::string& filename)
{
    Surface s;
    s.loadAs(filename);
    return s;
}

//static
Surface Surface::load(const RWOP& rwop)
{
    Surface s;
    s.loadAs(rwop);
    return s;
}

//static
Surface Surface::create(int width, int height, int depth, int Rmask, int Gmask, int Bmask, int Amask)
{
    Surface s;
    s.create(width,height,depth,Rmask,Gmask,Bmask,Amask);
    return s;
}

//static
Surface Surface::create(int width, int height, int depth, Uint32 surfaceFormat)
{
    Surface s;
    s.create(width,height,depth,surfaceFormat);
    return s;
}


int Surface::loadAs(const std::string& filename)
{
    SDL_Surface* temp=IMG_Load(filename.c_str());
    if(temp==nullptr)
    {
        return -1;
    }
    else
    {
        _set(temp);
        return 0;
    }
}

int Surface::loadAs(const RWOP& rwop)
{
    SDL_Surface* temp=IMG_Load_RW(rwop._get(),0);
    if(temp==nullptr)
    {
        return -1;
    }
    else
    {
        _set(temp);
        return 0;
    }
}

int Surface::createAs(int width,int height,int depth,int Rmask,int Gmask,int Bmask,int Amask)
{
    SDL_Surface* temp=SDL_CreateRGBSurface(0,width,height,depth,Rmask,Gmask,Bmask,Amask);
    if(temp==nullptr)
    {
        return -1;
    }
    else
    {
        _set(temp);
        return 0;
    }
}

int Surface::createAs(int width,int height,int depth,Uint32 surfaceFormat)
{
    /// FIXME: This Function is available from SDL2.0.5. But the linker report a undefined reference.

    /*
    SDL_Surface* temp=SDL_CreateRGBSurfaceWithFormat(0,width,height,depth,surfaceFormat);
    if(temp==nullptr)
    {
        return -1;
    }
    else
    {
        _set(temp);
        return 0;
    }
    */

    SDL_SetError("[MiniEngine] SDL_CreateRGBSurfaceWithFormat Not Linked.");
    return -1;
}

int Surface::getw() const
{
    if(_get()!=nullptr)
    {
        return _get()->w;
    }
    else
    {
        /// return -1 on null surface pointer
        return -1;
    }
}

int Surface::geth() const
{
    if(_get()!=nullptr)
    {
        return _get()->h;
    }
    else
    {
        /// return -1 on null surface pointer
        return -1;
    }
}

BlendMode Surface::getBlendMode() const
{
    SDL_BlendMode temp;
    /// FIXME: return value are ignored.
    SDL_GetSurfaceBlendMode(_get(),&temp);
    return _internal::getBlendModeFromSDLBlendMode(temp);
}

int Surface::setBlendMode(BlendMode mode)
{
    return SDL_SetSurfaceBlendMode(_get(),_internal::getSDLBlendModeFromBlendMode(mode));
}

int Surface::savePNG(const std::string& filename) const
{
    return IMG_SavePNG(_get(),filename.c_str());
}

int Surface::blit(const Surface& s,const Rect& src,const Rect& dst)
{
    SDL_Rect rsrc=src.toSDLRect();
    SDL_Rect rdst=dst.toSDLRect();
    return SDL_BlitSurface(s._get(),&rsrc,_get(),&rdst);
}

int Surface::blitTo(const Surface& s,const Rect& dst)
{
    SDL_Rect rdst=dst.toSDLRect();
    return SDL_BlitSurface(s._get(),NULL,_get(),&rdst);
}

int Surface::blitTo(const Surface& s,const Point& lupoint)
{
    return blitTo(s,Rect(lupoint.x,lupoint.y,s.getw(),s.geth()));
}

int Surface::blitFill(const Surface& s,const Rect& src)
{
    SDL_Rect rsrc=src.toSDLRect();
    return SDL_BlitSurface(s._get(),&rsrc,_get(),NULL);
}

int Surface::blitFullFill(const Surface& s)
{
    return SDL_BlitSurface(s._get(),NULL,_get(),NULL);
}

int Surface::blitScaled(const Surface& s,const Rect& src,const Rect& dst)
{
    SDL_Rect rsrc=src.toSDLRect();
    SDL_Rect rdst=dst.toSDLRect();
    return SDL_BlitScaled(s._get(),&rsrc,_get(),&rdst);
}

int Surface::blitScaledTo(const Surface& s,const Rect& dst)
{
    SDL_Rect rdst=dst.toSDLRect();
    return SDL_BlitScaled(s._get(),NULL,_get(),&rdst);
}

int Surface::blitScaledTo(const Surface& s,const Point& lupoint)
{
    return blitScaledTo(s,Rect(lupoint.x,lupoint.y,s.getw(),s.geth()));
}

int Surface::blitScaledFill(const Surface& s,const Rect& src)
{
    SDL_Rect rsrc=src.toSDLRect();
    return SDL_BlitScaled(s._get(),&rsrc,_get(),NULL);
}

int Surface::blitScaledFullFill(const Surface& s)
{
    return SDL_BlitScaled(s._get(),NULL,_get(),NULL);
}

int Surface::setClipRect(const Rect& clipRect)
{
    auto m=clipRect.toSDLRect();
    return (SDL_SetClipRect(_get(),&m) == SDL_TRUE) ? 0 : -1;
}

Rect Surface::getClipRect() const
{
    SDL_Rect rect;
    SDL_GetClipRect(_get(),&rect);
    return Rect(rect);
}

void Surface::disableClipping()
{
    SDL_SetClipRect(_get(),NULL);
}

int Surface::setAlphaMode(int alpha)
{
    return SDL_SetSurfaceAlphaMod(_get(),alpha);
}

int Surface::getAlphaMode() const
{
    Uint8 al;
    SDL_GetSurfaceAlphaMod(_get(),&al);
    return al;
}

int Surface::setColorMode(ColorMode mode)
{
    return SDL_SetSurfaceColorMod(_get(),mode.r,mode.g,mode.b);
}

ColorMode Surface::getColorMode() const
{
    Uint8 r,g,b;
    SDL_GetSurfaceColorMod(_get(),&r,&g,&b);
    ColorMode mode;
    mode.r=r;
    mode.g=g;
    mode.b=b;
    return mode;
}

void Surface::setRGBA(const RGBA& pack)
{
    setColorMode(pack.toColorMode());
    setAlphaMode(pack.a);
}

RGBA Surface::getRGBA() const
{
    return RGBA(getColorMode(),getAlphaMode());
}

bool Surface::mustlock() const
{
    return SDL_MUSTLOCK(_get());
}

int Surface::lock()
{
    return SDL_LockSurface(_get());
}

void Surface::unlock()
{
    SDL_UnlockSurface(_get());
}

bool Surface::isReady() const
{
    return _get()!=nullptr;
}

void Surface::release()
{
    _clear();
}

/// Experimental (Not constant)
SDL_Surface* Surface::getRawPointer()
{
    return _get();
}
#include "end_code.h"
