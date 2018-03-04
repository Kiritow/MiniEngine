#pragma once
#include "include.h"
namespace MiniEngine {
class LogSystem
{
    static void v(const char* fmt,...);/// Verbose
    static void d(const char* fmt,...);/// Debug
    static void i(const char* fmt,...);/// Information
    static void w(const char* fmt,...);/// Warning
    static void e(const char* fmt,...);/// Error
    static void critical(const char* fmt,...);/// Critical
};
} /// End of namespace MiniEngine
