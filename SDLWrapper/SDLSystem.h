#pragma once
#include "include.h"
#include "_PowerState.h"
#include "_Platform.h"
#include "_SDLInitFlag.h"
#include "_IMGInitFlag.h"
#include "_MixInitFlag.h"
#include "__Noncopyable.h"
#include "__Nonmoveable.h"
#include "ErrorViewer.h"
#include <tuple>
#include <string>
#include "begin_code.h"

class SDLSystem : public NonCopyable, public NonMoveable
{
public:
    SDLSystem(const std::initializer_list<SDLInitFlag>& flag_sdl = {SDLInitFlag::All} ,
              const std::initializer_list<IMGInitFlag>& flag_img = {IMGInitFlag::JPG,IMGInitFlag::PNG} ,
              const std::initializer_list<MixInitFlag>& flag_mix = {MixInitFlag::MP3} ,
              bool init_ttf = true ) throw (ErrorViewer);
    /// Experimental Constructor
    SDLSystem(Uint32 sdl_flag, Uint32 img_flag, Uint32 mix_flag, bool init_ttf) throw (ErrorViewer);
    ~SDLSystem();

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

    static int SetClipboardText(const std::string& str);
    static std::string GetClipboardText();
    static bool HasClipboardText();

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

private:
    void _init(Uint32,Uint32,Uint32,bool) throw (ErrorViewer);
    void _quit();
};
#include "end_code.h"
