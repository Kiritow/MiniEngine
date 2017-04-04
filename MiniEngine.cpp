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
	Rect::Rect(int X, int Y, int W, int H)
	{
		x = X;
		y = Y;
		w = W;
		h = H;
	}

	Rect::Rect()
	{
		x = y = w = h = 0;
	}

	SDL_Rect Rect::toSDLRect()
	{
		SDL_Rect r;
		r.x = x;
		r.y = y;
		r.w = w;
		r.h = h;
		return r;
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
		return SDL_PointInRect(&p, &r);
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
		return (text.get() != nullptr);
	}

	int Texture::setBlendMode(BlendMode mode)
	{
		return SDL_SetTextureBlendMode(text.get(), static_cast<SDL_BlendMode>(mode));
	}

	BlendMode Texture::getBlendMode()
	{
		SDL_BlendMode temp;
		SDL_GetTextureBlendMode(text.get(), &temp);
		return static_cast<BlendMode>(temp);
	}

	/// Alpha:  0: Transparent 255: opaque

	int Texture::setAlphaMode(int alpha)
	{
		Uint8 temp = std::max(std::min(alpha, 255), 0);
		return SDL_SetTextureAlphaMod(text.get(), temp);
	}

	int Texture::getAlphaMode()
	{
		Uint8 temp;
		SDL_GetTextureAlphaMod(text.get(), &temp);
		return temp;
	}

	ColorMode Texture::getColorMode()
	{
		ColorMode pack;
		Uint8 r, g, b;
		SDL_GetTextureColorMod(text.get(), &r, &g, &b);
		pack.r = r;
		pack.g = g;
		pack.b = b;
		return pack;
	}

	int Texture::setColorMode(ColorMode mode)
	{
		return SDL_SetTextureColorMod(text.get(), mode.r, mode.g, mode.b);
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

	void Texture::updateInfo()
	{
	    if(text.get()==nullptr)
        {
            rect.x=rect.y=rect.w=rect.h=0;
        }
		SDL_QueryTexture(text.get(), NULL, NULL, &rect.w, &rect.h);
		rect.x = rect.y = 0;
	}

	int Renderer::setColor(RGBA pack)
	{
		return SDL_SetRenderDrawColor(rnd.get(), pack.r, pack.g, pack.b, pack.a);
	}

	RGBA Renderer::getColor()
	{
		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(rnd.get(), &r, &g, &b, &a);
		RGBA pack(r, g, b, a);
		return pack;
	}

	int Renderer::setBlendMode(BlendMode mode)
	{
		return SDL_SetRenderDrawBlendMode(rnd.get(), static_cast<SDL_BlendMode>(mode));
	}

	BlendMode Renderer::getBlendMode()
	{
		SDL_BlendMode temp;
		SDL_GetRenderDrawBlendMode(rnd.get(), &temp);
		return static_cast<BlendMode>(temp);
	}

	int Renderer::setTarget(Texture & t)
	{
		return SDL_SetRenderTarget(rnd.get(), t.text.get());
	}

	int Renderer::setTarget()
	{
		return SDL_SetRenderTarget(rnd.get(), nullptr);
	}

	int Renderer::fillRect(Rect rect)
	{
		auto inr = rect.toSDLRect();
		return SDL_RenderFillRect(rnd.get(), &inr);
	}

	int Renderer::drawRect(Rect rect)
	{
		auto inr = rect.toSDLRect();
		return SDL_RenderDrawRect(rnd.get(), &inr);
	}

	int Renderer::drawPoint(Point p)
	{
        return SDL_RenderDrawPoint(rnd.get(),p.x,p.y);
	}

	int Renderer::clear()
	{
		return SDL_RenderClear(rnd.get());
	}

	void Renderer::update()
	{
		SDL_RenderPresent(rnd.get());
	}

	int Renderer::copy(Texture t, Rect src, Rect dst)
	{
		SDL_Rect s = src.toSDLRect();
		SDL_Rect d = dst.toSDLRect();
		return SDL_RenderCopy(rnd.get(), t.text.get(), &s, &d);
	}

	int Renderer::copyTo(Texture t, Rect dst)
	{
		SDL_Rect d = dst.toSDLRect();
		return SDL_RenderCopy(rnd.get(), t.text.get(), NULL, &d);
	}

	int Renderer::copyTo(Texture t, Point lupoint)
	{
		return copyTo(t, Rect(lupoint.x, lupoint.y, t.getw(), t.geth()));
	}

	int Renderer::copyFill(Texture t, Rect src)
	{
		SDL_Rect s = src.toSDLRect();
		return SDL_RenderCopy(rnd.get(), t.text.get(), &s, NULL);
	}

	int Renderer::copyFullFill(Texture t)
	{
		return SDL_RenderCopy(rnd.get(), t.text.get(), NULL, NULL);
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
        }

        return SDL_RenderCopyEx(rnd.get(),t.text.get(),pR1,pR2,angle,pPoint,flip);
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
		surf.surf.reset(temp, SDL_FreeSurface);
		return surf;
	}

	Texture Renderer::render(Surface surf) throw(ErrorViewer)
	{
		Texture t;
		SDL_Texture* temp = SDL_CreateTextureFromSurface(rnd.get(), surf.surf.get());
		if (temp == nullptr)
		{
			ErrorViewer e;
			e.fetch();
			throw e;
		}
		t.text.reset(temp, SDL_DestroyTexture);
		t.updateInfo();
		return t;
	}

	Texture Renderer::loadTexture(std::string FileName) throw(ErrorViewer)
	{
		Texture t;
		SDL_Texture* temp = IMG_LoadTexture(rnd.get(), FileName.c_str());
		if (temp == nullptr)
		{
			ErrorViewer e;
			e.fetch();
			throw e;
		}
		t.text.reset(temp, SDL_DestroyTexture);
		t.updateInfo();
		return t;
	}

	Texture Renderer::createTexture(int Width, int Height) throw(ErrorViewer)
	{
		SDL_Texture* temp = SDL_CreateTexture(rnd.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Width, Height);
		if (temp == NULL)
		{
			ErrorViewer e;
			e.fetch();
			throw e;
		}
		Texture t;
		t.text.reset(temp, SDL_DestroyTexture);
		t.updateInfo();
		return t;
	}

	bool Renderer::isReady()
	{
		return (rnd.get() != nullptr);
	}

	Window::Window(std::string Title, int Width, int Height, std::initializer_list<RendererType> RendererFlags) throw(ErrorViewer)
	{
		SDL_Window* temp = SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_SHOWN);
		if (temp == NULL)
		{
			ErrorViewer e;
			e.fetch();
			throw e;
		}
		wnd.reset(temp, SDL_DestroyWindow);
		setRenderer(RendererFlags);
	}

	Renderer Window::getRenderer() const
	{
		return winrnd;
	}

	void Window::setRenderer(std::initializer_list<RendererType> RendererFlags)
	{
		int flag = 0;
		for (auto v : RendererFlags)
		{
			flag |= static_cast<int>(v);
		}
		_setRenderer_Real(flag);
	}

	Rect Window::getSize()
	{
		int w, h;
		SDL_GetWindowSize(wnd.get(), &w, &h);
		return Rect(0, 0, w, h);
	}

	void Window::setSize(Rect sizeRect)
	{
		setSize(sizeRect.w, sizeRect.h);
	}

	void Window::setSize(int w, int h)
	{
		SDL_SetWindowSize(wnd.get(), w, h);
	}

	Rect Window::getPosition()
	{
		int x, y;
		SDL_GetWindowPosition(wnd.get(), &x, &y);
		return Rect(x, y, 0, 0);
	}

	void Window::setPosition(int x, int y)
	{
		SDL_SetWindowPosition(wnd.get(), x, y);
	}

	/// FIXME: Use class Point or class Rect ?

	void Window::setPosition(Point point)
	{
		SDL_SetWindowPosition(wnd.get(), point.x, point.y);
	}

	void Window::setTitle(std::string Title)
	{
		SDL_SetWindowTitle(wnd.get(), Title.c_str());
	}

	std::string Window::getTitle()
	{
		return std::string(SDL_GetWindowTitle(wnd.get()));
	}

	void Window::setResizable(bool resizable)
	{
		//SDL_SetWindowResizable(wnd.get(), static_cast<SDL_bool>(resizable));
	}

	void Window::show()
	{
		SDL_ShowWindow(wnd.get());
	}

	void Window::hide()
	{
		SDL_HideWindow(wnd.get());
	}

	void Window::raise()
	{
		SDL_RaiseWindow(wnd.get());
	}

	void Window::minimize()
	{
		SDL_MinimizeWindow(wnd.get());
	}

	void Window::maximize()
	{
		SDL_MaximizeWindow(wnd.get());
	}

	void Window::restore()
	{
		SDL_RestoreWindow(wnd.get());
	}

	_DECL_DEPRECATED Surface Window::getSurface()
	{
		SDL_Surface* temp = SDL_GetWindowSurface(wnd.get());
		Surface s;
		/// Don't Free This Surface
		s.surf.reset(temp, [](SDL_Surface*) {});
		return s;
	}

	void Window::_setRenderer_Real(Uint32 flags)
	{
		winrnd.rnd.reset(SDL_CreateRenderer(wnd.get(), -1, flags), SDL_DestroyRenderer);
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
        return SDL_ShowSimpleMessageBox(flags,Title.c_str(),Message.c_str(),wnd.get());
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
		font.reset(temp, TTF_CloseFont);
		return 0;
	}

	bool Font::isReady()
	{
		return (font.get() != nullptr);
	}

	Texture Font::renderText(Renderer rnd, std::string Text, RGBA fg)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderText_Blended(font.get(), Text.c_str(), fg.toSDLColor()), SDL_FreeSurface);
		return rnd.render(surf);
	}

	Texture Font::renderTextWrapped(Renderer rnd, std::string Text, RGBA fg, int WrapLength)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderText_Blended_Wrapped(font.get(), Text.c_str(), fg.toSDLColor(), WrapLength), SDL_FreeSurface);
		return rnd.render(surf);
	}

	Texture Font::renderTextShaded(Renderer rnd, std::string Text, RGBA fg, RGBA bg)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderText_Shaded(font.get(), Text.c_str(), fg.toSDLColor(), bg.toSDLColor()), SDL_FreeSurface);
		return rnd.render(surf);
	}

	Texture Font::renderTextSolid(Renderer rnd, std::string Text, RGBA fg)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderText_Solid(font.get(), Text.c_str(), fg.toSDLColor()), SDL_FreeSurface);
		return rnd.render(surf);
	}

	Texture Font::renderUTF8(Renderer rnd, std::string Text, RGBA fg)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderUTF8_Blended(font.get(), Text.c_str(), fg.toSDLColor()), SDL_FreeSurface);
		return rnd.render(surf);
	}

	Texture Font::renderUTF8Wrapped(Renderer rnd, std::string Text, RGBA fg, int WrapLength)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderUTF8_Blended_Wrapped(font.get(), Text.c_str(), fg.toSDLColor(), WrapLength), SDL_FreeSurface);
		return rnd.render(surf);
	}

	Texture Font::renderUTF8Shaded(Renderer rnd, std::string Text, RGBA fg, RGBA bg)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderUTF8_Shaded(font.get(), Text.c_str(), fg.toSDLColor(), bg.toSDLColor()), SDL_FreeSurface);
		return rnd.render(surf);
	}

	Texture Font::renderUTF8Solid(Renderer rnd, std::string Text, RGBA fg)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderUTF8_Solid(font.get(), Text.c_str(), fg.toSDLColor()), SDL_FreeSurface);
		return rnd.render(surf);
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

    Timer::Timer()
    {
        _enabled=false;
        _detached=false;
        id=-1;
    }

    Timer::Timer(SDL_TimerCallback callback,Uint32 interval,void* param) : Timer()
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
		m.music.reset(temp, Mix_FreeMusic);
		return m;
	}

	int MusicPlayer::play(Music music, int loops)
	{
		m = music;
		return Mix_PlayMusic(m.music.get(), loops);
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
		return Mix_FadeInMusic(m.music.get(), loops, ms);
	}

	int MusicPlayer::fadeOut(int ms)
	{
		return Mix_FadeOutMusic(ms);
	}

	bool MusicPlayer::isPlaying()
	{
		return Mix_PlayingMusic();
	}

	bool MusicPlayer::isPaused()
	{
		return Mix_PausedMusic();
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
		s.sound.reset(temp, Mix_FreeChunk);
		return s;
	}

	ChannelID SoundPlayer::playSound(Sound sound, int loops) throw(ErrorViewer)
	{
		ChannelID id;
		if (-1 == (id = Mix_PlayChannel(-1, sound.sound.get(), loops)))
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
		if (-1 == (id = Mix_FadeInChannel(-1, sound.sound.get(), loops, ms)))
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
