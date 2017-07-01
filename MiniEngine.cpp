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
