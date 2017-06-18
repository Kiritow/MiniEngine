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
