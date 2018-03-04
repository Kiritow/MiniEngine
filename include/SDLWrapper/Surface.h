#pragma once
#include "include.h"
#include "BlendMode.h"
#include "RGBA.h"
#include "Point.h"
#include "RWOP.h"
#include "ErrorViewer.h"
#include "__Plugin.h"

namespace MiniEngine {

class Surface
{
public:
    Surface()=default;
    Surface(int width,int height,int depth,int Rmask,int Gmask,int Bmask,int Amask);
    Surface(int width,int height,int depth,RGBA colorPack);
    Surface(int width,int height,int depth,Uint32 surfaceFormat);
    Surface(const std::string& filename);
    Surface(const RWOP& rwop);
    ~Surface() = default;

    /// static functions
    static Surface load(const std::string& filename);
    static Surface load(const RWOP& rwop);
    static Surface create(int width,int height,int depth,int Rmask,int Gmask,int Bmask,int Amask);
    static Surface create(int width,int height,int depth,Uint32 surfaceFormat);

    /// xxxAs will clear the current surface if loaded or created successfully.
    int loadAs(const std::string& filename);
    int loadAs(const RWOP& rwop);
    int createAs(int width,int height,int depth,int Rmask,int Gmask,int Bmask,int Amask);
    int createAs(int width,int height,int depth,Uint32 surfaceFormat);

    int savePNG(const std::string& filename) const;
    int getw() const;
    int geth() const;
    BlendMode getBlendMode() const;
    int setBlendMode(BlendMode mode);

    /// Rendering functions. Copy an external surface to this surface. So it has no constant attribute.
    int blit(const Surface& s,const Rect& src,const Rect& dst);
    int blitTo(const Surface& t, const Rect& dst);
    int blitTo(const Surface& t, const Point& lupoint);
    int blitFill(const Surface& t, const Rect& src);
    int blitFullFill(const Surface& t);

    int blitScaled(const Surface& s,const Rect& src,const Rect& dst);
    int blitScaledTo(const Surface& t, const Rect& dst);
    int blitScaledTo(const Surface& t, const Point& lupoint);
    int blitScaledFill(const Surface& t, const Rect& src);
    int blitScaledFullFill(const Surface& t);

    int setClipRect(const Rect& clipRect);
    Rect getClipRect() const;
    void disableClipping();

    int setAlphaMode(int alpha);
    int getAlphaMode() const;

    ColorMode getColorMode() const;
    int setColorMode(ColorMode mode);
    RGBA getRGBA() const;
    void setRGBA(const RGBA& pack);

    bool mustlock() const;
    int lock();
    void unlock();

    bool isReady() const;
    void release();

    /// Experimental : Get SDL_Surface Pointer and then do anything you want!
    /// In case you can do anything with this pointer, this function should not has constant attribute.
    SDL_Surface* getRawPointer();
private:
    std::shared_ptr<SDL_Surface> _surf;
    void _set(SDL_Surface*);
    void _set_no_delete(SDL_Surface*);
    void _clear();
    SDL_Surface* _get() const;

    friend class Window;
    friend class Renderer;
    friend class Font;
    friend class Cursor;

    friend class _internal::Plugin;
};

} /// End of namespace MiniEngine
