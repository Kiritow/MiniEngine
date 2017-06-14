#include "MiniEngine.h"
#include <algorithm>
#include <map>
#include <mutex>
#include <fstream>

#ifdef _MSC_VER /// Visual Studio
#define _MINIENGINE_HAS_UNISTD 0
#else
#define _MINIENGINE_HAS_UNISTD 1
#include <unistd.h>
#endif

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"

namespace MiniEngine
{
    namespace _internal
    {
        BlendMode getBlendModeFromSDLBlendMode(SDL_BlendMode mode)
        {
            switch(mode)
            {
            case SDL_BLENDMODE_ADD:
                return BlendMode::Add;
            case SDL_BLENDMODE_BLEND:
                return BlendMode::Blend;
            case SDL_BLENDMODE_MOD:
                return BlendMode::Mod;
            case SDL_BLENDMODE_NONE:
            default:/// return BlendMode::None on default.
                return BlendMode::None;
            }
        }

        SDL_BlendMode getSDLBlendModeFromBlendMode(BlendMode mode)
        {
            switch(mode)
            {
            case BlendMode::Add:
                return SDL_BLENDMODE_ADD;
            case BlendMode::Blend:
                return SDL_BLENDMODE_BLEND;
            case BlendMode::Mod:
                return SDL_BLENDMODE_MOD;
            case BlendMode::None:
            default:/// return SDL_BLENDMODE_NONE on default.
                return SDL_BLENDMODE_NONE;
            }
        }

        /// FIXME: return SDL_WindowFlags or Uint32 ?
        Uint32 getSDLWindowFlagsFromWindowType(WindowType type)
        {
            switch(type)
            {
            case WindowType::FullScreen:
                return SDL_WINDOW_FULLSCREEN;
            case WindowType::OpenGL:
                return SDL_WINDOW_OPENGL;
            case WindowType::Shown:
                return SDL_WINDOW_SHOWN;
            case WindowType::Hidden:
                return SDL_WINDOW_HIDDEN;
            case WindowType::Borderless:
                return SDL_WINDOW_BORDERLESS;
            case WindowType::Resizable:
                return SDL_WINDOW_RESIZABLE;
            case WindowType::Minimized:
                return SDL_WINDOW_MINIMIZED;
            case WindowType::Maximized:
                return SDL_WINDOW_MAXIMIZED;
            case WindowType::InputGrabbed:
                return SDL_WINDOW_INPUT_GRABBED;
            case WindowType::InputFocus:
                return SDL_WINDOW_INPUT_FOCUS;
            case WindowType::MouseFocus:
                return SDL_WINDOW_MOUSE_FOCUS;
            case WindowType::FullScreenDesktop:
                return SDL_WINDOW_FULLSCREEN_DESKTOP;
            case WindowType::Foreign:
                return SDL_WINDOW_FOREIGN;
            case WindowType::AllowHighDPI:
                return SDL_WINDOW_ALLOW_HIGHDPI;
            case WindowType::MouseCapture:
                return SDL_WINDOW_MOUSE_CAPTURE;

            #if _MINIENGINE_SDL_VERSION_ATLEAST(2,0,5) /// SDL 2.0.5 Required
            case WindowType::AlwaysOnTop:
                return SDL_WINDOW_ALWAYS_ON_TOP;
            case WindowType::SkipTaskBar:
                return SDL_WINDOW_SKIP_TASKBAR;
            case WindowType::Utility:
                return SDL_WINDOW_UTILITY;
            case WindowType::ToolTip:
                return SDL_WINDOW_TOOLTIP;
            case WindowType::PopUpMenu:
                return SDL_WINDOW_POPUP_MENU;
            #endif // End of SDL2.0.5 Require

            default:
                return 0;/// Return 0 on default.
            }
        }

        SystemCursorType getCursorTypeFromSDLSystemCursor(SDL_SystemCursor id)
        {
            switch(id)
            {
            case SDL_SYSTEM_CURSOR_ARROW:
                return SystemCursorType::Arrow;
            case SDL_SYSTEM_CURSOR_CROSSHAIR:
                return SystemCursorType::CrossHair;
            case SDL_SYSTEM_CURSOR_HAND:
                return SystemCursorType::Hand;
            case SDL_SYSTEM_CURSOR_IBEAM:
                return SystemCursorType::Ibeam;
            case SDL_SYSTEM_CURSOR_NO:
                return SystemCursorType::No;
            case SDL_SYSTEM_CURSOR_SIZEALL:
                return SystemCursorType::SizeAll;
            case SDL_SYSTEM_CURSOR_SIZENESW:
                return SystemCursorType::SizeNESW;
            case SDL_SYSTEM_CURSOR_SIZENS:
                return SystemCursorType::SizeNS;
            case SDL_SYSTEM_CURSOR_SIZENWSE:
                return SystemCursorType::SizeNWSE;
            case SDL_SYSTEM_CURSOR_SIZEWE:
                return SystemCursorType::SizeWE;
            case SDL_SYSTEM_CURSOR_WAIT:
                return SystemCursorType::Wait;
            case SDL_SYSTEM_CURSOR_WAITARROW:
                return SystemCursorType::WaitArrow;
            default:/// return SystemCursorType::Arrow on default.
                return SystemCursorType::Arrow;
            }
        }

        SDL_SystemCursor getSDLSystemCursorFromSystemCursorType(SystemCursorType type)
        {
            switch(type)
            {
            case SystemCursorType::Arrow:
                return SDL_SYSTEM_CURSOR_ARROW;
            case SystemCursorType::CrossHair:
                return SDL_SYSTEM_CURSOR_CROSSHAIR;
            case SystemCursorType::Hand:
                return SDL_SYSTEM_CURSOR_HAND;
            case SystemCursorType::Ibeam:
                return SDL_SYSTEM_CURSOR_IBEAM;
            case SystemCursorType::No:
                return SDL_SYSTEM_CURSOR_NO;
            case SystemCursorType::SizeAll:
                return SDL_SYSTEM_CURSOR_SIZEALL;
            case SystemCursorType::SizeNESW:
                return SDL_SYSTEM_CURSOR_SIZENESW;
            case SystemCursorType::SizeNS:
                return SDL_SYSTEM_CURSOR_SIZENS;
            case SystemCursorType::SizeNWSE:
                return SDL_SYSTEM_CURSOR_SIZENWSE;
            case SystemCursorType::SizeWE:
                return SDL_SYSTEM_CURSOR_SIZEWE;
            case SystemCursorType::Wait:
                return SDL_SYSTEM_CURSOR_WAIT;
            case SystemCursorType::WaitArrow:
                return SDL_SYSTEM_CURSOR_WAITARROW;
            default:/// return SDL_SYSTEM_CURSOR_ARROW on default.
                return SDL_SYSTEM_CURSOR_ARROW;
            }
        }

        int getTTFFontStyleFromFontStyle(FontStyle style)
        {
            switch(style)
            {
            case FontStyle::Bold:
                return TTF_STYLE_BOLD;
            case FontStyle::Italic:
                return TTF_STYLE_ITALIC;
            case FontStyle::Normal:
                return TTF_STYLE_NORMAL;
            case FontStyle::StrikeThrough:
                return TTF_STYLE_STRIKETHROUGH;
            case FontStyle::UnderLine:
                return TTF_STYLE_UNDERLINE;
            default:
                return TTF_STYLE_NORMAL;
            }
        }

        std::vector<FontStyle> getFontStyleVecFromMixedTTFFontStyle(int Mixed_TTF_Font_Style)
        {
            std::vector<FontStyle> vec;
            if(Mixed_TTF_Font_Style&TTF_STYLE_BOLD)
            {
                vec.push_back(FontStyle::Bold);
            }
            if(Mixed_TTF_Font_Style&TTF_STYLE_ITALIC)
            {
                vec.push_back(FontStyle::Italic);
            }
            if(Mixed_TTF_Font_Style&TTF_STYLE_STRIKETHROUGH)
            {
                vec.push_back(FontStyle::StrikeThrough);
            }
            if(Mixed_TTF_Font_Style&TTF_STYLE_UNDERLINE)
            {
                vec.push_back(FontStyle::UnderLine);
            }
            if(vec.empty())
            {
                vec.push_back(FontStyle::Normal);
            }

            return vec;
        }

        SDL_RendererFlip getSDLRendererFlipFromFlipMode(FlipMode mode)
        {
            switch(mode)
            {
            case FlipMode::None:
                return SDL_FLIP_NONE;
            case FlipMode::Horizontal:
                return SDL_FLIP_HORIZONTAL;
            case FlipMode::Vertical:
                return SDL_FLIP_VERTICAL;
            default:
                /// return non-flip mode on default.
                return SDL_FLIP_NONE;
            }
        }
    }/// End of namespace _internal

	Rect::Rect(int X, int Y, int W, int H)
	{
		x = X;
		y = Y;
		w = W;
		h = H;
	}

	Rect::Rect(const SDL_Rect& r):Rect(r.x,r.y,r.w,r.h)
	{

	}

	Rect::Rect()
	{
		x = y = w = h = 0;
	}

	SDL_Rect Rect::toSDLRect() const
	{
		SDL_Rect r;
		r.x = x;
		r.y = y;
		r.w = w;
		r.h = h;
		return r;
	}

	bool Rect::isEmpty()
	{
        SDL_Rect r=toSDLRect();
        return SDL_RectEmpty(&r)==SDL_TRUE;
	}

	bool Rect::operator == (const Rect& r) const
	{
        SDL_Rect a=toSDLRect(),b=r.toSDLRect();
        return SDL_RectEquals(&a,&b)==SDL_TRUE;
	}

	bool Rect::hasIntersection(const Rect& r) const
	{
        SDL_Rect a=toSDLRect(),b=r.toSDLRect();
        return SDL_HasIntersection(&a,&b)==SDL_TRUE;
	}

	Rect Rect::getIntersection(const Rect& r) const
	{
        SDL_Rect a=toSDLRect(),b=r.toSDLRect(),c;
        if(SDL_IntersectRect(&a,&b,&c)==SDL_TRUE)
        {
            return Rect(c);
        }
        else
        {
            return Rect();
        }
	}

	Rect Rect::getUnion(const Rect& r) const
	{
        SDL_Rect a=toSDLRect(),b=r.toSDLRect(),c;
        SDL_UnionRect(&a,&b,&c);//void
        return Rect(c);
	}

	Point::Point(int X, int Y)
	{
		x = X;
		y = Y;
	}

	Point::Point()
	{
		x = y = 0;
	}

	SDL_Point Point::toSDLPoint() const
	{
		SDL_Point p;
		p.x = x;
		p.y = y;
		return p;
	}

	bool Point::inRect(const Rect& rect) const
	{
		auto p = toSDLPoint();
		auto r = rect.toSDLRect();
		return ( SDL_PointInRect(&p, &r) == SDL_TRUE );
	}

	ColorMode::ColorMode(int R, int G, int B)
	{
		r = R;
		g = G;
		b = B;
	}

	ColorMode::ColorMode()
	{
		r = g = b = 0;
	}

	RGBA::RGBA(int R, int G, int B, int A)
	{
		r = R;
		g = G;
		b = B;
		a = A;
	}

	RGBA::RGBA(ColorMode mode, int A)
	{
		r = mode.r;
		g = mode.g;
		b = mode.b;
		a = A;
	}

	RGBA::RGBA()
	{
		r = g = b = a = 0;
	}

	SDL_Color RGBA::toSDLColor() const
	{
		SDL_Color c;
		c.r = r;
		c.g = g;
		c.b = b;
		c.a = a;
		return c;
	}

	ColorMode RGBA::toColorMode() const
	{
		return ColorMode(r, g, b);
	}

	void ErrorViewer::fetch()
	{
		str = SDL_GetError();
	}

	std::string ErrorViewer::getError() const
	{
		return str;
	}

	const char * ErrorViewer::what() const throw()
	{
		return str.c_str();
	}

	// private
    void RWOP::_set(SDL_RWops* p)
    {
        _op.reset(p,[](SDL_RWops* p){SDL_RWclose(p);});
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
        _surf.reset(p,[](SDL_Surface*){});
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

    //private
    void Texture::_set(SDL_Texture* p)
    {
        _text.reset(p,SDL_DestroyTexture);
        updateInfo();
    }

    //private
    void Texture::_set_no_delete(SDL_Texture* p)
    {
        _text.reset(p,[](SDL_Texture*){});
        updateInfo();
    }

    //private
    void Texture::_clear()
    {
        _text.reset();
        updateInfo();
    }

    //private
    SDL_Texture* Texture::_get() const
    {
        return _text.get();
    }

	Texture::Texture()
	{
        updateInfo();
	}

	Rect Texture::getSize()
	{
		return rect;
	}

	int Texture::getw() const
	{
		return rect.w;
	}

	int Texture::geth() const
	{
		return rect.h;
	}

	bool Texture::isReady() const
	{
		return (_get() != nullptr);
	}

	int Texture::setBlendMode(BlendMode mode)
	{
		return SDL_SetTextureBlendMode(_get(), _internal::getSDLBlendModeFromBlendMode(mode));
	}

	BlendMode Texture::getBlendMode() const
	{
		SDL_BlendMode temp;
		SDL_GetTextureBlendMode(_get(), &temp);
		return _internal::getBlendModeFromSDLBlendMode(temp);
	}

	/// Alpha:  0: Transparent 255: opaque

	int Texture::setAlphaMode(int alpha)
	{
		Uint8 temp = std::max(std::min(alpha, 255), 0);
		return SDL_SetTextureAlphaMod(_get(), temp);
	}

	int Texture::getAlphaMode() const
	{
		Uint8 temp;
		SDL_GetTextureAlphaMod(_get(), &temp);
		return temp;
	}

	ColorMode Texture::getColorMode() const
	{
		ColorMode pack;
		Uint8 r, g, b;
		SDL_GetTextureColorMod(_get(), &r, &g, &b);
		pack.r = r;
		pack.g = g;
		pack.b = b;
		return pack;
	}

	int Texture::setColorMode(ColorMode mode)
	{
		return SDL_SetTextureColorMod(_get(), mode.r, mode.g, mode.b);
	}

	RGBA Texture::getRGBA() const
	{
		return RGBA(getColorMode(), getAlphaMode());
	}

	void Texture::setRGBA(const RGBA& pack)
	{
		setColorMode(pack.toColorMode());
		setAlphaMode(pack.a);
	}

	/// updateInfo() must be called after Texture is changed.
	//protected
	void Texture::updateInfo()
	{
	    if(_get()==nullptr)
        {
            rect.x=rect.y=rect.w=rect.h=0;
        }
		SDL_QueryTexture(_get(), NULL, NULL, &rect.w, &rect.h);
		rect.x = rect.y = 0;
	}

	void Texture::release()
	{
        _clear();
	}

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

	//private
	void Cursor::_set(SDL_Cursor* p)
	{
        _cur.reset(p,SDL_FreeCursor);
	}

	//private
	void Cursor::_set_no_delete(SDL_Cursor* p)
	{
        _cur.reset(p,[](SDL_Cursor* p){});
	}

	//private
	SDL_Cursor* Cursor::_get()
	{
        return _cur.get();
	}

	//private
	void Cursor::_clear()
	{
        _cur.reset();
	}

    Cursor::Cursor(Surface surf,Point hotspot)
    {
        Cursor ns;
        SDL_Cursor* cursor=SDL_CreateColorCursor(surf._get(),hotspot.x,hotspot.y);
        ns._set(cursor);
    }

    Cursor::Cursor(SystemCursorType type)
    {
        Cursor ns;
        ns._set(SDL_CreateSystemCursor(_internal::getSDLSystemCursorFromSystemCursorType(type)));
    }

    //static
    Cursor Cursor::GetActiveCursor()
    {
        Cursor ns;
        ns._set_no_delete(SDL_GetCursor());
        return ns;
    }

    //static
    Cursor Cursor::GetDefaultCursor()
    {
        Cursor ns;
        ns._set_no_delete(SDL_GetDefaultCursor());
        return ns;
    }

    //static
    bool Cursor::isShow()
    {
        return (SDL_ShowCursor(SDL_QUERY)==SDL_ENABLE);
    }

    //static
    void Cursor::setShow(bool Settings)
    {
        SDL_ShowCursor(Settings?SDL_ENABLE:SDL_DISABLE);
    }

    void Cursor::release()
    {
        _clear();
    }

    void Cursor::activate()
    {
        if(_get()!=nullptr)
        {
            SDL_SetCursor(_get());
        }
    }

    WindowMessageBoxButton::WindowMessageBoxButton()
    {
        _hitoption=0;
        text="Button";
        callback=[](){};
    }

    WindowMessageBoxButton::WindowMessageBoxButton(const std::string& ButtonText,const std::function<void()>& CallbackFunc) : text(ButtonText)
    {
        _hitoption=0;
        callback=CallbackFunc;
    }

    void WindowMessageBoxButton::setHitAsEscape(bool enable)
    {
        _hitoption=enable?1:0;
    }

    void WindowMessageBoxButton::setHitAsReturn(bool enable)
    {
        _hitoption=enable?2:0;
    }

    bool WindowMessageBoxButton::isHitAsEscape() const
    {
        return _hitoption==1;
    }

    bool WindowMessageBoxButton::isHitAsReturn() const
    {
        return _hitoption==2;
    }

    WindowMessageBox::WindowMessageBox()
    {
        boxtype=MessageBoxType::Information;
    }

    WindowMessageBox::WindowMessageBox(const std::string& Title,const std::string& Text,MessageBoxType BoxType,const std::function<void()>& DefaultCallback) : title(Title), text(Text)
    {
        boxtype=BoxType;
        defaultcallback=DefaultCallback;
    }

    void WindowMessageBox::addButton(const WindowMessageBoxButton& button)
    {
        _vec.push_back(button);
    }

    int WindowMessageBox::getButtonNum() const
    {
        return _vec.size();
    }

    WindowMessageBoxButton& WindowMessageBox::getButton(int index)
    {
        return _vec.at(index);
    }

    const WindowMessageBoxButton& WindowMessageBox::getButtonConst(int index) const
    {
        return _vec.at(index);
    }

    //private
	void Window::_set(SDL_Window* p)
	{
        _wnd.reset(p,SDL_DestroyWindow);
	}

	//private
	void Window::_clear()
	{
        _wnd.reset();
	}

	//private
	SDL_Window* Window::_get() const
	{
	    return _wnd.get();
	}

	Window::Window(std::string Title, int Width, int Height,
                std::initializer_list<RendererType> RendererFlags,
                std::initializer_list<WindowType> WindowFlags , int WindowPositionX, int WindowPositionY) throw(ErrorViewer)
	{
	    /// Calculate Window Flags
	    Uint32 windowFlag=0;
	    for(auto v:WindowFlags)
        {
            windowFlag|=_internal::getSDLWindowFlagsFromWindowType(v);
        }

		SDL_Window* temp = SDL_CreateWindow(Title.c_str(), WindowPositionX, WindowPositionY, Width, Height, windowFlag);
		if (temp == NULL)
		{
			ErrorViewer e;
			e.fetch();
			throw e;
		}
		_set(temp);
		setRenderer(RendererFlags);
	}

	Renderer Window::getRenderer() const
	{
		return _winrnd;
	}

	void Window::setRenderer(std::initializer_list<RendererType> RendererFlags)
	{
		Uint32 flag = 0;
		for (auto v : RendererFlags)
		{
			flag |= _render_caster(v);
		}
		_setRenderer_Real(flag);
	}

	Rect Window::getSize() const
	{
		int w, h;
		SDL_GetWindowSize(_get(), &w, &h);
		return Rect(0, 0, w, h);
	}

	void Window::setSize(const Rect& sizeRect)
	{
		setSize(sizeRect.w, sizeRect.h);
	}

	void Window::setSize(int w, int h)
	{
		SDL_SetWindowSize(_get(), w, h);
	}

	Point Window::getPosition() const
	{
		int x, y;
		SDL_GetWindowPosition(_get(), &x, &y);
		return Point(x, y);
	}

	void Window::setPosition(int x, int y)
	{
		SDL_SetWindowPosition(_get(), x, y);
	}

	void Window::setPosition(const Point& point)
	{
		SDL_SetWindowPosition(_get(), point.x, point.y);
	}

	void Window::setTitle(const std::string& Title)
	{
		SDL_SetWindowTitle(_get(), Title.c_str());
	}

	std::string Window::getTitle() const
	{
		return std::string(SDL_GetWindowTitle(_get()));
	}

	void Window::setGrab(bool isGrab)
	{
        SDL_SetWindowGrab(_get(),isGrab?SDL_TRUE:SDL_FALSE);
	}

	bool Window::getGrab() const
	{
        return (SDL_GetWindowGrab(_get())==SDL_TRUE)?true:false;
	}

	#if _MINIENGINE_SDL_VERSION_ATLEAST(2,0,5)
	int Window::setOpacity(float opacity)
	{
        return SDL_SetWindowOpacity(_get(),opacity);
	}
    float Window::getOpacity() const
    {
        float op=-1;
        SDL_GetWindowOpacity(_get(),&op);
        return op;
    }
    #endif /// End of SDL2 2.0.5 Require.

    /// FIXME: Not Implemented.
	void Window::setResizable(bool resizable)
	{
		//SDL_SetWindowResizable(_get(), resizable?SDL_TRUE:SDL_FALSE);
	}

	void Window::show()
	{
		SDL_ShowWindow(_get());
	}

	void Window::hide()
	{
		SDL_HideWindow(_get());
	}

	void Window::raise()
	{
		SDL_RaiseWindow(_get());
	}

	void Window::minimize()
	{
		SDL_MinimizeWindow(_get());
	}

	void Window::maximize()
	{
		SDL_MaximizeWindow(_get());
	}

	void Window::restore()
	{
		SDL_RestoreWindow(_get());
	}

	_DECL_DEPRECATED Surface Window::getSurface()
	{
		SDL_Surface* temp = SDL_GetWindowSurface(_get());
		Surface s;
		/// Don't Free This Surface
		s._set_no_delete(temp);
		return s;
	}

	void Window::release()
	{
        _clear();
	}

	// private
	Uint32 Window::_render_caster(RendererType Type)
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
	void Window::_setRenderer_Real(Uint32 flags)
	{
		_winrnd._rnd.reset(SDL_CreateRenderer(_get(), -1, flags), SDL_DestroyRenderer);
	}

	int Window::showSimpleMessageBox(MessageBoxType type,const std::string& Title,const std::string& Message) const
	{
	    Uint32 flags=0;
	    switch(type)
	    {
        case MessageBoxType::Error:
            flags=SDL_MESSAGEBOX_ERROR;
            break;
        case MessageBoxType::Information:
            flags=SDL_MESSAGEBOX_INFORMATION;
            break;
        case MessageBoxType::Warning:
            flags=SDL_MESSAGEBOX_WARNING;
            break;
	    }
        return SDL_ShowSimpleMessageBox(flags,Title.c_str(),Message.c_str(),_get());
	}

	int Window::showMessageBox(const WindowMessageBox& box) const
	{
	    SDL_MessageBoxData mboxdata;
	    mboxdata.title=box.title.c_str();
	    mboxdata.message=box.text.c_str();
	    mboxdata.window=_get();
	    mboxdata.colorScheme=nullptr;
	    mboxdata.numbuttons=box.getButtonNum();
	    SDL_MessageBoxButtonData* pButtonArr=(SDL_MessageBoxButtonData*)malloc(sizeof(SDL_MessageBoxButtonData)*(mboxdata.numbuttons));
	    if(pButtonArr==nullptr)
        {
            /// Failed to malloc
            return -2;
        }
	    for(int i=0;i<mboxdata.numbuttons;i++)
        {
            pButtonArr[i].buttonid=i+1;
            pButtonArr[i].text=box.getButtonConst(i).text.c_str();
            pButtonArr[i].flags=
            (box.getButtonConst(i).isHitAsEscape()) ? SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT
            :(
                (box.getButtonConst(i).isHitAsReturn()) ?SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT
              :0
            );
        }
        mboxdata.buttons=pButtonArr;
        switch(box.boxtype)
	    {
        case MessageBoxType::Error:
            mboxdata.flags=SDL_MESSAGEBOX_ERROR;
            break;
        case MessageBoxType::Information:
            mboxdata.flags=SDL_MESSAGEBOX_INFORMATION;
            break;
        case MessageBoxType::Warning:
            mboxdata.flags=SDL_MESSAGEBOX_WARNING;
            break;
	    }

	    int clickret=-2;

	    /// Call SDL2 to show MessageBox.
        int ret=SDL_ShowMessageBox(&mboxdata,&clickret);

        if(ret==0)
        {
            /// Success.
            if(clickret>=1)
            {
                /// If any button is clicked, call the callback function associated with it.
                if(box.getButtonConst(clickret-1).callback)
                {
                    box.getButtonConst(clickret-1).callback();
                }
            }
            else
            {
                /// ... else, call the default callback
                if(box.defaultcallback) box.defaultcallback();
            }
        }

        /// Free allocated memory
        free(pButtonArr);

        return ret;
	}

    bool Window::isScreenKeyboardShown()
    {
        return SDL_IsScreenKeyboardShown(_get())==SDL_TRUE;
    }

    /// Experimental
    void Window::resetRenderer()
    {
        /// Check if there is a renderer exists.
        if(SDL_GetRenderer(_get())!=nullptr)
        {
            /// Clear internal Renderer class.
            _winrnd._clear();
            /// Check again.
            if(SDL_GetRenderer(_get())!=nullptr)
            {
                /// If it still exists, (May be some other Renderer is holding)
                /// then destroy it.
                SDL_DestroyRenderer(SDL_GetRenderer(_get()));
            }
        }
    }

	void Font::_set(TTF_Font* p)
	{
        _font.reset(p,TTF_CloseFont);
	}

	void Font::_clear()
	{
	    _font.reset();
	}

	TTF_Font* Font::_get() const
	{
        return _font.get();
	}

	Font::Font(std::string FontFileName, size_t size) throw(ErrorViewer)
	{
		if (use(FontFileName, size) != 0)
		{
			ErrorViewer e;
			e.fetch();
			throw e;
		}
	}

	int Font::use(std::string FontFileName, size_t size)
	{
		TTF_Font* temp = TTF_OpenFont(FontFileName.c_str(), size);
		if (temp == NULL) return -1;
		_set(temp);
		return 0;
	}

	bool Font::isReady() const
	{
		return (_get() != nullptr);
	}

    bool Font::isNormal() const
    {
        return !(TTF_GetFontStyle(_get()));
    }

    bool Font::isBold() const
    {
        return (TTF_GetFontStyle(_get()) & TTF_STYLE_BOLD );
    }

    bool Font::isItalic() const
    {
        return (TTF_GetFontStyle(_get()) & TTF_STYLE_ITALIC );
    }

    bool Font::isUnderLine() const
    {
        return (TTF_GetFontStyle(_get()) & TTF_STYLE_UNDERLINE );
    }

    bool Font::isStrikeThrough() const
    {
        return (TTF_GetFontStyle(_get()) & TTF_STYLE_STRIKETHROUGH );
    }

    void Font::setNormal()
    {
        _real_setFontStyle(TTF_STYLE_NORMAL);
    }

    void Font::setBold(bool enable)
    {
        if( enable!=isBold() )
        {
            _real_setFontStyle( TTF_GetFontStyle(_get()) | (enable?TTF_STYLE_BOLD:!TTF_STYLE_BOLD) );
        }
    }

    void Font::setItalic(bool enable)
    {
        if( enable!=isItalic() )
        {
            _real_setFontStyle( TTF_GetFontStyle(_get()) | (enable?TTF_STYLE_ITALIC:!TTF_STYLE_ITALIC) );
        }
    }

    void Font::setUnderLine(bool enable)
    {
        if( enable!=isUnderLine() )
        {
            _real_setFontStyle( TTF_GetFontStyle(_get()) | (enable?TTF_STYLE_UNDERLINE:!TTF_STYLE_UNDERLINE) );
        }
    }

    void Font::setStrikeThrough(bool enable)
    {
        if( enable!=isStrikeThrough() )
        {
            _real_setFontStyle( TTF_GetFontStyle(_get()) | (enable?TTF_STYLE_STRIKETHROUGH:!TTF_STYLE_STRIKETHROUGH) );
        }
    }

    void Font::_real_setFontStyle(int Style)
    {
        TTF_SetFontStyle(_get(),Style);
    }

    int Font::_style_caster(FontStyle style)
    {
        return _internal::getTTFFontStyleFromFontStyle(style);
    }

    std::vector<FontStyle> Font::getFontStyles() const
    {
        int styles=TTF_GetFontStyle(_get());
        return _internal::getFontStyleVecFromMixedTTFFontStyle(styles);
    }

    int Font::getFontHeight() const
    {
        return TTF_FontHeight(_get());
    }

    int Font::getFontAscent() const
    {
        return TTF_FontAscent(_get());
    }

    int Font::getFontDescent() const
    {
        return TTF_FontDescent(_get());
    }

    int Font::getFontLineSkip() const
    {
        return TTF_FontLineSkip(_get());
    }

    bool Font::isFontKerning() const
    {
        return (TTF_GetFontKerning(_get())!=0);
    }

    void Font::setFontKerning(bool enableKerning)
    {
        TTF_SetFontKerning(_get(),enableKerning?1:0);
    }

    long Font::getFontFaceNum() const
    {
        return TTF_FontFaces(_get());
    }

    int Font::getFontFaceIsFixedWidth() const
    {
        return TTF_FontFaceIsFixedWidth(_get());
    }

    std::string Font::getFontFaceFamilyName() const
    {
        return std::string(TTF_FontFaceFamilyName(_get()));
    }

    std::string Font::getFontFaceStyleName() const
    {
        return std::string(TTF_FontFaceStyleName(_get()));
    }

    FontHint Font::getFontHint() const
    {
       switch(TTF_GetFontHinting(_get()))
       {
        case TTF_HINTING_NORMAL:
            return FontHint::Normal;
        case TTF_HINTING_LIGHT:
            return FontHint::Light;
        case TTF_HINTING_MONO:
            return FontHint::Mono;
        case TTF_HINTING_NONE:
            return FontHint::None;
       }
       /// Return Error on default.
       return FontHint::Error;
    }

    void Font::setFontHint(FontHint hint)
    {
        int v=0;
        switch(hint)
        {
        case FontHint::Normal:
            v=TTF_HINTING_NORMAL;
            break;
        case FontHint::Light:
            v=TTF_HINTING_LIGHT;
            break;
        case FontHint::Mono:
            v=TTF_HINTING_MONO;
            break;
        case FontHint::None:
            v=TTF_HINTING_NONE;
            break;
        case FontHint::Error:
            /// No Action on FontHint::Error.
            return;
        }
        TTF_SetFontHinting(_get(),v);
    }

    Rect Font::sizeText(const std::string& Text) const throw (ErrorViewer)
    {
        int w=0,h=0;
        if(TTF_SizeText(_get(),Text.c_str(),&w,&h)!=0)
        {
            /// Something might be wrong
            throw ErrorViewer();
        }
        return Rect(0,0,w,h);
    }

    Rect Font::sizeUTF8(const std::string& Text) const throw (ErrorViewer)
    {
        int w=0,h=0;
        if(TTF_SizeUTF8(_get(),Text.c_str(),&w,&h)!=0)
        {
            /// Something might be wrong
            throw ErrorViewer();
        }
        return Rect(0,0,w,h);
    }

    Rect Font::sizeUnicode(const uint16_t* Text) const throw (ErrorViewer)
    {
        int w=0,h=0;
        if(TTF_SizeUNICODE(_get(),Text,&w,&h)!=0)
        {
            /// Something might be wrong
            throw ErrorViewer();
        }
        return Rect(0,0,w,h);
    }

    /// rendering surfaces...
    Surface Font::renderText(const std::string& Text,const RGBA& fg) const
    {
        Surface surf;
        surf._set(TTF_RenderText_Blended(_get(), Text.c_str(), fg.toSDLColor()));
        return surf;
    }

    Surface Font::renderTextWrapped(const std::string& Text, const RGBA& fg, size_t WrapLength) const
    {
        Surface surf;
		surf._set(TTF_RenderText_Blended_Wrapped(_get(), Text.c_str(), fg.toSDLColor(), WrapLength));
		return surf;
    }

    Surface Font::renderTextShaded(const std::string& Text, const RGBA& fg,const RGBA& bg) const
    {
        Surface surf;
		surf._set(TTF_RenderText_Shaded(_get(), Text.c_str(), fg.toSDLColor(), bg.toSDLColor()));
		return surf;
    }

    Surface Font::renderTextSolid(const std::string& Text,const RGBA& fg) const
    {
        Surface surf;
		surf._set(TTF_RenderText_Solid(_get(), Text.c_str(), fg.toSDLColor()));
		return surf;
    }

    Surface Font::renderUTF8(const std::string& Text,const RGBA& fg) const
    {
        Surface surf;
        surf._set(TTF_RenderUTF8_Blended(_get(), Text.c_str(), fg.toSDLColor()));
        return surf;
    }

    Surface Font::renderUTF8Wrapped(const std::string& Text, const RGBA& fg, size_t WrapLength) const
    {
        Surface surf;
		surf._set(TTF_RenderUTF8_Blended_Wrapped(_get(), Text.c_str(), fg.toSDLColor(), WrapLength));
		return surf;
    }

    Surface Font::renderUTF8Shaded(const std::string& Text, const RGBA& fg,const RGBA& bg) const
    {
        Surface surf;
		surf._set(TTF_RenderUTF8_Shaded(_get(), Text.c_str(), fg.toSDLColor(), bg.toSDLColor()));
		return surf;
    }

    Surface Font::renderUTF8Solid(const std::string& Text,const RGBA& fg) const
    {
        Surface surf;
		surf._set(TTF_RenderUTF8_Solid(_get(), Text.c_str(), fg.toSDLColor()));
		return surf;
    }

    Surface Font::renderUnicode(const uint16_t* Text, const RGBA& fg) const
    {
        Surface surf;
        surf._set(TTF_RenderUNICODE_Blended(_get(),Text,fg.toSDLColor()));
        return surf;
    }

    Surface Font::renderUnicodeWrapped(const uint16_t* Text, const RGBA& fg, size_t WrapLength) const
    {
        Surface surf;
        surf._set(TTF_RenderUNICODE_Blended_Wrapped(_get(),Text,fg.toSDLColor(),WrapLength));
        return surf;
    }

    Surface Font::renderUnicodeShaded(const uint16_t* Text, const RGBA& fg, const RGBA& bg) const
    {
        Surface surf;
        surf._set(TTF_RenderUNICODE_Shaded(_get(),Text,fg.toSDLColor(),bg.toSDLColor()));
        return surf;
    }

    Surface Font::renderUnicodeSolid(const uint16_t* Text, const RGBA& fg) const
    {
        Surface surf;
        surf._set(TTF_RenderUNICODE_Solid(_get(),Text,fg.toSDLColor()));
        return surf;
    }

    /// rendering textures...
	Texture Font::renderText(const Renderer& rnd, const std::string& Text, const RGBA& fg) const
	{
		return rnd.render(renderText(Text,fg));
	}

	Texture Font::renderTextWrapped(const Renderer& rnd, const std::string& Text, const RGBA& fg, size_t WrapLength) const
	{
		return rnd.render(renderTextWrapped(Text,fg,WrapLength));
	}

	Texture Font::renderTextShaded(const Renderer& rnd, const std::string& Text, const RGBA& fg, const RGBA& bg) const
	{
		return rnd.render(renderTextShaded(Text,fg,bg));
	}

	Texture Font::renderTextSolid(const Renderer& rnd, const std::string& Text, const RGBA& fg) const
	{
		return rnd.render(renderTextSolid(Text,fg));
	}

	Texture Font::renderUTF8(const Renderer& rnd, const std::string& Text, const RGBA& fg) const
	{
		return rnd.render(renderUTF8(Text,fg));
	}

	Texture Font::renderUTF8Wrapped(const Renderer& rnd, const std::string& Text, const RGBA& fg, size_t WrapLength) const
	{
		return rnd.render(renderUTF8Wrapped(Text,fg,WrapLength));
	}

	Texture Font::renderUTF8Shaded(const Renderer& rnd, const std::string& Text, const RGBA& fg, const RGBA& bg) const
	{
		return rnd.render(renderUTF8Shaded(Text,fg,bg));
	}

	Texture Font::renderUTF8Solid(const Renderer& rnd, const std::string& Text, const RGBA& fg) const
	{
		return rnd.render(renderUTF8Solid(Text,fg));
	}


    Texture Font::renderUnicode(const Renderer& rnd, const uint16_t* Text, const RGBA& fg) const
    {
        return rnd.render(renderUnicode(Text,fg));
    }

    Texture Font::renderUnicodeWrapped(const Renderer& rnd, const uint16_t* Text, const RGBA& fg, size_t WrapLength) const
    {
        return rnd.render(renderUnicodeWrapped(Text,fg,WrapLength));
    }

    Texture Font::renderUnicodeShaded(const Renderer& rnd, const uint16_t* Text, const RGBA& fg, const RGBA& bg) const
    {
        return rnd.render(renderUnicodeShaded(Text,fg,bg));
    }

    Texture Font::renderUnicodeSolid(const Renderer& rnd, const uint16_t* Text, const RGBA& fg) const
    {
        return rnd.render(renderUnicodeSolid(Text,fg));
    }

	void Font::release()
	{
        _clear();
	}

	void LogSystem::d(const char* fmt,...)
	{
        va_list ap;
	    va_start(ap,fmt);
	    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_DEBUG,fmt,ap);
	    va_end(ap);
	}

	void LogSystem::v(const char* fmt,...)
	{
	    va_list ap;
	    va_start(ap,fmt);
	    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_VERBOSE,fmt,ap);
	    va_end(ap);
	}

	void LogSystem::e(const char* fmt,...)
	{
	    va_list ap;
	    va_start(ap,fmt);
	    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_ERROR,fmt,ap);
	    va_end(ap);
	}

	void LogSystem::i(const char* fmt,...)
	{
	    va_list ap;
	    va_start(ap,fmt);
	    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_INFO,fmt,ap);
	    va_end(ap);
	}

	void LogSystem::w(const char* fmt,...)
	{
	    va_list ap;
	    va_start(ap,fmt);
	    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_WARN,fmt,ap);
	    va_end(ap);
	}

	void LogSystem::critical(const char* fmt,...)
	{
	    va_list ap;
	    va_start(ap,fmt);
	    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_CRITICAL,fmt,ap);
	    va_end(ap);
	}

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

    //static
	int SDLSystem::SDLInit()
	{
		return SDL_Init(SDL_INIT_EVERYTHING);
	}

	//static
	void SDLSystem::SDLQuit()
	{
		SDL_Quit();
	}

	//static
	int SDLSystem::IMGInit()
	{
		return IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	}

	//static
	void SDLSystem::IMGQuit()
	{
		IMG_Quit();
	}

	//static
	int SDLSystem::TTFInit()
	{
		return TTF_Init();
	}

	//static
	void SDLSystem::TTFQuit()
	{
		TTF_Quit();
	}

	//static
	int SDLSystem::MixInit()
	{
		return Mix_Init(MIX_INIT_MP3);
	}

	//static
	void SDLSystem::MixQuit()
	{
		Mix_Quit();
	}

	//static
	void SDLSystem::Init()
	{
		SDLInit();
		IMGInit();
		TTFInit();
		MixInit();
	}

	//static
	void SDLSystem::Quit()
	{
		MixQuit();
		TTFQuit();
		IMGQuit();
		SDLQuit();
	}

	//static
	void SDLSystem::Delay(int ms)
	{
		SDL_Delay(ms);
	}

	//static
	PowerState SDLSystem::GetPowerState()
	{
        SDL_PowerState ret=SDL_GetPowerInfo(NULL,NULL);
        switch(ret)
        {
        case SDL_POWERSTATE_ON_BATTERY:
            return PowerState::OnBattery;
        case SDL_POWERSTATE_NO_BATTERY:
            return PowerState::NoBattery;
        case SDL_POWERSTATE_CHARGING:
            return PowerState::Charging;
        case SDL_POWERSTATE_CHARGED:
            return PowerState::Charged;

        case SDL_POWERSTATE_UNKNOWN:
        default:
            return PowerState::Unknown;
        }
	}

	//static
	int SDLSystem::GetPowerLifeLeft()
	{
        int i;
        SDL_GetPowerInfo(&i,NULL);
        return i;
	}

	//static
	int SDLSystem::GetPowerPrecentageLeft()
	{
        int i;
        SDL_GetPowerInfo(NULL,&i);
        return i;
	}

	//static
	Platform SDLSystem::GetPlatform()
	{
        std::string s(SDL_GetPlatform());
        if(s=="Windows")
        {
            return Platform::Windows;
        }
        else if(s=="Mac OS X")
        {
            return Platform::MacOS;
        }
        else if(s=="Linux")
        {
            return Platform::Linux;
        }
        else if(s=="iOS")
        {
            return Platform::iOS;
        }
        else if(s=="Android")
        {
            return Platform::Android;
        }
        else
        {
            return Platform::Unknown;
        }
	}

	//static
	void SDLSystem::StartTextInput()
	{
		SDL_StartTextInput();
	}

	//static
	bool SDLSystem::IsTextInputActive()
	{
        return SDL_IsTextInputActive()==SDL_TRUE;
	}

	//static
	void SDLSystem::StopTextInput()
	{
		SDL_StopTextInput();
	}

	//static
	bool SDLSystem::HasScreenKeyboardSupport()
	{
        return SDL_HasScreenKeyboardSupport()==SDL_TRUE;
	}

	//static
    std::tuple<int,int,int> SDLSystem::GetSDLCompileVersion()
    {
        SDL_version ver;
        SDL_VERSION(&ver);
        return std::make_tuple(ver.major,ver.minor,ver.patch);
    }

    //static
    std::tuple<int,int,int> SDLSystem::GetSDLLinkedVersion()
    {
        SDL_version ver;
        SDL_GetVersion(&ver);
        return std::make_tuple(ver.major,ver.minor,ver.patch);
    }
    //static
    std::tuple<int,int,int> SDLSystem::GetIMGCompileVersion()
    {
        SDL_version ver;
        SDL_IMAGE_VERSION(&ver);
        return std::make_tuple(ver.major,ver.minor,ver.patch);
    }

    //static
    std::tuple<int,int,int> SDLSystem::GetIMGLinkedVersion()
    {
        const SDL_version* ptr=IMG_Linked_Version();
        return std::make_tuple(ptr->major,ptr->minor,ptr->patch);
    }

    //static
    std::tuple<int,int,int> SDLSystem::GetMixCompileVersion()
    {
        SDL_version ver;
        SDL_MIXER_VERSION(&ver);
        return std::make_tuple(ver.major,ver.minor,ver.patch);
    }

    //static
    std::tuple<int,int,int> SDLSystem::GetMixLinkedVersion()
    {
        const SDL_version* ptr=Mix_Linked_Version();
        return std::make_tuple(ptr->major,ptr->minor,ptr->patch);
    }

    //static
    std::tuple<int,int,int> SDLSystem::GetTTFCompileVersion()
    {
        SDL_version ver;
        SDL_TTF_VERSION(&ver);
        return std::make_tuple(ver.major,ver.minor,ver.patch);
    }

    //static
    std::tuple<int,int,int> SDLSystem::GetTTFLinkedVersion()
    {
        const SDL_version* ptr=TTF_Linked_Version();
        return std::make_tuple(ptr->major,ptr->minor,ptr->patch);
    }

    //static
    int SDLSystem::GetCPUCount()
    {
        return SDL_GetCPUCount();
    }

    //static
    int SDLSystem::GetCPUCacheLineSize()
    {
        return SDL_GetCPUCacheLineSize();
    }

    //static
    int SDLSystem::GetSystemRAM()
    {
        return SDL_GetSystemRAM();
    }

	/// Global Executor For class Timer
	Uint32 _global_timer_executor(Uint32 interval,void* param)
	{
        auto p=reinterpret_cast<std::function<Uint32(Uint32 interval)>*>(param);
        return (*p)(interval);
	}

    Timer::Timer()
    {
        _enabled=false;
        _detached=false;
        _delete_on_disable=false;
        id=-1;
    }

    Timer::Timer(SDL_TimerCallback callback,Uint32 interval,void* param) : Timer()
    {
        _real_timer_call(callback,interval,param);
    }

    void Timer::_real_timer_call(SDL_TimerCallback callback,Uint32 interval,void* param)
    {
        _callback=callback;
        _interval=interval;
        _param=param;
    }

    int Timer::enable()
    {
        if(_enabled)
        {
            return -1;
        }
        else
        {
            id=SDL_AddTimer(_interval,_callback,_param);
            if(id<0) return -2;
            _enabled=true;
            return 0;
        }
    }

    int Timer::disable()
    {
        if(_enabled)
        {
            SDL_RemoveTimer(id);
            _enabled=false;
            id=-1;
            _callback=nullptr;

            if(_delete_on_disable)
            {
                _delete_delegator(reinterpret_cast<std::function<Uint32(Uint32 interval)>*>(_param));
                _delete_on_disable=false;
            }

            _param=nullptr;
            return 0;
        }
        else
        {
            return -1;
        }
    }

    void Timer::detach()
    {
        _detached=true;
    }

    Timer::~Timer()
    {
        if(!_detached)
        {
            disable();
        }
    }

    //static
    void Timer::_delete_delegator(std::function<Uint32(Uint32)>* param)
    {
        delete param;
    }

	AudioPlayer::AudioPlayer()
	{
		if (!_sysAudioCounter)
		{
			_sysAudio = new _Audio;
		}
		++_sysAudioCounter;
	}

	AudioPlayer::~AudioPlayer()
	{
		--_sysAudioCounter;
		if (!_sysAudioCounter)
		{
			delete _sysAudio;
		}
	}

	AudioPlayer::_Audio::_Audio()
	{
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	}

	AudioPlayer::_Audio::~_Audio()
	{
		Mix_CloseAudio();
	}

	void Music::_set(Mix_Music* p)//private
    {
        _music.reset(p,Mix_FreeMusic);
    }

    void Music::_clear()//private
    {
        _music.reset();
    }

    Mix_Music* Music::_get()//private
    {
        return _music.get();
    }

    //static
    int MusicPlayer::GetDecoderNum()
    {
        return Mix_GetNumMusicDecoders();
    }

    //static
    std::string MusicPlayer::GetDecoderName(int index)
    {
        return std::string(Mix_GetMusicDecoder(index));
    }

	Music MusicPlayer::loadMusic(std::string Filename) throw(ErrorViewer)
	{
		Mix_Music* temp = Mix_LoadMUS(Filename.c_str());
		if (temp == nullptr)
		{
			ErrorViewer e;
			e.fetch();
			throw e;
		}
		Music m;
		m._set(temp);
		return m;
	}

	int MusicPlayer::play(Music music, int loops)
	{
		m = music;
		return Mix_PlayMusic(m._get(), loops);
	}

	void MusicPlayer::pause()
	{
		Mix_PauseMusic();
	}

	void MusicPlayer::resume()
	{
		Mix_ResumeMusic();
	}

	void MusicPlayer::rewind()
	{
		Mix_RewindMusic();
	}

	int MusicPlayer::stop()
	{
		return Mix_HaltMusic();
	}

	int MusicPlayer::fadeIn(int loops, int ms)
	{
		return Mix_FadeInMusic(m._get(), loops, ms);
	}

	int MusicPlayer::fadeOut(int ms)
	{
		return Mix_FadeOutMusic(ms);
	}

	bool MusicPlayer::isPlaying()
	{
		return (Mix_PlayingMusic() == 1);
	}

	bool MusicPlayer::isPaused()
	{
		return (Mix_PausedMusic() == 1);
	}

	int MusicPlayer::isFading()
	{
		switch (Mix_FadingMusic())
		{
		case MIX_NO_FADING:
			return 0;
		case MIX_FADING_IN:
			return 1;
		case MIX_FADING_OUT:
			return 2;
		default:
			return -1;
		}
	}

	//static
	int MusicPlayer::SetMusicPosition(double position)
	{
        return Mix_SetMusicPosition(position);
	}

	void Sound::_set(Mix_Chunk* p)
	{
        _sound.reset(p,Mix_FreeChunk);
	}

	void Sound::_clear()//private
	{
        _sound.reset();
	}

	Mix_Chunk* Sound::_get()
	{
        return _sound.get();
	}

    //static
    int SoundPlayer::GetDecoderNum()
    {
        return Mix_GetNumChunkDecoders();
    }

    //static
    std::string SoundPlayer::GetDecoderName(int index)
    {
        return std::string(Mix_GetChunkDecoder(index));
    }

	SoundPlayer::SoundPlayer(int Channels)
	{
		Mix_AllocateChannels(Channels);
	}

	Sound SoundPlayer::loadSound(std::string Filename) throw(ErrorViewer)
	{
		Mix_Chunk* temp = Mix_LoadWAV(Filename.c_str());
		if (temp == NULL)
		{
			ErrorViewer e;
			e.fetch();
			throw e;
		}
		Sound s;
		s._set(temp);
		return s;
	}

	ChannelID SoundPlayer::playSound(Sound sound, int loops) throw(ErrorViewer)
	{
		ChannelID id;
		if (-1 == (id = Mix_PlayChannel(-1, sound._get(), loops)))
		{
			ErrorViewer e;
			e.fetch();
			throw e;
		}
		return id;
	}

	ChannelID SoundPlayer::fadein(Sound sound, int loops, int ms) throw(ErrorViewer)
	{
		ChannelID id;
		if (-1 == (id = Mix_FadeInChannel(-1, sound._get(), loops, ms)))
		{
			ErrorViewer e;
			e.fetch();
			throw e;
		}
		return id;
	}

	int SoundPlayer::fadeout(ChannelID id, int ms)
	{
		return Mix_FadeOutChannel(id, ms);
	}

	void SoundPlayer::pause(ChannelID id)
	{
		Mix_Pause(id);
	}

	void SoundPlayer::resume(ChannelID id)
	{
		Mix_Resume(id);
	}

	int SoundPlayer::stop(ChannelID id)
	{
		return Mix_HaltChannel(id);
	}

    int SoundPlayer::setPanning(ChannelID id, uint8_t left, uint8_t right)
    {
        return Mix_SetPanning(id,left,right);
    }

    int SoundPlayer::setPosition(ChannelID id, int16_t angle, uint8_t distance)
    {
        return Mix_SetPosition(id,angle,distance);
    }

    int SoundPlayer::setDistance(ChannelID id, uint8_t distance)
    {
        return Mix_SetDistance(id,distance);
    }

    int SoundPlayer::setReverseStereo(ChannelID id, int flip)
    {
        return Mix_SetReverseStereo(id,flip);
    }

    int SoundPlayer::addEffect(ChannelID id,Mix_EffectFunc_t f, Mix_EffectDone_t d, void* arg)
    {
        return Mix_RegisterEffect(id,f,d,arg);
    }

    int SoundPlayer::removeEffect(ChannelID id,Mix_EffectFunc_t f)
    {
        return Mix_UnregisterEffect(id,f);
    }

    int SoundPlayer::removeAllEffect(ChannelID id)
    {
        return Mix_UnregisterAllEffects(id);
    }


	AudioPlayer::_Audio* AudioPlayer::_sysAudio = nullptr;
	int AudioPlayer::_sysAudioCounter = 0;

    struct StringEngine::impl
    {
        rapidxml::xml_document<> doc;
        rapidxml::xml_node<>* root;
        bool status;
    };

    StringEngine::StringEngine(std::string StringFile,std::string LanguageTag)
    {
        pimpl=new impl;
        pimpl->status=false;

        std::ifstream ifs(StringFile);
        if(!ifs) return;
        rapidxml::file<> strFile(ifs);
        pimpl->doc.parse<0>(strFile.data());
        pimpl->root=pimpl->doc.first_node(LanguageTag.c_str());
        if(pimpl->root==nullptr) return;

        pimpl->status=true;
    }

    bool StringEngine::ready()
    {
        return pimpl->status;
    }

    int StringEngine::useLanaguage(std::string LanguageTag)
    {
        pimpl->root=pimpl->doc.first_node(LanguageTag.c_str());
        if(pimpl->root==nullptr)
        {
            pimpl->status=false;
            return -1;
        }
        else
        {
            pimpl->status=true;
            return 0;
        }
    }

    std::string StringEngine::getString(std::string Tag)
    {
        if(!ready()) return "(StringEngine::STRING_NOT_FOUND)";
        rapidxml::xml_node<>* pnode=pimpl->root->first_node(Tag.c_str());
        if(pnode==nullptr) return "(StringEngine::STRING_NOT_FOUND)";
        char* context=pnode->value();
        if(context==nullptr) return "";/// Empty String.
        else return std::string(context);
    }

    StringEngine::~StringEngine()
    {
        delete pimpl;
    }

    int SetClipboardText(const std::string& str)
    {
        return SDL_SetClipboardText(str.c_str());
    }

    std::string GetClipboardText()
    {
        char* pstr=SDL_GetClipboardText();
        if(pstr==nullptr)
        {
            return std::string();
        }
        else
        {
            std::string s(pstr);
            SDL_free(pstr);
            return s;
        }
    }

    bool HasClipboardText()
    {
        return SDL_HasClipboardText()==SDL_TRUE;
    }

    bool GetScanKeyState(SDL_Scancode code)
    {
        return SDL_GetKeyboardState(NULL)[code];
    }

}/// End of namespace MiniEngine

/// The Following Functions are not avaliable in Visual Studio
#if (_MINIENGINE_HAS_UNISTD == 1)
bool isexist(std::string Path)
{
    return access(Path.c_str(),F_OK)==0;
}

bool canread(std::string Path)
{
    return access(Path.c_str(),R_OK)==0;
}

bool canwrite(std::string Path)
{
    return access(Path.c_str(),W_OK)==0;
}

bool canexecute(std::string Path)
{
    return access(Path.c_str(),X_OK)==0;
}
#else /// _MINIENGINE_HAS_UNISTD == 0
/// File Functions will be implied in platform specific source file.
#endif

int _miniengine_argc;
char** _miniengine_argv;

 /// Default Setup Code
int main(int argc, char* argv[])
{
    _miniengine_argc=argc;
    _miniengine_argv=argv;
	MiniEngine::SDLSystem::Init();
	int ret = AppMain();
	MiniEngine::SDLSystem::Quit();
	return ret;
}

int GetArgc()
{
    return _miniengine_argc;
}

char** GetArgv()
{
    return _miniengine_argv;
}
