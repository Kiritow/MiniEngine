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
