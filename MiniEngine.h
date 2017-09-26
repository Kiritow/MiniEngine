#pragma once
#include <string>
#include <memory>
#include <functional>
#include "SDLWrapper/IncludeAll.h"

namespace MiniEngine
{
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

    /// Experimental - For Experts: Use SDL ScanCode
    bool GetScanKeyState(SDL_Scancode);

}/// End of namespace MiniEngine

std::string UTF8ToGBK(std::string UTF8String);
std::string GBKToUTF8(std::string GBKString);
bool canread(std::string Path);
bool canwrite(std::string Path);
bool isexist(std::string Path);
bool canexecute(std::string Path);

/// Return Value
/// true: transfer succeed. Value is outputed to _out_wstr
/// false: transfer failed. _out_wstr is not modified.
bool _ANSIToUnicode(const std::string& str,std::wstring& _out_wstr);
bool _UTF8ToUnicode(const std::string& str,std::wstring& _out_wstr);

/// Return Value
/// true: transfer succeed. Value is outputed to _out_str
/// false: transfer failed. _out_str is not modified.
bool _UnicodeToANSI(const std::wstring& wstr,std::string& _out_str);
bool _UnicodeToUTF8(const std::wstring& wstr,std::string& _out_str);
