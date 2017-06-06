#pragma once
#include "MiniEngine_Config.h"
#include <string>
#include <memory>
#include <functional>
#include <vector>

#define _MINIENGINE_SDL_VERSION_ATLEAST(X,Y,Z) SDL_VERSION_ATLEAST(X,Y,Z)

namespace MiniEngine
{

	class Rect
	{
	public:
		int x, y, w, h;
		Rect(int X, int Y, int W, int H);
		Rect(const SDL_Rect&);
		Rect();
		SDL_Rect toSDLRect() const;
		bool isEmpty();
		bool operator == (const Rect&) const;
		bool hasIntersection(const Rect&) const;
		Rect getIntersection(const Rect&) const;
		Rect getUnion(const Rect&) const;
	};

	class Point
	{
	public:
		int x, y;
		Point(int X, int Y);
		Point();
		SDL_Point toSDLPoint() const;
		bool inRect(const Rect& rect) const;
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
		SDL_Color toSDLColor() const;
		ColorMode toColorMode() const;
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
		std::string getError() const;
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

        void release();
    private:
        std::shared_ptr<SDL_RWops> _op;
        SDL_RWops* _get() const;
        void _clear();
        void _set(SDL_RWops*);
        friend class Surface;
        friend class Renderer;
	};

	enum class BlendMode { None,Blend,Add,Mod };

	class Surface
	{
	public:
	    Surface()=default;
	    Surface(int width,int height,int depth,int Rmask,int Gmask,int Bmask,int Amask) throw(ErrorViewer);
	    Surface(int width,int height,int depth,RGBA colorPack) throw(ErrorViewer);
	    Surface(int width,int height,int depth,Uint32 surfaceFormat) throw(ErrorViewer);
        Surface(const std::string& filename) throw(ErrorViewer);
        Surface(const RWOP& rwop) throw(ErrorViewer);
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
	};

	class Texture
	{
	public:
	    Texture();
		~Texture() = default;
		Rect getSize();
		int getw() const;
		int geth() const;
		bool isReady() const;
		int setBlendMode(BlendMode mode);
		BlendMode getBlendMode() const;
		/// Alpha:  0: Transparent 255: opaque
		int setAlphaMode(int alpha);
		int getAlphaMode() const;

		ColorMode getColorMode() const;
		int setColorMode(ColorMode mode);
		RGBA getRGBA() const;
		void setRGBA(const RGBA& pack);

		void release();
	protected:
		/// updateInfo() must be called after Texture is changed.
		void updateInfo();
	private:
		std::shared_ptr<SDL_Texture> _text;
		void _set(SDL_Texture*);
		void _clear();
		SDL_Texture* _get() const;
		Rect rect;
		friend class Renderer;
	};

	enum class RendererType { Software, Accelerated, PresentSync, TargetTexture };

	enum class FlipMode { None, Horizontal, Vertical };

	class Renderer
	{
	public:
	    Renderer() = default;
		int setColor(const RGBA& pack);
		RGBA getColor() const;
		int setBlendMode(BlendMode mode);
		BlendMode getBlendMode() const;

		int setTarget(Texture& t);
		int setTarget();

		int fillRect(const Rect& rect);
		int drawRect(const Rect& rect);
		int drawPoint(const Point& p);

		int clear();
		void update();

		int copy(const Texture& t, const Rect& src, const Rect& dst);
		int copyTo(const Texture& t, const Rect& dst);
		int copyTo(const Texture& t, const Point& lupoint);
		int copyFill(const Texture& t, const Rect& src);
		int copyFullFill(const Texture& t);

		int supercopy(const Texture& t,
                bool srcfull,const Rect& src,bool dstfull,const Rect& dst,
                double angle,
                bool haspoint,const Point& center,FlipMode mode);

		Texture render(const Surface& surf) const throw (ErrorViewer);
		Texture loadTexture(const std::string& FileName) const throw(ErrorViewer);
		Texture loadTextureRW(const RWOP& rwop) const throw(ErrorViewer);
		Texture createTexture(int Width, int Height) const throw(ErrorViewer);

		bool isReady() const;

		void release();
	private:
		std::shared_ptr<SDL_Renderer> _rnd;
		void _set(SDL_Renderer*);
		void _clear();
		SDL_Renderer* _get() const;

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
        Cursor()=default;
        Cursor(SystemCursorType);
        Cursor(Surface surf,Point hotspot={0,0});

        static Cursor GetActiveCursor();
        static Cursor GetDefaultCursor();

        static void setShow(bool);
        static bool isShow();

        void activate();

        void release();
    private:
        std::shared_ptr<SDL_Cursor> _cur;
        void _set(SDL_Cursor*);
        void _set_no_delete(SDL_Cursor*);
        SDL_Cursor* _get();
        void _clear();
	};

	enum class MessageBoxType { Error, Warning, Information };

	enum class WindowType
	{
	    FullScreen, OpenGL, Shown, Hidden,
	    Borderless, Resizable, Minimized, Maximized,
	    InputGrabbed, InputFocus, MouseFocus,
	    FullScreenDesktop, Foreign, AllowHighDPI,
	    MouseCapture, AlwaysOnTop, SkipTaskBar,
	    Utility, ToolTip, PopUpMenu
	};

	class Window
	{
	public:
	    Window()=default;
		Window(std::string Title, int Width, int Height,
         std::initializer_list<RendererType> RendererFlags = { RendererType::Accelerated,RendererType::TargetTexture },
         std::initializer_list<WindowType> WindowFlags = {WindowType::Shown} ,
         int WindowPositionX=SDL_WINDOWPOS_CENTERED, int WindowPositionY=SDL_WINDOWPOS_CENTERED) throw(ErrorViewer);
		Renderer getRenderer() const;

		void setRenderer(RendererType Type)
		{
		    int flagcalc=0;
            _setRenderer(flagcalc,Type);
		}

		template<typename... Args>
		void setRenderer(RendererType Type,Args&&... args)
		{
            int flagcalc=0;
            _setRenderer(flagcalc,Type,std::forward<RendererType>(args...));
		}

		void setRenderer(std::initializer_list<RendererType>);

		Rect getSize() const;
		void setSize(const Rect& sizeRect);
		void setSize(int w, int h);

		Point getPosition() const;
		void setPosition(int x, int y);
		void setPosition(const Point& point);

		void setTitle(const std::string& Title);
		std::string getTitle() const;

		void setGrab(bool isGrab);
		bool getGrab() const;

		#if _MINIENGINE_SDL_VERSION_ATLEAST(2,0,5)
		/// SDL2.0.5 Required.
		int setOpacity(float opacity);
		float getOpacity() const;
		#endif

		/// FIXME: Not Implemented.
		void setResizable(bool resizable);

		/// Use UTF8 in Title and Message please.
		int showSimpleMessageBox(MessageBoxType type,const std::string& Title,const std::string& Message) const;

		void show();
		void hide();
		void raise();
		void minimize();
		void maximize();
		void restore();

		_DECL_DEPRECATED Surface getSurface();

		bool isScreenKeyboardShown();

		void release();

        /// Experimental : Free current renderer.
        /// This will cause all existing Renderer class throw an error on delete.
        /// This function destroy current renderer (if exist) and all Renderer class will not be notified.
        void resetRenderer();
    protected:
        template<typename... Args>
        void _setRenderer(int& refcalc,RendererType Type,Args&&... args)
        {
            refcalc|=_render_caster(Type);
            _setRenderer(refcalc,args...);
        }

        void _setRenderer(int& refcalc,RendererType Type)
        {
            refcalc|=_render_caster(Type);
            _setRenderer_Real(refcalc);
        }
	private:
		void _setRenderer_Real(Uint32 flags);
		Uint32 _render_caster(RendererType);

		std::shared_ptr<SDL_Window> _wnd;
		void _set(SDL_Window*);
		void _clear();
		SDL_Window* _get() const;

		Renderer _winrnd;
	};

	enum class FontStyle { Normal, Bold, Italic, UnderLine, StrikeThrough };
	enum class FontHint { Normal, Light, Mono, None , Error };

	class Font
	{
	public:
		Font() = default;
		Font(std::string FontFileName, size_t size) throw(ErrorViewer);
		int use(std::string FontFileName, size_t size);
		bool isReady() const;

		bool isNormal() const;
        bool isBold() const;
        bool isItalic() const;
        bool isUnderLine() const;
        bool isStrikeThrough() const;

        void setNormal();
        void setBold(bool);
        void setItalic(bool);
        void setUnderLine(bool);
        void setStrikeThrough(bool);

		template<typename... Args>
		void setFontStyle(FontStyle style,Args&&... args)
		{
		    int fontcalc=0;
		    _setFontStyle(fontcalc,style,args...);
		}

		void setFontStyle(FontStyle style)
		{
            int fontcalc=0;
            _setFontStyle(fontcalc,style);
		}

		std::vector<FontStyle> getFontStyles() const;

		int getFontHeight() const;
		int getFontAscent() const;
		int getFontDescent() const;
		int getFontLineSkip() const;

		bool isFontKerning() const;
		void setFontKerning(bool enableKerning);

		long getFontFaceNum() const;
        int getFontFaceIsFixedWidth() const;
        std::string getFontFaceFamilyName() const;
        std::string getFontFaceStyleName() const;

		FontHint getFontHint() const;
		void setFontHint(FontHint hint);


        Rect sizeText(const std::string& Text) const throw (ErrorViewer);
        Rect sizeUTF8(const std::string& Text) const throw (ErrorViewer);
        Rect sizeUnicode(const uint16_t* Text) const throw (ErrorViewer);

        /// Surface Rendering Functions.
		Surface renderText(const std::string& Text, const RGBA& fg) const;
		Surface renderTextWrapped(const std::string& Text, const RGBA& fg, size_t WrapLength) const;
		Surface renderTextShaded(const std::string& Text, const RGBA& fg, const RGBA& bg) const;
		Surface renderTextSolid(const std::string& Text, const RGBA& fg) const;

		Surface renderUTF8(const std::string& Text, const RGBA& fg) const;
		Surface renderUTF8Wrapped(const std::string& Text, const RGBA& fg, size_t WrapLength) const;
		Surface renderUTF8Shaded(const std::string& Text, const RGBA& fg, const RGBA& bg) const;
		Surface renderUTF8Solid(const std::string& Text, const RGBA& fg) const;

		Surface renderUnicode(const uint16_t* Text,const RGBA& fg) const;
		Surface renderUnicodeWrapped(const uint16_t* Text,const RGBA& fg,size_t WrapLength) const;
		Surface renderUnicodeShaded(const uint16_t* Text,const RGBA& fg,const RGBA& bg) const;
		Surface renderUnicodeSolid(const uint16_t* Text,const RGBA& fg) const;

		/// Texture Rendering Functions.
		Texture renderText(const Renderer& rnd, const std::string& Text, const RGBA& fg) const;
		Texture renderTextWrapped(const Renderer& rnd, const std::string& Text, const RGBA& fg, size_t WrapLength) const;
		Texture renderTextShaded(const Renderer& rnd, const std::string& Text, const RGBA& fg, const RGBA& bg) const;
		Texture renderTextSolid(const Renderer& rnd, const std::string& Text, const RGBA& fg) const;

		Texture renderUTF8(const Renderer& rnd, const std::string& Text, const RGBA& fg) const;
		Texture renderUTF8Wrapped(const Renderer& rnd, const std::string& Text, const RGBA& fg, size_t WrapLength) const;
		Texture renderUTF8Shaded(const Renderer& rnd, const std::string& Text, const RGBA& fg, const RGBA& bg) const;
		Texture renderUTF8Solid(const Renderer& rnd, const std::string& Text, const RGBA& fg) const;

		Texture renderUnicode(const Renderer& rnd,const uint16_t* Text,const RGBA& fg) const;
		Texture renderUnicodeWrapped(const Renderer& rnd,const uint16_t* Text,const RGBA& fg,size_t WrapLength) const;
		Texture renderUnicodeShaded(const Renderer& rnd,const uint16_t* Text,const RGBA& fg,const RGBA& bg) const;
		Texture renderUnicodeSolid(const Renderer& rnd,const uint16_t* Text,const RGBA& fg) const;

		void release();
    protected:
		template<typename... Args>
        void _setFontStyle(int& fontcalc,FontStyle style,Args&&... args)
        {
            fontcalc|=_style_caster(style);
            _setFontStyle(fontcalc,args...);
        }

        void _setFontStyle(int& fontcalc,FontStyle style)
        {
            fontcalc|=_style_caster(style);
            _real_setFontStyle(fontcalc);
        }
	private:
	    void _real_setFontStyle(int);
	    int _style_caster(FontStyle);

		std::shared_ptr<TTF_Font> _font;
		void _set(TTF_Font*);
		void _clear();
		TTF_Font* _get() const;
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

	class SharedLibrary
	{
    public:
        SharedLibrary();
        SharedLibrary(const std::string& Filename);
        ~SharedLibrary()=default;
        int load(const std::string& Filename);
        int unload();

        template<typename ReturnType,typename... Arguments>
        std::function<ReturnType(Arguments...)> get(const std::string& FunctionName)
        {
            return std::function<ReturnType(Arguments...)>(reinterpret_cast<ReturnType(*)(Arguments...)>(get(FunctionName)));
        }

        void* get(const std::string& FunctionName) const;
        void release();
    private:
        void* _get() const;
        void _set(void*);
        void _clear();
        std::shared_ptr<void> _obj;
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
		static bool IsTextInputActive();
		static void StopTextInput();

		static bool HasScreenKeyboardSupport();

        static std::tuple<int,int,int> GetSDLCompileVersion();
        static std::tuple<int,int,int> GetSDLLinkedVersion();

        static std::tuple<int,int,int> GetIMGCompileVersion();
        static std::tuple<int,int,int> GetIMGLinkedVersion();

        static std::tuple<int,int,int> GetMixCompileVersion();
        static std::tuple<int,int,int> GetMixLinkedVersion();

        static std::tuple<int,int,int> GetTTFCompileVersion();
        static std::tuple<int,int,int> GetTTFLinkedVersion();

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
            auto realCall=[&,vcallable](Uint32 ims)->Uint32{vcallable(ims,args...);return ims;};
            auto pfunc=new std::function<Uint32(Uint32 interval)>(realCall);
            _real_timer_call(_global_timer_executor,interval,pfunc);
        }

        /// Uint32 func(Uint32,...)
        template<typename Callable,typename... Args>
        Timer(Callable&& callable,Uint32 interval,Args&&... args) : Timer()
        {
            auto realCall=[&,callable](Uint32 ims)->Uint32{return callable(ims,args...);};
            auto pfunc=new std::function<Uint32(Uint32 interval)>(realCall);
            _real_timer_call(_global_timer_executor,interval,pfunc);
        }

        /// Restore For Capability
        Timer(SDL_TimerCallback callback,Uint32 interval,void* param);

        int enable();
        int disable();
        bool isenable() const;
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
	    static int GetDecoderNum();
		static std::string GetDecoderName(int index);

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

		/// Experimental
		static int SetMusicPosition(double position);

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
	    static int GetDecoderNum();
		static std::string GetDecoderName(int index);

		SoundPlayer(int Channels = 16);
		Sound loadSound(std::string Filename) throw (ErrorViewer);
		ChannelID playSound(Sound sound, int loops) throw (ErrorViewer);
		ChannelID fadein(Sound sound, int loops, int ms) throw (ErrorViewer);
		int fadeout(ChannelID id, int ms);
		void pause(ChannelID id);
		void resume(ChannelID id);
		int stop(ChannelID id);

		/// Experimental
		int setPanning(ChannelID id,uint8_t left,uint8_t right);
		int setPosition(ChannelID id,int16_t angle,uint8_t distance);
		int setDistance(ChannelID id,uint8_t distance);
		int setReverseStereo(ChannelID id,int flip);
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

    int SetClipboardText(const std::string& str);
    std::string GetClipboardText();
    bool HasClipboardText();

    /// Experimental - For Experts: Use SDL ScanCode
    bool GetScanKeyState(SDL_Scancode);

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
