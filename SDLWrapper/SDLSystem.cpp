#include "SDLSystem.h"
#include "begin_code.h"
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
#include "end_code.h"
