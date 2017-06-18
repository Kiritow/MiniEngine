#pragma once
#include "MiniEngine_Config.h"
#include <string>
#include <memory>
#include <functional>
#include <vector>

#define _MINIENGINE_SDL_VERSION_ATLEAST(X,Y,Z) SDL_VERSION_ATLEAST(X,Y,Z)

namespace MiniEngine
{


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



	enum class SystemCursorType
	{
	    Arrow, Ibeam, CrossHair,
        Wait, WaitArrow,
        SizeNWSE, SizeNESW, SizeWE, SizeNS, SizeAll,
        No, Hand
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

	class WindowMessageBoxButton
	{
    public:
        WindowMessageBoxButton();
        WindowMessageBoxButton(const std::string& ButtonText,const std::function<void()>& CallbackFunc=[](){});

        std::string text;
        std::function<void()> callback;

        /// Default: no hit option set.
        void setHitAsReturn(bool);
        void setHitAsEscape(bool);

        bool isHitAsReturn() const;
        bool isHitAsEscape() const;
    private:
        int _hitoption;
	};

	class WindowMessageBox
	{
    public:
        WindowMessageBox();
        WindowMessageBox(const std::string& Title,const std::string& Text,MessageBoxType BoxType=MessageBoxType::Information,const std::function<void()>& DefaultCallback=[](){});

        MessageBoxType boxtype;
        std::string title;
        std::string text;
        std::function<void()> defaultcallback;

        void addButton(const WindowMessageBoxButton& button);
        int getButtonNum() const;
        WindowMessageBoxButton& getButton(int index);
        const WindowMessageBoxButton& getButtonConst(int index) const;
    private:
        std::vector<WindowMessageBoxButton> _vec;
	};



	enum class RendererType { Software, Accelerated, PresentSync, TargetTexture };

	enum class FlipMode { None, Horizontal, Vertical };



	enum class FontStyle { Normal, Bold, Italic, UnderLine, StrikeThrough };
	enum class FontHint { Normal, Light, Mono, None , Error };


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

        static int GetCPUCount();
        static int GetCPUCacheLineSize();
        /// RAM is calculated in MB.
        static int GetSystemRAM();

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
