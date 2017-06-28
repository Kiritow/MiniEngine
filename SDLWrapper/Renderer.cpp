#include "Renderer.h"
#include "_caster.h"
#include "begin_code.h"

//private
void Renderer::_set(SDL_Renderer* p)
{
    _rnd.reset(p,SDL_DestroyRenderer);
}

//private
void Renderer::_clear()
{
    _rnd.reset();
}

//private
SDL_Renderer* Renderer::_get() const
{
    return _rnd.get();
}

Renderer::Renderer(Window& wnd,std::initializer_list<RendererType> RendererFlags) throw (ErrorViewer)
{
    if(createRenderer(wnd,RendererFlags)!=0)
    {
        throw ErrorViewer();
    }
}

int Renderer::createRenderer(Window& wnd,std::initializer_list<RendererType> RendererFlags)
{
    Uint32 flag = 0;
    for (auto v : RendererFlags)
    {
        flag |= _rendertype_caster(v);
    }
    return _createRenderer_Real(wnd,flag);
}

int Renderer::setColor(const RGBA& pack)
{
    return SDL_SetRenderDrawColor(_get(), pack.r, pack.g, pack.b, pack.a);
}

RGBA Renderer::getColor() const
{
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(_get(), &r, &g, &b, &a);
    return RGBA(r, g, b, a);
}

int Renderer::setBlendMode(BlendMode mode)
{
    return SDL_SetRenderDrawBlendMode(_get(), _internal::getSDLBlendModeFromBlendMode(mode));
}

BlendMode Renderer::getBlendMode() const
{
    SDL_BlendMode temp;
    SDL_GetRenderDrawBlendMode(_get(), &temp);
    return _internal::getBlendModeFromSDLBlendMode(temp);
}

int Renderer::setTarget(Texture & t)
{
    return SDL_SetRenderTarget(_get(), t._get());
}

int Renderer::setTarget()
{
    return SDL_SetRenderTarget(_get(), nullptr);
}

Texture Renderer::getTarget()
{
    Texture t;
    t._set_no_delete(SDL_GetRenderTarget(_get()));
    return t;
}

int Renderer::fillRect(const Rect& rect)
{
    auto inr = rect.toSDLRect();
    return SDL_RenderFillRect(_get(), &inr);
}

int Renderer::drawRect(const Rect& rect)
{
    auto inr = rect.toSDLRect();
    return SDL_RenderDrawRect(_get(), &inr);
}

int Renderer::drawPoint(const Point& p)
{
    return SDL_RenderDrawPoint(_get(),p.x,p.y);
}

int Renderer::drawLine(const Point& A,const Point& B)
{
    return SDL_RenderDrawLine(_get(),A.x,A.y,B.x,B.y);
}

int Renderer::fillRects(const SDL_Rect* pRectArray, int n)
{
    return SDL_RenderFillRects(_get(),pRectArray,n);
}

int Renderer::drawRects(const SDL_Rect* pRectArray, int n)
{
    return SDL_RenderDrawRects(_get(),pRectArray,n);
}

int Renderer::drawPoints(const SDL_Point* pPointArray, int n)
{
    return SDL_RenderDrawPoints(_get(),pPointArray,n);
}

int Renderer::drawLines(const SDL_Point* pPointArray, int n)
{
    return SDL_RenderDrawLines(_get(),pPointArray,n);
}

int Renderer::fillRects(const std::vector<SDL_Rect>& rectvec)
{
    return fillRects(rectvec.data(),rectvec.size());
}

int Renderer::drawRects(const std::vector<SDL_Rect>& rectvec)
{
    return drawRects(rectvec.data(),rectvec.size());
}

int Renderer::drawPoints(const std::vector<SDL_Point>& pointvec)
{
    return drawPoints(pointvec.data(),pointvec.size());
}

int Renderer::drawLines(const std::vector<SDL_Point>& pointvec)
{
    return drawLines(pointvec.data(),pointvec.size());
}

int Renderer::fillRects(const std::vector<Rect>& rectvec)
{
    std::vector<SDL_Rect> thisvec;
    for(auto& rectref:rectvec)
    {
        thisvec.push_back(rectref.toSDLRect());
    }
    return fillRects(thisvec);
}

int Renderer::drawRects(const std::vector<Rect>& rectvec)
{
    std::vector<SDL_Rect> thisvec;
    for(auto& rectref:rectvec)
    {
        thisvec.push_back(rectref.toSDLRect());
    }
    return drawRects(thisvec);
}

int Renderer::drawPoints(const std::vector<Point>& pointvec)
{
    std::vector<SDL_Point> thisvec;
    for(auto& pointref:pointvec)
    {
        thisvec.push_back(pointref.toSDLPoint());
    }
    return drawPoints(thisvec);
}

int Renderer::drawLines(const std::vector<Point>& pointvec)
{
    std::vector<SDL_Point> thisvec;
    for(auto& pointref:pointvec)
    {
        thisvec.push_back(pointref.toSDLPoint());
    }
    return drawLines(thisvec);
}


int Renderer::setScale(float scaleX, float scaleY)
{
    return SDL_RenderSetScale(_get(),scaleX,scaleY);
}

std::tuple<float,float> Renderer::getScale() const
{
    float sx,sy;
    SDL_RenderGetScale(_get(),&sx,&sy);
    return std::make_tuple(sx,sy);
}

int Renderer::setViewport(const Rect& viewport)
{
    auto rect=viewport.toSDLRect();
    return SDL_RenderSetViewport(_get(),&rect);
}

Rect Renderer::getViewport() const
{
    SDL_Rect rect;
    SDL_RenderGetViewport(_get(),&rect);
    return Rect(rect);
}

int Renderer::setLogicalSize(int w, int h)
{
    return SDL_RenderSetLogicalSize(_get(),w,h);
}

Rect Renderer::getLogicalSize() const
{
    int w,h;
    SDL_RenderGetLogicalSize(_get(),&w,&h);
    return Rect(0,0,w,h);
}

int Renderer::setClipRect(const Rect& cliprect)
{
    auto r=cliprect.toSDLRect();
    return SDL_RenderSetClipRect(_get(),&r);
}

Rect Renderer::getClipRect() const
{
    SDL_Rect r;
    SDL_RenderGetClipRect(_get(),&r);
    return Rect(r);
}

bool Renderer::isClipEnabled() const
{
    return (SDL_RenderIsClipEnabled(_get())==SDL_TRUE);
}

Rect Renderer::getOutputSize() const
{
    int w,h;
    SDL_GetRendererOutputSize(_get(),&w,&h);
    return Rect(0,0,w,h);
}

int Renderer::clear()
{
    return SDL_RenderClear(_get());
}

void Renderer::update()
{
    SDL_RenderPresent(_get());
}

int Renderer::copy(const Texture& t, const Rect& src, const Rect& dst)
{
    SDL_Rect s = src.toSDLRect();
    SDL_Rect d = dst.toSDLRect();
    return SDL_RenderCopy(_get(), t._get(), &s, &d);
}

int Renderer::copyTo(const Texture& t, const Rect& dst)
{
    SDL_Rect d = dst.toSDLRect();
    return SDL_RenderCopy(_get(), t._get(), NULL, &d);
}

int Renderer::copyTo(const Texture& t, const Point& lupoint)
{
    return copyTo(t, Rect(lupoint.x, lupoint.y, t.getw(), t.geth()));
}

int Renderer::copyFill(const Texture& t, const Rect& src)
{
    SDL_Rect s = src.toSDLRect();
    return SDL_RenderCopy(_get(), t._get(), &s, NULL);
}

int Renderer::copyFullFill(const Texture& t)
{
    return SDL_RenderCopy(_get(), t._get(), NULL, NULL);
}

/// ----- Super Copy Extend ----- (Begin)
int Renderer::copy(const Texture& t, const Rect& src, const Rect& dst, double angle, FlipMode mode)
{
    auto s=src.toSDLRect();
    auto d=src.toSDLRect();
    return SDL_RenderCopyEx(_get(),t._get(),&s,&d,angle,NULL,_internal::getSDLRendererFlipFromFlipMode(mode));
}

int Renderer::copyTo(const Texture& t, const Rect& dst, double angle, FlipMode mode)
{
    auto d=dst.toSDLRect();
    return SDL_RenderCopyEx(_get(),t._get(),NULL,&d,angle,NULL,_internal::getSDLRendererFlipFromFlipMode(mode));
}

int Renderer::copyTo(const Texture& t, const Point& lupoint, double angle, FlipMode mode)
{
    return copyTo(t,Rect(lupoint.x,lupoint.y,t.getw(),t.geth()),angle,mode);
}

int Renderer::copyFill(const Texture& t, const Rect& src, double angle, FlipMode mode)
{
    auto s=src.toSDLRect();
    return SDL_RenderCopyEx(_get(),t._get(),&s,NULL,angle,NULL,_internal::getSDLRendererFlipFromFlipMode(mode));
}

int Renderer::copyFullFill(const Texture& t, double angle, FlipMode mode)
{
    return SDL_RenderCopyEx(_get(),t._get(),NULL,NULL,angle,NULL,_internal::getSDLRendererFlipFromFlipMode(mode));
}

int Renderer::copy(const Texture& t, const Rect& src, const Rect& dst, const Point& centerPoint, double angle, FlipMode mode)
{
    auto s=src.toSDLRect();
    auto d=src.toSDLRect();
    auto c=centerPoint.toSDLPoint();
    return SDL_RenderCopyEx(_get(),t._get(),&s,&d,angle,&c,_internal::getSDLRendererFlipFromFlipMode(mode));
}

int Renderer::copyTo(const Texture& t, const Rect& dst, const Point& centerPoint, double angle, FlipMode mode)
{
    auto d=dst.toSDLRect();
    auto c=centerPoint.toSDLPoint();
    return SDL_RenderCopyEx(_get(),t._get(),NULL,&d,angle,&c,_internal::getSDLRendererFlipFromFlipMode(mode));
}

int Renderer::copyTo(const Texture& t, const Point& lupoint, const Point& centerPoint, double angle, FlipMode mode)
{
    return copyTo(t,lupoint,centerPoint,angle,mode);
}

int Renderer::copyFill(const Texture& t, const Rect& src, const Point& centerPoint, double angle, FlipMode mode)
{
    auto s=src.toSDLRect();
    auto c=centerPoint.toSDLPoint();
    return SDL_RenderCopyEx(_get(),t._get(),&s,NULL,angle,&c,_internal::getSDLRendererFlipFromFlipMode(mode));
}

int Renderer::copyFullFill(const Texture& t, const Point& centerPoint, double angle, FlipMode mode)
{
    auto c=centerPoint.toSDLPoint();
    return SDL_RenderCopyEx(_get(),t._get(),NULL,NULL,angle,&c,_internal::getSDLRendererFlipFromFlipMode(mode));
}
/// ----- Super Copy Extend ----- (End)

int Renderer::supercopy(const Texture& t,
                        bool srcfull,const Rect& src,bool dstfull,const Rect& dst,
                        double angle,
                        bool haspoint,const Point& center,FlipMode mode)
{
    SDL_Rect R1,R2;
    SDL_Point P;
    SDL_Rect* pR1=nullptr;
    SDL_Rect* pR2=nullptr;
    SDL_Point* pPoint=nullptr;
    SDL_RendererFlip flip;
    if(srcfull)
    {
        R1=src.toSDLRect();
        pR1=&R1;
    }
    if(dstfull)
    {
        R2=dst.toSDLRect();
        pR2=&R2;
    }
    if(haspoint)
    {
        P=center.toSDLPoint();
        pPoint=&P;
    }

    flip=_internal::getSDLRendererFlipFromFlipMode(mode);

    return SDL_RenderCopyEx(_get(),t._get(),pR1,pR2,angle,pPoint,flip);
}

Texture Renderer::render(const Surface& surf) const throw(ErrorViewer)
{
    Texture t;
    SDL_Texture* temp = SDL_CreateTextureFromSurface(_get(), surf._get());
    if (temp == nullptr)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
    t._set(temp);
    return t;
}

Texture Renderer::loadTexture(const std::string& FileName) const throw(ErrorViewer)
{
    Texture t;
    SDL_Texture* temp = IMG_LoadTexture(_get(), FileName.c_str());
    if (temp == nullptr)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
    t._set(temp);
    return t;
}

Texture Renderer::loadTextureRW(const RWOP& rwop) const throw (ErrorViewer)
{
    Texture t;
    SDL_Texture* temp=IMG_LoadTexture_RW(_get(),rwop._get(),0);
    if (temp == nullptr)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
    t._set(temp);
    return t;
}

Texture Renderer::createTexture(int Width, int Height) const throw(ErrorViewer)
{
    SDL_Texture* temp = SDL_CreateTexture(_get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Width, Height);
    if (temp == NULL)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
    Texture t;
    t._set(temp);
    return t;
}

bool Renderer::isRenderTargetSupported() const
{
    return (SDL_RenderTargetSupported(_get())==SDL_TRUE);
}

bool Renderer::isReady() const
{
    return (_get() != nullptr);
}

void Renderer::release()
{
    _clear();
}

//static
int Renderer::GetDriversNum()
{
    return SDL_GetNumRenderDrivers();
}

// private
Uint32 Renderer::_rendertype_caster(RendererType Type)
{
    switch(Type)
    {
    case RendererType::Accelerated:
        return SDL_RENDERER_ACCELERATED;
    case RendererType::PresentSync:
        return SDL_RENDERER_PRESENTVSYNC;
    case RendererType::Software:
        return SDL_RENDERER_SOFTWARE;
    case RendererType::TargetTexture:
        return SDL_RENDERER_TARGETTEXTURE;
    }

    /// If an error occurs, return 0 by default.
    return 0;
}

// private
int Renderer::_createRenderer_Real(Window& wnd,Uint32 flags)
{
    SDL_Renderer* pSDLRnd=SDL_CreateRenderer(wnd._get(), -1, flags);
    if(pSDLRnd!=nullptr)
    {
        _set(pSDLRnd);
        return 0;
    }
    else
    {
        return -1;
    }
}

#include "end_code.h"
