#include "SDLSystem.h"
#include "_caster.h"
#include "begin_code.h"
SDLSystem::SDLSystem(const std::initializer_list<SDLInitFlag>& flag_sdl,
                     const std::initializer_list<IMGInitFlag>& flag_img,
                     const std::initializer_list<MixInitFlag>& flag_mix,
                     bool init_ttf ) throw (ErrorViewer)
{
    Uint32 sdl_flag=0;
    for(auto& v:flag_sdl)
    {
        sdl_flag |= _internal::getUint32FromSDLInitFlag(v);
    }
    int img_flag=0;
    for(auto& v:flag_img)
    {
        img_flag |= _internal::getIntFromIMGInitFlag(v);
    }
    int mix_flag=0;
    for(auto& v:flag_mix)
    {
        mix_flag |= _internal::getIntFromMixInitFlag(v);
    }

    int ret=SDL_Init(sdl_flag);
    if(ret!=0)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }

    ret=IMG_Init(img_flag);
    if(ret!=img_flag) /// IMG_Init returns its parameter on success.
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }

    ret=Mix_Init(mix_flag);
    if(ret!=mix_flag) /// Mix_Init returns its parameter on success.
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }

    if(init_ttf)
    {
        ret=TTF_Init();
        if(ret!=0)
        {
            ErrorViewer e;
            e.fetch();
            throw e;
        }
    }
}

SDLSystem::~SDLSystem()
{
    if(TTF_WasInit())
    {
        TTF_Quit();
    }

    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
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

//static
int SDLSystem::SetClipboardText(const std::string& str)
{
    return SDL_SetClipboardText(str.c_str());
}

//static
std::string SDLSystem::GetClipboardText()
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

//static
bool SDLSystem::HasClipboardText()
{
    return SDL_HasClipboardText()==SDL_TRUE;
}

#include "end_code.h"
