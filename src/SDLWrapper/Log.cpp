#include "Log.h"
#include "begin_code.h"
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
#include "end_code.h"
