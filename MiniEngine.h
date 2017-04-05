#pragma once
#include <SDL2/SDL.h>
#undef main
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <string>
#include <memory>
#include <functional>

#define _DECL_DEPRECATED __declspec(deprecated)
#define _DECL_DEPRECATED_MSG(InfoString) __declspec(deprecated(InfoString))

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

	enum class BlendMode
	{
		None = SDL_BLENDMODE_NONE,
		Blend = SDL_BLENDMODE_BLEND,
		Add = SDL_BLENDMODE_ADD,
		Mod = SDL_BLENDMODE_MOD
	};

	class Surface
	{
	public:
		~Surface() = default;
	protected:
		Surface() = default;
	private:
		std::shared_ptr<SDL_Surface> surf;
		friend class Window;
		friend class Renderer;
		friend class Font;
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
		std::shared_ptr<SDL_Texture> text;
		Rect rect;
		friend class Renderer;
	};

	enum class RendererType
	{
		Software = SDL_RENDERER_SOFTWARE,
		Accelerated = SDL_RENDERER_ACCELERATED,
		PresentSync = SDL_RENDERER_PRESENTVSYNC,
		TargetTexture = SDL_RENDERER_TARGETTEXTURE
	};

	enum class FlipMode { None, Horizontal, Vertical };

	class Renderer
	{
	public:
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
		Texture render(Surface surf) throw (ErrorViewer);
		Texture loadTexture(std::string FileName) throw(ErrorViewer);
		Texture createTexture(int Width, int Height) throw(ErrorViewer);

		Renderer() = default;
		bool isReady();
	private:
		std::shared_ptr<SDL_Renderer> rnd;
		friend class Window;
	};

	enum class MessageBoxType { Error,Warning,Information };

	class Window
	{
	public:
		Window(std::string Title, int Width, int Height, std::initializer_list<RendererType> RendererFlags = { RendererType::Accelerated,RendererType::TargetTexture }) throw(ErrorViewer);
		Renderer getRenderer() const;

		void setRenderer(std::initializer_list<RendererType> RendererFlags);

		Rect getSize();
		void setSize(Rect sizeRect);
		void setSize(int w, int h);

		Rect getPosition();
		void setPosition(int x, int y);
		/// FIXME: Use class Point or class Rect ?
		void setPosition(Point point);


		void setTitle(std::string Title);
		std::string getTitle();

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
	private:
		void _setRenderer_Real(Uint32 flags);
		std::shared_ptr<SDL_Window> wnd;
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
		std::shared_ptr<TTF_Font> font;
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

	class Timer
	{
    public:
        Timer();
        /// Uint32 func(Uint32,void*) ...
        Timer(SDL_TimerCallback callback,Uint32 interval,void* param);
        int enable();
        int disable();
        bool isenable();
        void detach();
        ~Timer();
    private:
        SDL_TimerCallback _callback;
        Uint32 _interval;
        void* _param;
        SDL_TimerID id;
        bool _enabled;
        bool _detached;
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
		std::shared_ptr<Mix_Music> music;
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
		std::shared_ptr<Mix_Chunk> sound;
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
