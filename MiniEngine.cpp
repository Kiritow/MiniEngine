#include "MiniEngine.h"
#include <algorithm>
#include <map>
#include <mutex>

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

	Texture Font::renderText(Renderer rnd, std::string Text, RGBA fg)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderText_Blended(font.get(), Text.c_str(), fg.toSDLColor()));
		return rnd.render(surf);
	}

	Texture Font::renderTextWrapped(Renderer rnd, std::string Text, RGBA fg, int WrapLength)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderText_Blended_Wrapped(font.get(), Text.c_str(), fg.toSDLColor(), WrapLength));
		return rnd.render(surf);
	}

	Texture Font::renderTextShaded(Renderer rnd, std::string Text, RGBA fg, RGBA bg)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderText_Shaded(font.get(), Text.c_str(), fg.toSDLColor(), bg.toSDLColor()));
		return rnd.render(surf);
	}

	Texture Font::renderTextSolid(Renderer rnd, std::string Text, RGBA fg)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderText_Solid(font.get(), Text.c_str(), fg.toSDLColor()));
		return rnd.render(surf);
	}

	Texture Font::renderUTF8(Renderer rnd, std::string Text, RGBA fg)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderUTF8_Blended(font.get(), Text.c_str(), fg.toSDLColor()));
		return rnd.render(surf);
	}

	Texture Font::renderUTF8Wrapped(Renderer rnd, std::string Text, RGBA fg, int WrapLength)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderUTF8_Blended_Wrapped(font.get(), Text.c_str(), fg.toSDLColor(), WrapLength));
		return rnd.render(surf);
	}

	Texture Font::renderUTF8Shaded(Renderer rnd, std::string Text, RGBA fg, RGBA bg)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderUTF8_Shaded(font.get(), Text.c_str(), fg.toSDLColor(), bg.toSDLColor()));
		return rnd.render(surf);
	}

	Texture Font::renderUTF8Solid(Renderer rnd, std::string Text, RGBA fg)
	{
		Surface surf;
		surf.surf.reset(TTF_RenderUTF8_Solid(font.get(), Text.c_str(), fg.toSDLColor()));
		return rnd.render(surf);
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

	int Event::gettype()
	{
		return e.type;
	}

	int MouseButtonEvent::getx()
	{
		return e.button.x;
	}

	int MouseButtonEvent::gety()
	{
		return e.button.y;
	}

	int MouseButtonEvent::getbutton()
	{
		return e.button.button;
	}

	Event EventEngine::poll(bool mustNew) /// mustNew: false=SDL_PollEvent(&e) returns 0 ; true=SDL_PollEvent(&e) returns 1
	{
		Event e;
		while (1)
		{
			int ret = SDL_PollEvent(&e.e);
			if ((mustNew&&ret) || !mustNew) break;
		}
		return e;
	}

	Event EventEngine::wait()
	{
		Event e;
		SDL_WaitEvent(&e.e);
		return e;
	}

	Event EventEngine::waitfor(int ms)
	{
		Event e;
		SDL_WaitEventTimeout(&e.e, ms);
		return e;
	}


	namespace EventHandle
	{
	    const int _internal_event_num=46;

	    std::vector<std::map<int,DispatcherType>> disvec(_internal_event_num);

        int TransIDToEvent(int ID)
        {
            switch(ID)
            {
            case 0:
                return SDL_QUIT;
            case 1:
                return SDL_APP_TERMINATING;
            case 2:
                return SDL_APP_LOWMEMORY;
            case 3:
                return SDL_APP_WILLENTERBACKGROUND;
            case 4:
                return SDL_APP_DIDENTERBACKGROUND;
            case 5:
                return SDL_APP_WILLENTERFOREGROUND;
            case 6:
                return SDL_APP_DIDENTERFOREGROUND;
            case 7:
                return SDL_WINDOWEVENT;
            case 8:
                return SDL_SYSWMEVENT;
            case 9:
                return SDL_KEYDOWN;
            case 10:
                return SDL_KEYUP;
            case 11:
                return SDL_TEXTEDITING;
            case 12:
                return SDL_TEXTINPUT;
            case 13:
                return SDL_KEYMAPCHANGED;
            case 14:
                return SDL_MOUSEMOTION;
            case 15:
                return SDL_MOUSEBUTTONDOWN;
            case 16:
                return SDL_MOUSEBUTTONUP;
            case 17:
                return SDL_MOUSEWHEEL;
            case 18:
                return SDL_JOYAXISMOTION;
            case 19:
                return SDL_JOYBALLMOTION;
            case 20:
                return SDL_JOYHATMOTION;
            case 21:
                return SDL_JOYBUTTONDOWN;
            case 22:
                return SDL_JOYBUTTONUP;
            case 23:
                return SDL_JOYDEVICEADDED;
            case 24:
                return SDL_JOYDEVICEREMOVED;
            case 25:
                return SDL_CONTROLLERAXISMOTION;
            case 26:
                return SDL_CONTROLLERBUTTONDOWN;
            case 27:
                return SDL_CONTROLLERBUTTONUP;
            case 28:
                return SDL_CONTROLLERDEVICEADDED;
            case 29:
                return SDL_CONTROLLERDEVICEREMOVED;
            case 30:
                return SDL_CONTROLLERDEVICEREMAPPED;
            case 31:
                return SDL_FINGERDOWN;
            case 32:
                return SDL_FINGERUP;
            case 33:
                return SDL_FINGERMOTION;
            case 34:
                return SDL_DOLLARGESTURE;
            case 35:
                return SDL_DOLLARRECORD;
            case 36:
                return SDL_MULTIGESTURE;
            case 37:
                return SDL_CLIPBOARDUPDATE;
            case 38:
                return SDL_DROPFILE;
            case 39:
                return SDL_DROPTEXT;
            case 40:
                return SDL_DROPBEGIN;
            case 41:
                return SDL_DROPCOMPLETE;
            case 42:
                return SDL_AUDIODEVICEADDED;
            case 43:
                return SDL_AUDIODEVICEREMOVED;
            case 44:
                return SDL_RENDER_TARGETS_RESET;
            case 45:
                return SDL_RENDER_DEVICE_RESET;
            default:
                return 0;/// Nothing.
            }
        }

        int TransEventToID(int EventType)
        {
            switch(EventType)
            {
            case SDL_QUIT:
                return 0;
            case SDL_APP_TERMINATING:
                return 1;
            case SDL_APP_LOWMEMORY:
                return 2;
            case SDL_APP_WILLENTERBACKGROUND:
                return 3;
            case SDL_APP_DIDENTERBACKGROUND:
                return 4;
            case SDL_APP_WILLENTERFOREGROUND:
                return 5;
            case SDL_APP_DIDENTERFOREGROUND:
                return 6;
            case SDL_WINDOWEVENT:
                return 7;
            case SDL_SYSWMEVENT:
                return 8;
            case SDL_KEYDOWN:
                return 9;
            case SDL_KEYUP:
                return 10;
            case SDL_TEXTEDITING:
                return 11;
            case SDL_TEXTINPUT:
                return 12;
            case SDL_KEYMAPCHANGED:
                return 13;
            case SDL_MOUSEMOTION:
                return 14;
            case SDL_MOUSEBUTTONDOWN:
                return 15;
            case SDL_MOUSEBUTTONUP:
                return 16;
            case SDL_MOUSEWHEEL:
                return 17;
            case SDL_JOYAXISMOTION:
                return 18;
            case SDL_JOYBALLMOTION:
                return 19;
            case SDL_JOYHATMOTION:
                return 20;
            case SDL_JOYBUTTONDOWN:
                return 21;
            case SDL_JOYBUTTONUP:
                return 22;
            case SDL_JOYDEVICEADDED:
                return 23;
            case SDL_JOYDEVICEREMOVED:
                return 24;
            case SDL_CONTROLLERAXISMOTION:
                return 25;
            case SDL_CONTROLLERBUTTONDOWN:
                return 26;
            case SDL_CONTROLLERBUTTONUP:
                return 27;
            case SDL_CONTROLLERDEVICEADDED:
                return 28;
            case SDL_CONTROLLERDEVICEREMOVED:
                return 29;
            case SDL_CONTROLLERDEVICEREMAPPED:
                return 30;
            case SDL_FINGERDOWN:
                return 31;
            case SDL_FINGERUP:
                return 32;
            case SDL_FINGERMOTION:
                return 33;
            case SDL_DOLLARGESTURE:
                return 34;
            case SDL_DOLLARRECORD:
                return 35;
            case SDL_MULTIGESTURE:
                return 36;
            case SDL_CLIPBOARDUPDATE:
                return 37;
            case SDL_DROPFILE:
                return 38;
            case SDL_DROPTEXT:
                return 39;
            case SDL_DROPBEGIN:
                return 40;
            case SDL_DROPCOMPLETE:
                return 41;
            case SDL_AUDIODEVICEADDED:
                return 42;
            case SDL_AUDIODEVICEREMOVED:
                return 43;
            case SDL_RENDER_TARGETS_RESET:
                return 44;
            case SDL_RENDER_DEVICE_RESET:
                return 45;
            default:
                return -1;/// Error
            }
        }

		std::mutex mdisvec_counter;
		int disvec_counter = 0;

		std::map<int, UpdaterType> upvec;

		std::mutex mupvec_counter;
		int upvec_counter = 0;

		int RegistDispatcher(int EventType,DispatcherType func)
		{
			mdisvec_counter.lock();
			int id = disvec_counter++;
			mdisvec_counter.unlock();

			disvec.at(TransEventToID(EventType)).insert(make_pair(id, func));
			return id;
		}

        int UnregistDispatcher(int callbackid)
        {
            for (auto it = disvec.begin(); it != disvec.end(); it++)
            {
                for(auto i=it->begin(); i!=it->end(); i++)
                {
                    if (callbackid == i->first)
                    {
                        it->erase(i);
                        return 0;
                    }
                }
            }
            return -1;
        }

		int RegistUpdater(UpdaterType func)
		{
			mupvec_counter.lock();
			int id = upvec_counter++;
			mupvec_counter.unlock();

			upvec.insert(make_pair(id, func));
			return id;
		}

		int UnregistUpdater(int callbackid)
		{
			for (auto it = upvec.begin(); it != upvec.end(); it++)
			{
				if (callbackid == it->first)
				{
					upvec.erase(it);
					return 0;
				}
			}
			return -1;
		}

		void Dispatcher(SDL_Event e, int & running, int & update)
		{
			for (auto& func : disvec.at(TransEventToID(e.type)))
			{
				int r = 1;
				int u = 0;
				func.second(e, r, u);
				running &= r;
				update |= u;
			}
		}

		void Updater(Renderer & rnd)
		{
			for (auto& func : upvec)
			{
				func.second(rnd);
			}
		}

		void Loop(Renderer & rnd)
		{
			SDL_Event e;
			int running = 1;
			int update = 1;
			while (running)
			{
				while (!update&&SDL_WaitEvent(&e))
				{
					Dispatcher(e, running, update);
				}

				rnd.clear();
				Updater(rnd);
				rnd.update();
				update = 0;
			}
		}
	}/// End of namespace EventHandle



	AudioPlayer::_Audio* AudioPlayer::_sysAudio = nullptr;
	int AudioPlayer::_sysAudioCounter = 0;

}/// End of namespace MiniEngine

 /// Default Setup Code
int main(int argc, char* argv[])
{
	MiniEngine::SDLSystem::Init();
	int ret = AppMain();
	MiniEngine::SDLSystem::Quit();
	return ret;
}
