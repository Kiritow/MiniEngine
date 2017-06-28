#pragma once
#include "include.h"
#include "_RendererType.h"
#include "_FlipMode.h"
#include "Window.h"
#include "Surface.h"
#include "Texture.h"
#include <initializer_list>
#include "begin_code.h"
class Renderer
{
public:
    Renderer() = default;
    /// Create a Renderer associated with Window
    Renderer(Window& wnd,std::initializer_list<RendererType> RendererFlags = { RendererType::Accelerated,RendererType::TargetTexture }) throw (ErrorViewer);
    /// Create a software Renderer
    Renderer(Surface& surf) throw (ErrorViewer);
    ~Renderer() = default;

    /// If Renderer is current not ready, setRenderer will create Renderer.
    /// Otherwise, setRenderer will fail.
    int createRenderer(Window& wnd,RendererType Type)
    {
        int flagcalc=0;
        return _createRenderer(wnd,flagcalc,Type);
    }

    template<typename... Args>
    int createRenderer(Window& wnd,RendererType Type,Args&&... args)
    {
        int flagcalc=0;
        return _createRenderer(wnd,flagcalc,Type,std::forward<RendererType>(args...));
    }

    int createRenderer(Window& wnd,std::initializer_list<RendererType>);

    int createSoftRenderer(Surface& surf);

    int setColor(const RGBA& pack);
    RGBA getColor() const;
    int setBlendMode(BlendMode mode);
    BlendMode getBlendMode() const;

    int setTarget(Texture& t);
    int setTarget();
    Texture getTarget();

    int fillRect(const Rect& rect);
    int drawRect(const Rect& rect);
    int drawPoint(const Point& p);
    int drawLine(const Point& A,const Point& B);

    /// Experimental
    int fillRects(const SDL_Rect* pRectArray,int n);
    int drawRects(const SDL_Rect* pRectArray,int n);
    int drawPoints(const SDL_Point* pPointArray,int n);
    int drawLines(const SDL_Point* pPointArray,int n);
    /// Experimental
    int fillRects(const std::vector<SDL_Rect>& rectvec);
    int drawRects(const std::vector<SDL_Rect>& rectvec);
    int drawPoints(const std::vector<SDL_Point>& pointvec);
    int drawLines(const std::vector<SDL_Point>& pointvec);

    /// Slower Functions (Need Convert First, then call Experimental Functions.)
    int fillRects(const std::vector<Rect>& rectvec);
    int drawRects(const std::vector<Rect>& rectvec);
    int drawPoints(const std::vector<Point>& pointvec);
    int drawLines(const std::vector<Point>& pointvec);

    int setScale(float scaleX,float scaleY);
    std::tuple<float,float> getScale() const;

    int setViewport(const Rect& viewport);
    Rect getViewport() const;

    int setLogicalSize(int w,int h);
    Rect getLogicalSize() const;

    int setClipRect(const Rect& cliprect);
    Rect getClipRect() const;
    bool isClipEnabled() const;

    Rect getOutputSize() const;

    int clear();
    void update();

    int copy(const Texture& t, const Rect& src, const Rect& dst);
    int copyTo(const Texture& t, const Rect& dst);
    int copyTo(const Texture& t, const Point& lupoint);
    int copyFill(const Texture& t, const Rect& src);
    int copyFullFill(const Texture& t);

    /// Super copy without center point.
    int copy(const Texture& t, const Rect& src, const Rect& dst,double angle,FlipMode mode);
    int copyTo(const Texture& t, const Rect& dst,double angle,FlipMode mode);
    int copyTo(const Texture& t, const Point& lupoint,double angle,FlipMode mode);
    int copyFill(const Texture& t, const Rect& src,double angle,FlipMode mode);
    int copyFullFill(const Texture& t,double angle,FlipMode mode);
    /// Super copy with center point
    int copy(const Texture& t, const Rect& src, const Rect& dst,const Point& centerPoint,double angle,FlipMode mode);
    int copyTo(const Texture& t, const Rect& dst,const Point& centerPoint,double angle,FlipMode mode);
    int copyTo(const Texture& t, const Point& lupoint,const Point& centerPoint,double angle,FlipMode mode);
    int copyFill(const Texture& t, const Rect& src,const Point& centerPoint,double angle,FlipMode mode);
    int copyFullFill(const Texture& t,const Point& centerPoint,double angle,FlipMode mode);

    /// Reserved for compatibility
    int supercopy(const Texture& t,
                  bool srcfull,const Rect& src,bool dstfull,const Rect& dst,
                  double angle,
                  bool haspoint,const Point& center,FlipMode mode);

    Texture render(const Surface& surf) const throw (ErrorViewer);
    Texture loadTexture(const std::string& FileName) const throw(ErrorViewer);
    Texture loadTextureRW(const RWOP& rwop) const throw(ErrorViewer);
    Texture createTexture(int Width, int Height) const throw(ErrorViewer);

    bool isRenderTargetSupported() const;
    bool isReady() const;

    void release();

    /// Experimental
    static int GetDriversNum();
protected:
    template<typename... Args>
    int _createRenderer(Window& wnd,int& refcalc,RendererType Type,Args&&... args)
    {
        refcalc|=_rendertype_caster(Type);
        return _createRenderer(wnd,refcalc,args...);
    }

    // template<>
    int _createRenderer(Window& wnd,int& refcalc,RendererType Type)
    {
        refcalc|=_rendertype_caster(Type);
        return _createRenderer_Real(wnd,refcalc);
    }
private:
    std::shared_ptr<SDL_Renderer> _rnd;

    int _createRenderer_Real(Window& wnd,Uint32 flags);
    Uint32 _rendertype_caster(RendererType);

    void _set(SDL_Renderer*);
    void _clear();
    SDL_Renderer* _get() const;
};
#include "end_code.h"
