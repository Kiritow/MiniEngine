#include "MiniEngine.h"

namespace MiniEngine
{

/// Android Device
#if defined(__ANDROID__) && __ANDROID__
std::string SDLSystem::Android::GetInternal()
{
    return std::string(SDL_AndroidGetInternalStoragePath());
}

std::string SDLSystem::Android::GetExternal()
{
    return std::string(SDL_AndroidGetExternalStoragePath());
}

bool SDLSystem::Android::CanReadExternal()
{
    return SDL_AndroidGetExternalStorageState() & SDL_ANDROID_EXTERNAL_STORAGE_READ;
}

bool SDLSystem::Android::CanWriteExternal()
{
    return SDL_AndroidGetExternalStorageState() & SDL_ANDROID_EXTERNAL_STORAGE_WRITE;
}

bool SDLSystem::Android::ExternalAvaliable()
{
    return SDL_AndroidGetExternalStorageState() != 0;
}
void* SDLSystem::Android::GetJNIEnv()
{
    return SDL_AndroidGetJNIEnv();
}

/// Not An Android Device
#else

std::string SDLSystem::Android::GetInternal()
{
    return "";
}

std::string SDLSystem::Android::GetExternal()
{
    return "";
}

bool SDLSystem::Android::CanReadExternal()
{
    return false;
}

bool SDLSystem::Android::CanWriteExternal()
{
    return false;
}

bool SDLSystem::Android::ExternalAvaliable()
{
    return false;
}
void* SDLSystem::Android::GetJNIEnv()
{
    return nullptr;
}

#endif

}///End of namespace MiniEngine
