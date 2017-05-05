#pragma once

#ifdef _MSC_VER
/// Visual Studio (VC++ Compiler)
#include <SDL.h>
#undef main
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

/// VC++ does not implied C++ exception. Use this to ignore compile warning on this.
#pragma warning (disable:4290)
#else
/// CodeBlocks (MinGW Compiler)
#include <SDL2/SDL.h>
#undef main
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#endif

#include <string>
#include <memory>
#include <functional>

#define _DECL_DEPRECATED [[deprecated]]

namespace MiniEngine
{

	class Rect
	{
	public:
		int x, y, w, h;
		Rect(int X, int Y, int W, int H);
		Rect();
		SDL_Rect toSDLRect();
	};

	class Point
	{
	public:
		int x, y;
		Point(int X, int Y);
		Point();
		SDL_Point toSDLPoint();
		bool inRect(Rect rect);
	};

	class ColorMode
	{
	public:
		int r, g, b;
		ColorMode(int R, int G, int B);
		ColorMode();
	};

	class RGBA
	{
	public:
		int r, g, b, a;
		RGBA(int R, int G, int B, int A);
		RGBA(ColorMode mode, int A);
		RGBA();
		SDL_Color toSDLColor();
		ColorMode toColorMode();
	};

	class NonCopyable
	{
	protected:
		NonCopyable() = default;
		~NonCopyable() = default;
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator = (const NonCopyable&) = delete;
	};

	class ErrorViewer : public std::exception
	{
	public:
		void fetch();
		std::string getError();
		const char* what() const throw() override;
	private:
		std::string str;
	};

    class RWOP
	{
    public:
        RWOP(FILE* fp,bool autoclose);
        RWOP(const std::string& filename,const std::string& openmode);
        RWOP(const void* mem,int size);
        RWOP(void* mem,int size);
        RWOP()=default;
        ~RWOP()=default;
    private:
        std::shared_ptr<SDL_RWops> _op;
        SDL_RWops* _get();
        void _clear();
        void _set(SDL_RWops*);
        friend class Renderer;
	};

	enum class BlendMode { None,Blend,Add,Mod };

	class Surface
	{
	public:
		~Surface() = default;
        int savePNG(const std::string& filename);
        int getw();
        int geth();
        BlendMode getBlendMode();
        int setBlendMode(BlendMode mode);

        int blit(Surface s,Rect src,Rect dst);
        int blitTo(Surface t, Rect dst);
		int blitTo(Surface t, Point lupoint);
		int blitFill(Surface t, Rect src);
		int blitFullFill(Surface t);

		int blitScaled(Surface s,Rect src,Rect dst);
		int blitScaledTo(Surface t, Rect dst);
		int blitScaledTo(Surface t, Point lupoint);
		int blitScaledFill(Surface t, Rect src);
		int blitScaledFullFill(Surface t);

        int setAlphaMode(int alpha);
		int getAlphaMode();

		ColorMode getColorMode();
		int setColorMode(ColorMode mode);
		RGBA getRGBA();
		void setRGBA(RGBA pack);

		bool mustlock();
        int lock();
        void unlock();

        static Surface createSurface(int width,int height,int depth,int Rmask,int Gmask,int Bmask,int Amask) throw(ErrorViewer);

	protected:
		Surface() = default;
	private:
	    std::shared_ptr<SDL_Surface> _surf;
        void _set(SDL_Surface*);
        void _clear();
        SDL_Surface* _get();
        std::shared_ptr<SDL_Surface>& _getex();

		friend class Window;
		friend class Renderer;
		friend class Font;
		friend class Cursor;
	};

	class Texture
	{
	public:
	    Texture();
		~Texture() = default;
		Rect getSize();
		int getw();
		int geth();
		bool isReady();
		int setBlendMode(BlendMode mode);
		BlendMode getBlendMode();
		/// Alpha:  0: Transparent 255: opaque
		int setAlphaMode(int alpha);
		int getAlphaMode();

		ColorMode getColorMode();
		int setColorMode(ColorMode mode);
		RGBA getRGBA();
		void setRGBA(RGBA pack);

	protected:
		/// updateInfo() must be called after Texture is changed.
		void updateInfo();
	private:
		std::shared_ptr<SDL_Texture> _text;
		void _set(SDL_Texture*);
		void _clear();
		SDL_Texture* _get();
		Rect rect;
		friend class Renderer;
	};

	enum class RendererType { Software, Accelerated, PresentSync, TargetTexture };

	enum class FlipMode { None, Horizontal, Vertical };

	class Renderer
	{
	public:
	    Renderer() = default;
		int setColor(RGBA pack);
		RGBA getColor();
		int setBlendMode(BlendMode mode);
		BlendMode getBlendMode();

		int setTarget(Texture& t);
		int setTarget();

		int fillRect(Rect rect);
		int drawRect(Rect rect);
		int drawPoint(Point p);

		int clear();
		void update();

		int copy(Texture t, Rect src, Rect dst);
		int copyTo(Texture t, Rect dst);
		int copyTo(Texture t, Point lupoint);
		int copyFill(Texture t, Rect src);
		int copyFullFill(Texture t);

		int supercopy(Texture t,bool srcfull,Rect src,bool dstfull,Rect dst,double angle,bool haspoint,Point center,FlipMode mode);

		Surface loadSurface(std::string FileName) throw(ErrorViewer);
		Surface loadSurfaceRW(RWOP rwop) throw(ErrorViewer);
		Texture render(Surface surf) throw (ErrorViewer);
		Texture loadTexture(std::string FileName) throw(ErrorViewer);
		Texture loadTextureRW(RWOP rwop) throw(ErrorViewer);
		Texture createTexture(int Width, int Height) throw(ErrorViewer);

		bool isReady();
	private:
		std::shared_ptr<SDL_Renderer> _rnd;
		void _set(SDL_Renderer*);
		void _clear();
		SDL_Renderer* _get();

		friend class Window;
	};

	enum class SystemCursorType
	{
	    Arrow, Ibeam, CrossHair,
        Wait, WaitArrow,
        SizeNWSE, SizeNESW, SizeWE, SizeNS, SizeAll,
        No, Hand
    };

	class Cursor
	{
    public:
        static Cursor CreateSystemCursor(SystemCursorType);
        static Cursor CreateCursor(Surface surf,Point hotspot={0,0});

        static Cursor GetActiveCursor();
        static Cursor GetDefaultCursor();

        static void show(bool);
        static bool isShow();

        void activate();
    private:
        std::shared_ptr<SDL_Cursor> _cur;
        void _set(SDL_Cursor*);
        void _set_no_delete(SDL_Cursor*);
        SDL_Cursor* _get();
        void _clear();
	};

	enum class MessageBoxType { Error, Warning, Information };

	class Window
	{
	public:
		Window(std::string Title, int Width, int Height, std::initializer_list<RendererType> RendererFlags = { RendererType::Accelerated,RendererType::TargetTexture }) throw(ErrorViewer);
		Renderer getRenderer() const;

		void setRenderer(RendererType Type)
		{
		    _internal_rndflagcalc=0;
            _setRenderer(Type);
		}

		template<typename... Args>
		void setRenderer(RendererType Type,Args&&... args)
		{
            _internal_rndflagcalc=0;
            _setRenderer(Type,std::forward<RendererType>(args...));
		}

		void setRenderer(std::initializer_list<RendererType>);

		Rect getSize();
		void setSize(Rect sizeRect);
		void setSize(int w, int h);

		Rect getPosition();
		void setPosition(int x, int y);
		/// FIXME: Use class Point or class Rect ?
		void setPosition(Point point);


		void setTitle(std::string Title);
		std::string getTitle();

		void setGrab(bool);
		bool getGrab();

		void setResizable(bool resizable);

		/// Use UTF8 in Title and Message please.
		int showSimpleMessageBox(MessageBoxType type,std::string Title,std::string Message);

		void show();
		void hide();
		void raise();
		void minimize();
		void maximize();
		void restore();


		_DECL_DEPRECATED Surface getSurface();
    protected:
        template<typename... Args>
        void _setRenderer(RendererType Type,Args&&... args)
        {
            _internal_rndflagcalc|=_render_caster(Type);
            _setRenderer(args...);
        }

        void _setRenderer(RendererType Type)
        {
            _internal_rndflagcalc|=_render_caster(Type);
            _setRenderer_Real(_internal_rndflagcalc);
        }
	private:
		void _setRenderer_Real(Uint32 flags);
		Uint32 _internal_rndflagcalc;
		Uint32 _render_caster(RendererType);

		std::shared_ptr<SDL_Window> _wnd;
		void _set(SDL_Window*);
		void _clear();
		SDL_Window* _get();

		Renderer winrnd;
	};

	class Font
	{
	public:
	    enum class Style { Normal, Bold, Italic, UnderLine, StrikeThrough };

		Font() = default;
		Font(std::string FontFileName, int size) throw(ErrorViewer);
		int use(std::string FontFileName, int size);
		bool isReady();

		template<typename... Args>
		void setFontStyle(Style style,Args&&... args)
		{
            _internal_fontcalc=0;
            _setFontStyle(style,std::forward(args...));
		}

		void setFontStyle(Style style)
		{
            _real_setFontStyle(_style_caster(style));
		}

		std::tuple<Style> getFontStyles();

		Surface renderText(std::string Text, RGBA fg);
		Surface renderTextWrapped(std::string Text, RGBA fg, int WrapLength);
		Surface renderTextShaded(std::string Text, RGBA fg, RGBA bg);
		Surface renderTextSolid(std::string Text, RGBA fg);

		Surface renderUTF8(std::string Text, RGBA fg);
		Surface renderUTF8Wrapped(std::string Text, RGBA fg, int WrapLength);
		Surface renderUTF8Shaded(std::string Text, RGBA fg, RGBA bg);
		Surface renderUTF8Solid(std::string Text, RGBA fg);

		Texture renderText(Renderer rnd, std::string Text, RGBA fg);
		Texture renderTextWrapped(Renderer rnd, std::string Text, RGBA fg, int WrapLength);
		Texture renderTextShaded(Renderer rnd, std::string Text, RGBA fg, RGBA bg);
		Texture renderTextSolid(Renderer rnd, std::string Text, RGBA fg);

		Texture renderUTF8(Renderer rnd, std::string Text, RGBA fg);
		Texture renderUTF8Wrapped(Renderer rnd, std::string Text, RGBA fg, int WrapLength);
		Texture renderUTF8Shaded(Renderer rnd, std::string Text, RGBA fg, RGBA bg);
		Texture renderUTF8Solid(Renderer rnd, std::string Text, RGBA fg);
    protected:
		template<typename... Args>
        void _setFontStyle(Style style,Args&&... args)
        {
            _internal_fontcalc|=_style_caster(style);
            _setFontStyle(args...);
        }

        void _setFontStyle(Style style)
        {
            _internal_fontcalc|=_style_caster(style);
            _real_setFontStyle(_internal_fontcalc);
        }
	private:
	    void _real_setFontStyle(int);
	    int _style_caster(Style);
        int _internal_fontcalc;

		std::shared_ptr<TTF_Font> _font;
		void _set(TTF_Font*);
		void _clear();
		TTF_Font* _get();
	};

	enum class Platform { Unknown,Windows,MacOS,Linux,iOS,Android };
	enum class PowerState { Unknown,OnBattery,NoBattery,Charging,Charged };

	class LogSystem
	{
	    static void v(const char* fmt,...);/// Verbose
	    static void d(const char* fmt,...);/// Debug
	    static void i(const char* fmt,...);/// Information
	    static void w(const char* fmt,...);/// Warning
	    static void e(const char* fmt,...);/// Error
	    static void critical(const char* fmt,...);/// Critical
	};

	class SDLSystem
	{
	public:
		static int SDLInit();
		static void SDLQuit();
		static int IMGInit();
		static void IMGQuit();
		static int TTFInit();
		static void TTFQuit();
		static int MixInit();
		static void MixQuit();

		static void Init();
		static void Quit();

		static void Delay(int ms);

		static PowerState GetPowerState();
		static int GetPowerLifeLeft();
		static int GetPowerPrecentageLeft();

		static Platform GetPlatform();

		static void StartTextInput();
		static void StopTextInput();

		class Android
		{
        public:
            static std::string GetInternal();
            static bool ExternalAvaliable();
            static bool CanReadExternal();
            static bool CanWriteExternal();
            static std::string GetExternal();
            static void* GetJNIEnv();
		};
	};

	Uint32 _global_timer_executor(Uint32 interval,void* param);

	class Timer
	{
    public:
        Timer();

        /// void func(Uint32,...)
        template<typename VoidCallable,typename... Args>
        Timer(Uint32 interval,VoidCallable&& vcallable,Args&&... args) : Timer()
        {
            auto realCall=[&](Uint32 ims)->Uint32{vcallable(ims,args...);return interval;};
            auto pfunc=new std::function<Uint32(Uint32 interval)>(realCall);
            _real_timer_call(_global_timer_executor,interval,pfunc);
        }

        /// Uint32 func(Uint32,...)
        template<typename Callable,typename... Args>
        Timer(Callable&& callable,Uint32 interval,Args&&... args) : Timer()
        {
            auto realCall=[&](Uint32 ims)->Uint32{return callable(ims,args...);};
            auto pfunc=new std::function<Uint32(Uint32 interval)>(realCall);
            _real_timer_call(_global_timer_executor,interval,pfunc);
        }

        /// Restore For Capability
        Timer(SDL_TimerCallback callback,Uint32 interval,void* param);

        int enable();
        int disable();
        bool isenable();
        void detach();
        ~Timer();

        static void _delete_delegator(std::function<Uint32(Uint32)>* Delegator);
    private:

        void _real_timer_call(SDL_TimerCallback callback,Uint32 interval,void* param);

        SDL_TimerCallback _callback;
        Uint32 _interval;
        void* _param;
        SDL_TimerID id;
        bool _enabled;
        bool _detached;
        /// Reserved Variable For Template variable Parameter
        bool _delete_on_disable;
	};

	class AudioPlayer
	{
	public:
		AudioPlayer();
		~AudioPlayer();
	private:
		class _Audio
		{
		public:
			_Audio();
			~_Audio();
		};

		static _Audio* _sysAudio;
		static int _sysAudioCounter;
	};

	/// Forward Declaration
	class Music
	{
	public:

	protected:
		Music() = default;
	private:
		std::shared_ptr<Mix_Music> _music;
        void _set(Mix_Music*);
        void _clear();
        Mix_Music* _get();
		friend class MusicPlayer;
	};

	class MusicPlayer : public AudioPlayer
	{
	public:
		Music loadMusic(std::string Filename) throw (ErrorViewer);

		int play(Music music, int loops);
		void pause();
		void resume();
		void rewind();
		int stop();
		int fadeIn(int loops, int ms);
		int fadeOut(int ms);

		bool isPlaying();
		bool isPaused();
		int isFading();

	private:
		Music m;
	};

	class Sound
	{
	public:
	protected:
		Sound() = default;
	private:
		std::shared_ptr<Mix_Chunk> _sound;
		void _set(Mix_Chunk*);
		void _clear();
		Mix_Chunk* _get();
		friend class SoundPlayer;
	};

	typedef int ChannelID;

	class SoundPlayer : public AudioPlayer
	{
	public:
		SoundPlayer(int Channels = 16);
		Sound loadSound(std::string Filename) throw (ErrorViewer);
		ChannelID playSound(Sound sound, int loops) throw (ErrorViewer);
		ChannelID fadein(Sound sound, int loops, int ms) throw (ErrorViewer);
		int fadeout(ChannelID id, int ms);
		void pause(ChannelID id);
		void resume(ChannelID id);
		int stop(ChannelID id);
	};

	class StringEngine
	{
    public:
        StringEngine(std::string StringFile,std::string LanguageTag);

        StringEngine(StringEngine& )=delete;
        StringEngine& operator = (StringEngine& )=delete;

        int useLanaguage(std::string LanguageTag);
        bool ready();
        std::string getString(std::string Tag);
        ~StringEngine();
    private:
        struct impl;
        impl* pimpl;
	};

}/// End of namespace MiniEngine

std::string UTF8ToGBK(std::string UTF8String);
std::string GBKToUTF8(std::string GBKString);
bool canread(std::string Path);
bool canwrite(std::string Path);
bool isexist(std::string Path);
bool canexecute(std::string Path);

 /// Your Program Should Start Here
int AppMain();

/// MiniEngine Provides main
int main(int argc,char* argv[]);

/// MiniEngine Provided API: Get Start Parameters
int GetArgc();
char** GetArgv();
