#pragma once
#include "include.h"
#include "_PowerState.h"
#include "_Platform.h"
#include <tuple>
#include <string>
#include "begin_code.h"
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
#include "end_code.h"
