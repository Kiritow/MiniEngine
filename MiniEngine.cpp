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

        int getTTFFontStyleFromFontStyle(Font::Style style)
        {
            switch(style)
            {
            case Font::Style::Bold:
                return TTF_STYLE_BOLD;
            case Font::Style::Italic:
                return TTF_STYLE_ITALIC;
            case Font::Style::Normal:
                return TTF_STYLE_NORMAL;
            case Font::Style::StrikeThrough:
                return TTF_STYLE_STRIKETHROUGH;
            case Font::Style::UnderLine:
                return TTF_STYLE_UNDERLINE;
            default:
                return TTF_STYLE_NORMAL;
            }
        }

        std::vector<Font::Style> getFontStyleVecFromMixedTTFFontStyle(int Mixed_TTF_Font_Style)
        {
            std::vector<Font::Style> vec;
            if(Mixed_TTF_Font_Style&TTF_STYLE_BOLD)
            {
                vec.push_back(Font::Style::Bold);
            }
            if(Mixed_TTF_Font_Style&TTF_STYLE_ITALIC)
            {
                vec.push_back(Font::Style::Italic);
            }
            if(Mixed_TTF_Font_Style&TTF_STYLE_STRIKETHROUGH)
            {
                vec.push_back(Font::Style::StrikeThrough);
            }
            if(Mixed_TTF_Font_Style&TTF_STYLE_UNDERLINE)
            {
                vec.push_back(Font::Style::UnderLine);
            }
            if(vec.empty())
            {
                vec.push_back(Font::Style::Normal);
            }

            return vec;
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

	bool Rect::hasIntersection(const Rect& r)
	{
        SDL_Rect a=toSDLRect(),b=r.toSDLRect();
        return SDL_HasIntersection(&a,&b)==SDL_TRUE;
	}

	Rect Rect::getIntersection(const Rect& r)
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

	Rect Rect::getUnion(const Rect& r)
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

	SDL_Point Point::toSDLPoint()
	{
		SDL_Point p;
		p.x = x;
		p.y = y;
		return p;
	}

	bool Point::inRect(Rect rect)
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

	SDL_Color RGBA::toSDLColor()
	{
		SDL_Color c;
		c.r = r;
		c.g = g;
		c.b = b;
		c.a = a;
		return c;
	}

	ColorMode RGBA::toColorMode()
	{
		return ColorMode(r, g, b);
	}

	void ErrorViewer::fetch()
	{
		str = SDL_GetError();
	}

	std::string ErrorViewer::getError()
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
    SDL_RWops* RWOP::_get()
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

    void Surface::_set(SDL_Surface* p)//private
    {
        _surf.reset(p,SDL_FreeSurface);
    }

    void Surface::_clear()//private
    {
        _surf.reset();
    }

    SDL_Surface* Surface::_get()//private
    {
        return _surf.get();
    }

    std::shared_ptr<SDL_Surface>& Surface::_getex()
    {
        return _surf;
    }

    int Surface::getw()
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

    int Surface::geth()
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

    BlendMode Surface::getBlendMode()
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

    int Surface::savePNG(const std::string& filename)
    {
        return IMG_SavePNG(_get(),filename.c_str());
    }

    int Surface::blit(Surface s,Rect src,Rect dst)
    {
        SDL_Rect rsrc=src.toSDLRect();
        SDL_Rect rdst=dst.toSDLRect();
        return SDL_BlitSurface(s._get(),&rsrc,_get(),&rdst);
    }

    int Surface::blitTo(Surface s,Rect dst)
    {
        SDL_Rect rdst=dst.toSDLRect();
        return SDL_BlitSurface(s._get(),NULL,_get(),&rdst);
    }

    int Surface::blitTo(Surface s,Point lupoint)
    {
        return blitTo(s,Rect(lupoint.x,lupoint.y,s.getw(),s.geth()));
    }

    int Surface::blitFill(Surface s,Rect src)
    {
        SDL_Rect rsrc=src.toSDLRect();
        return SDL_BlitSurface(s._get(),&rsrc,_get(),NULL);
    }

    int Surface::blitFullFill(Surface s)
    {
        return SDL_BlitSurface(s._get(),NULL,_get(),NULL);
    }

    int Surface::blitScaled(Surface s,Rect src,Rect dst)
    {
        SDL_Rect rsrc=src.toSDLRect();
        SDL_Rect rdst=dst.toSDLRect();
        return SDL_BlitScaled(s._get(),&rsrc,_get(),&rdst);
    }

    int Surface::blitScaledTo(Surface s,Rect dst)
    {
        SDL_Rect rdst=dst.toSDLRect();
        return SDL_BlitScaled(s._get(),NULL,_get(),&rdst);
    }

    int Surface::blitScaledTo(Surface s,Point lupoint)
    {
        return blitScaledTo(s,Rect(lupoint.x,lupoint.y,s.getw(),s.geth()));
    }

    int Surface::blitScaledFill(Surface s,Rect src)
    {
        SDL_Rect rsrc=src.toSDLRect();
        return SDL_BlitScaled(s._get(),&rsrc,_get(),NULL);
    }

    int Surface::blitScaledFullFill(Surface s)
    {
        return SDL_BlitScaled(s._get(),NULL,_get(),NULL);
    }

    int Surface::setAlphaMode(int alpha)
    {
        return SDL_SetSurfaceAlphaMod(_get(),alpha);
    }

    int Surface::getAlphaMode()
    {
        Uint8 al;
        SDL_GetSurfaceAlphaMod(_get(),&al);
        return al;
    }

    int Surface::setColorMode(ColorMode mode)
    {
        return SDL_SetSurfaceColorMod(_get(),mode.r,mode.g,mode.b);
    }

    ColorMode Surface::getColorMode()
    {
        Uint8 r,g,b;
        SDL_GetSurfaceColorMod(_get(),&r,&g,&b);
        ColorMode mode;
        mode.r=r;
        mode.g=g;
        mode.b=b;
        return mode;
    }

    void Surface::setRGBA(RGBA pack)
    {
        setColorMode(pack.toColorMode());
        setAlphaMode(pack.a);
    }

    RGBA Surface::getRGBA()
    {
        return RGBA(getColorMode(),getAlphaMode());
    }

    bool Surface::mustlock()
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

    //static
    Surface Surface::createSurface(int width,int height,int depth,int Rmask,int Gmask,int Bmask,int Amask) throw(ErrorViewer)
    {
        SDL_Surface* temp=SDL_CreateRGBSurface(0,width,height,depth,Rmask,Gmask,Bmask,Amask);
        if(temp==nullptr)
        {
            ErrorViewer e;
            e.fetch();
            throw e;
        }
        Surface surf;
        surf._set(temp);
        return surf;
    }

    void Texture::_set(SDL_Texture* p)//private
    {
        _text.reset(p,SDL_DestroyTexture);
        updateInfo();
    }

    void Texture::_clear()//private
    {
        _text.reset();
        updateInfo();
    }

    SDL_Texture* Texture::_get()//private
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

	int Texture::getw()
	{
		return rect.w;
	}

	int Texture::geth()
	{
		return rect.h;
	}

	bool Texture::isReady()
	{
		return (_get() != nullptr);
	}

	int Texture::setBlendMode(BlendMode mode)
	{
		return SDL_SetTextureBlendMode(_get(), _internal::getSDLBlendModeFromBlendMode(mode));
	}

	BlendMode Texture::getBlendMode()
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

	int Texture::getAlphaMode()
	{
		Uint8 temp;
		SDL_GetTextureAlphaMod(_get(), &temp);
		return temp;
	}

	ColorMode Texture::getColorMode()
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

	RGBA Texture::getRGBA()
	{
		return RGBA(getColorMode(), getAlphaMode());
	}

	void Texture::setRGBA(RGBA pack)
	{
		setColorMode(pack.toColorMode());
		setAlphaMode(pack.a);
	}

	/// updateInfo() must be called after Texture is changed.

	void Texture::updateInfo()//protected
	{
	    if(_get()==nullptr)
        {
            rect.x=rect.y=rect.w=rect.h=0;
        }
		SDL_QueryTexture(_get(), NULL, NULL, &rect.w, &rect.h);
		rect.x = rect.y = 0;
	}

	void Renderer::_set(SDL_Renderer* p)
	{
        _rnd.reset(p,SDL_DestroyRenderer);
	}

	void Renderer::_clear()
	{
        _rnd.reset();
	}

	SDL_Renderer* Renderer::_get()
	{
        return _rnd.get();
	}

	int Renderer::setColor(RGBA pack)
	{
		return SDL_SetRenderDrawColor(_get(), pack.r, pack.g, pack.b, pack.a);
	}

	RGBA Renderer::getColor()
	{
		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(_get(), &r, &g, &b, &a);
		RGBA pack(r, g, b, a);
		return pack;
	}

	int Renderer::setBlendMode(BlendMode mode)
	{
		return SDL_SetRenderDrawBlendMode(_get(), _internal::getSDLBlendModeFromBlendMode(mode));
	}

	BlendMode Renderer::getBlendMode()
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

	int Renderer::fillRect(Rect rect)
	{
		auto inr = rect.toSDLRect();
		return SDL_RenderFillRect(_get(), &inr);
	}

	int Renderer::drawRect(Rect rect)
	{
		auto inr = rect.toSDLRect();
		return SDL_RenderDrawRect(_get(), &inr);
	}

	int Renderer::drawPoint(Point p)
	{
        return SDL_RenderDrawPoint(_get(),p.x,p.y);
	}

	int Renderer::clear()
	{
		return SDL_RenderClear(_get());
	}

	void Renderer::update()
	{
		SDL_RenderPresent(_get());
	}

	int Renderer::copy(Texture t, Rect src, Rect dst)
	{
		SDL_Rect s = src.toSDLRect();
		SDL_Rect d = dst.toSDLRect();
		return SDL_RenderCopy(_get(), t._get(), &s, &d);
	}

	int Renderer::copyTo(Texture t, Rect dst)
	{
		SDL_Rect d = dst.toSDLRect();
		return SDL_RenderCopy(_get(), t._get(), NULL, &d);
	}

	int Renderer::copyTo(Texture t, Point lupoint)
	{
		return copyTo(t, Rect(lupoint.x, lupoint.y, t.getw(), t.geth()));
	}

	int Renderer::copyFill(Texture t, Rect src)
	{
		SDL_Rect s = src.toSDLRect();
		return SDL_RenderCopy(_get(), t._get(), &s, NULL);
	}

	int Renderer::copyFullFill(Texture t)
	{
		return SDL_RenderCopy(_get(), t._get(), NULL, NULL);
	}

	int Renderer::supercopy(Texture t,bool srcfull,Rect src,bool dstfull,Rect dst,double angle,bool haspoint,Point center,FlipMode mode)
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

        switch(mode)
        {
        case FlipMode::None:
            flip=SDL_FLIP_NONE;
            break;
        case FlipMode::Horizontal:
            flip=SDL_FLIP_HORIZONTAL;
            break;
        case FlipMode::Vertical:
            flip=SDL_FLIP_VERTICAL;
            break;
        default:
            flip=SDL_FLIP_NONE;
        }

        return SDL_RenderCopyEx(_get(),t._get(),pR1,pR2,angle,pPoint,flip);
	}

	Surface Renderer::loadSurface(std::string FileName) throw(ErrorViewer)
	{
		Surface surf;
		SDL_Surface* temp = IMG_Load(FileName.c_str());
		if (temp == nullptr)
		{
			ErrorViewer e;
			e.fetch();
			throw e;
		}
		surf._set(temp);
		return surf;
	}

	Surface Renderer::loadSurfaceRW(RWOP rwop) throw (ErrorViewer)
	{
        Surface surf;
        SDL_Surface* temp=IMG_Load_RW(rwop._get(),0);
        if(temp==nullptr)
        {
            ErrorViewer e;
            e.fetch();
            throw e;
        }
        surf._set(temp);
        return surf;
	}

	Texture Renderer::render(Surface surf) throw(ErrorViewer)
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

	Texture Renderer::loadTexture(std::string FileName) throw(ErrorViewer)
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

	Texture Renderer::loadTextureRW(RWOP rwop) throw (ErrorViewer)
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

	Texture Renderer::createTexture(int Width, int Height) throw(ErrorViewer)
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

	//static
    Cursor Cursor::CreateCursor(Surface surf,Point hotspot)
    {
        Cursor ns;
        SDL_Cursor* cursor=SDL_CreateColorCursor(surf._get(),hotspot.x,hotspot.y);
        ns._set(cursor);
        return ns;
    }

    //static
    Cursor Cursor::CreateSystemCursor(SystemCursorType type)
    {
        Cursor ns;
        ns._set(SDL_CreateSystemCursor(_internal::getSDLSystemCursorFromSystemCursorType(type)));
        return ns;
    }

    //static
    Cursor Cursor::GetActiveCursor()
    {
        Cursor ns;
        ns._set(SDL_GetCursor());
        return ns;
    }

    //static
    Cursor Cursor::GetDefaultCursor()
    {
        Cursor ns;
        ns._set(SDL_GetDefaultCursor());
        return ns;
    }

    //static
    bool Cursor::isShow()
    {
        return (SDL_ShowCursor(SDL_QUERY)==SDL_ENABLE);
    }

    //static
    void Cursor::show(bool Settings)
    {
        SDL_ShowCursor(Settings?SDL_ENABLE:SDL_DISABLE);
    }

    void Cursor::activate()
    {
        SDL_SetCursor(_get());
    }

	bool Renderer::isReady()
	{
		return (_get() != nullptr);
	}

	void Window::_set(SDL_Window* p)
	{
        _wnd.reset(p,SDL_DestroyWindow);
	}

	void Window::_clear()
	{
        _wnd.reset();
	}

	SDL_Window* Window::_get()
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
		return winrnd;
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

	Rect Window::getSize()
	{
		int w, h;
		SDL_GetWindowSize(_get(), &w, &h);
		return Rect(0, 0, w, h);
	}

	void Window::setSize(Rect sizeRect)
	{
		setSize(sizeRect.w, sizeRect.h);
	}

	void Window::setSize(int w, int h)
	{
		SDL_SetWindowSize(_get(), w, h);
	}

	Rect Window::getPosition()
	{
		int x, y;
		SDL_GetWindowPosition(_get(), &x, &y);
		return Rect(x, y, 0, 0);
	}

	void Window::setPosition(int x, int y)
	{
		SDL_SetWindowPosition(_get(), x, y);
	}

	/// FIXME: Use class Point or class Rect ?

	void Window::setPosition(Point point)
	{
		SDL_SetWindowPosition(_get(), point.x, point.y);
	}

	void Window::setTitle(std::string Title)
	{
		SDL_SetWindowTitle(_get(), Title.c_str());
	}

	std::string Window::getTitle()
	{
		return std::string(SDL_GetWindowTitle(_get()));
	}

	void Window::setGrab(bool isGrab)
	{
        SDL_SetWindowGrab(_get(),isGrab?SDL_TRUE:SDL_FALSE);
	}

	bool Window::getGrab()
	{
        return (SDL_GetWindowGrab(_get())==SDL_TRUE)?true:false;
	}

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
		s._getex().reset(temp, [](SDL_Surface*) {});
		return s;
	}

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

	void Window::_setRenderer_Real(Uint32 flags)
	{
		winrnd._rnd.reset(SDL_CreateRenderer(_get(), -1, flags), SDL_DestroyRenderer);
	}

	int Window::showSimpleMessageBox(MessageBoxType type,std::string Title,std::string Message)
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

	void Font::_set(TTF_Font* p)
	{
        _font.reset(p,TTF_CloseFont);
	}

	void Font::_clear()
	{
	    _font.reset();
	}

	TTF_Font* Font::_get()
	{
        return _font.get();
	}

	Font::Font(std::string FontFileName, int size) throw(ErrorViewer)
	{
		if (use(FontFileName, size) != 0)
		{
			ErrorViewer e;
			e.fetch();
			throw e;
		}
	}

	int Font::use(std::string FontFileName, int size)
	{
		TTF_Font* temp = TTF_OpenFont(FontFileName.c_str(), size);
		if (temp == NULL) return -1;
		_set(temp);
		return 0;
	}

	bool Font::isReady()
	{
		return (_get() != nullptr);
	}

    bool Font::isNormal()
    {
        return !(TTF_GetFontStyle(_get()));
    }

    bool Font::isBold()
    {
        return (TTF_GetFontStyle(_get()) & TTF_STYLE_BOLD );
    }

    bool Font::isItalic()
    {
        return (TTF_GetFontStyle(_get()) & TTF_STYLE_ITALIC );
    }

    bool Font::isUnderLine()
    {
        return (TTF_GetFontStyle(_get()) & TTF_STYLE_UNDERLINE );
    }

    bool Font::isStrikeThrough()
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

    int Font::_style_caster(Style style)
    {
        return _internal::getTTFFontStyleFromFontStyle(style);
    }

    std::vector<Font::Style> Font::getFontStyles()
    {
        int styles=TTF_GetFontStyle(_get());
        return _internal::getFontStyleVecFromMixedTTFFontStyle(styles);
    }

    Rect Font::sizeText(const std::string& Text) throw (ErrorViewer)
    {
        int w=0,h=0;
        if(TTF_SizeText(_get(),Text.c_str(),&w,&h)!=0)
        {
            /// Something might be wrong
            throw ErrorViewer();
        }
        return Rect(0,0,w,h);
    }

    Rect Font::sizeUTF8(const std::string& Text) throw (ErrorViewer)
    {
        int w=0,h=0;
        if(TTF_SizeUTF8(_get(),Text.c_str(),&w,&h)!=0)
        {
            /// Something might be wrong
            throw ErrorViewer();
        }
        return Rect(0,0,w,h);
    }

    /// rendering surfaces...
    Surface Font::renderText(std::string Text,RGBA fg)
    {
        Surface surf;
        surf._set(TTF_RenderText_Blended(_get(), Text.c_str(), fg.toSDLColor()));
        return surf;
    }

    Surface Font::renderTextWrapped(std::string Text, RGBA fg, int WrapLength)
    {
        Surface surf;
		surf._set(TTF_RenderText_Blended_Wrapped(_get(), Text.c_str(), fg.toSDLColor(), WrapLength));
		return surf;
    }

    Surface Font::renderTextShaded(std::string Text, RGBA fg,RGBA bg)
    {
        Surface surf;
		surf._set(TTF_RenderText_Shaded(_get(), Text.c_str(), fg.toSDLColor(), bg.toSDLColor()));
		return surf;
    }

    Surface Font::renderTextSolid(std::string Text,RGBA fg)
    {
        Surface surf;
		surf._set(TTF_RenderText_Solid(_get(), Text.c_str(), fg.toSDLColor()));
		return surf;
    }

    Surface Font::renderUTF8(std::string Text,RGBA fg)
    {
        Surface surf;
        surf._set(TTF_RenderUTF8_Blended(_get(), Text.c_str(), fg.toSDLColor()));
        return surf;
    }

    Surface Font::renderUTF8Wrapped(std::string Text, RGBA fg, int WrapLength)
    {
        Surface surf;
		surf._set(TTF_RenderUTF8_Blended_Wrapped(_get(), Text.c_str(), fg.toSDLColor(), WrapLength));
		return surf;
    }

    Surface Font::renderUTF8Shaded(std::string Text, RGBA fg,RGBA bg)
    {
        Surface surf;
		surf._set(TTF_RenderUTF8_Shaded(_get(), Text.c_str(), fg.toSDLColor(), bg.toSDLColor()));
		return surf;
    }

    Surface Font::renderUTF8Solid(std::string Text,RGBA fg)
    {
        Surface surf;
		surf._set(TTF_RenderUTF8_Solid(_get(), Text.c_str(), fg.toSDLColor()));
		return surf;
    }

    /// rendering textures...
	Texture Font::renderText(Renderer rnd, std::string Text, RGBA fg)
	{
		return rnd.render(renderText(Text,fg));
	}

	Texture Font::renderTextWrapped(Renderer rnd, std::string Text, RGBA fg, int WrapLength)
	{
		return rnd.render(renderTextWrapped(Text,fg,WrapLength));
	}

	Texture Font::renderTextShaded(Renderer rnd, std::string Text, RGBA fg, RGBA bg)
	{
		return rnd.render(renderTextShaded(Text,fg,bg));
	}

	Texture Font::renderTextSolid(Renderer rnd, std::string Text, RGBA fg)
	{
		return rnd.render(renderTextSolid(Text,fg));
	}

	Texture Font::renderUTF8(Renderer rnd, std::string Text, RGBA fg)
	{
		return rnd.render(renderUTF8(Text,fg));
	}

	Texture Font::renderUTF8Wrapped(Renderer rnd, std::string Text, RGBA fg, int WrapLength)
	{
		return rnd.render(renderUTF8Wrapped(Text,fg,WrapLength));
	}

	Texture Font::renderUTF8Shaded(Renderer rnd, std::string Text, RGBA fg, RGBA bg)
	{
		return rnd.render(renderUTF8Shaded(Text,fg,bg));
	}

	Texture Font::renderUTF8Solid(Renderer rnd, std::string Text, RGBA fg)
	{
		return rnd.render(renderUTF8Solid(Text,fg));
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

    SharedLibrary::SharedLibrary()
    {
        _obj=nullptr;
    }

    SharedLibrary::SharedLibrary(const std::string& Filename)
    {
        _obj=nullptr;
        load(Filename);
    }

    SharedLibrary::~SharedLibrary()
    {
        if(_obj)
        {
            unload();
        }
    }

    int SharedLibrary::load(const std::string& Filename)
    {
        if(_obj) return -1;
        else
        {
            _obj=SDL_LoadObject(Filename.c_str());
            if(_obj) return 0;
            else return -2;
        }
    }

    int SharedLibrary::unload()
    {
        if(_obj)
        {
            SDL_UnloadObject(_obj);
            _obj=nullptr;
            return 0;
        }
        else return -1;
    }

    void* SharedLibrary::get(const std::string& FunctionName)
    {
        if(!_obj) return nullptr;
        else return SDL_LoadFunction(_obj,FunctionName.c_str());
    }


	int SDLSystem::SDLInit()
	{
		return SDL_Init(SDL_INIT_EVERYTHING);
	}

	void SDLSystem::SDLQuit()
	{
		SDL_Quit();
	}

	int SDLSystem::IMGInit()
	{
		return IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	}

	void SDLSystem::IMGQuit()
	{
		IMG_Quit();
	}

	int SDLSystem::TTFInit()
	{
		return TTF_Init();
	}

	void SDLSystem::TTFQuit()
	{
		TTF_Quit();
	}

	int SDLSystem::MixInit()
	{
		return Mix_Init(MIX_INIT_MP3);
	}

	void SDLSystem::MixQuit()
	{
		Mix_Quit();
	}

	void SDLSystem::Init()
	{
		SDLInit();
		IMGInit();
		TTFInit();
		MixInit();
	}

	void SDLSystem::Quit()
	{
		MixQuit();
		TTFQuit();
		IMGQuit();
		SDLQuit();
	}

	void SDLSystem::Delay(int ms)
	{
		SDL_Delay(ms);
	}

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

	int SDLSystem::GetPowerLifeLeft()
	{
        int i;
        SDL_GetPowerInfo(&i,NULL);
        return i;
	}

	int SDLSystem::GetPowerPrecentageLeft()
	{
        int i;
        SDL_GetPowerInfo(NULL,&i);
        return i;
	}

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

	void SDLSystem::StartTextInput()
	{
		SDL_StartTextInput();
	}

	void SDLSystem::StopTextInput()
	{
		SDL_StopTextInput();
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
        if(!ready()) return "";
        char* context=pimpl->root->first_node(Tag.c_str())->value();
        if(context==nullptr) return "";
        else return std::string(context);
    }

    StringEngine::~StringEngine()
    {
        delete pimpl;
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
