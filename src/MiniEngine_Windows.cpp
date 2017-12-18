#include "MiniEngine.h"
using namespace std;

#include <windows.h>

int _utf8_to_gb(const char* src, char* dst, int len)
{
    int ret = 0;
    WCHAR* strA;
    int i= MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
    if (i <= 0) {
        return -1;
    }
    strA = (WCHAR*)malloc(i * 2);
    MultiByteToWideChar(CP_UTF8, 0, src, -1, strA, i);
    i = WideCharToMultiByte(CP_ACP, 0, strA, -1, NULL, 0, NULL, NULL);
    if (len >= i) {
        ret = WideCharToMultiByte(CP_ACP, 0, strA, -1, dst, i, NULL, NULL);
        dst[i] = 0;
    }
    if (ret <= 0) {
        free(strA);
        return -2;
    }
    free( strA );
    return 0;
}

int _gb_to_utf8(const char* src, char* dst, int len)
{
    int ret = 0;
    WCHAR* strA;
    int i= MultiByteToWideChar(CP_ACP, 0, src, -1, NULL, 0);
    if (i <= 0) {
        return -1;
    }
    strA = (WCHAR*)malloc(i * 2);
    MultiByteToWideChar(CP_ACP, 0, src, -1, strA, i);
    i = WideCharToMultiByte(CP_UTF8, 0, strA, -1, NULL, 0, NULL, NULL);
    if (len >= i) {
        ret = WideCharToMultiByte(CP_UTF8, 0, strA, -1, dst, i, NULL, NULL);
        dst[i] = 0;
    }

    if (ret <= 0) {
        free(strA);
        return -2;
    }
    free(strA);
    return 0;
}

string UTF8ToGBK(string UTF8String)
{
    int sz=UTF8String.size()*2/3+256;
    auto gbkstr=new char[sz];
    memset(gbkstr,0,sz);

    if(_utf8_to_gb(UTF8String.c_str(),gbkstr,sz)!=0)
    {
        return "[MiniEngine] UT8ToGBK Convert Failed";
    }

    string s((char*)gbkstr);

    delete[] gbkstr;
    return s;
}

string GBKToUTF8(string GBKString)
{
    int sz=GBKString.size()*3/2+32;
    auto utf8str=new char[sz];
    memset(utf8str,0,sz);

    if(_gb_to_utf8(GBKString.c_str(),utf8str,sz)!=0)
    {
        return "[MiniEngine] GBKToUTF8 Convert Failed";
    }

    string s(utf8str);

    delete[] utf8str;
    return s;
}

#ifdef _MSC_VER
bool isexist(std::string Path)
{
    return false;
}

bool canread(std::string Path)
{
    return false;
}

bool canwrite(std::string Path)
{
    return false;
}

bool canexecute(std::string Path)
{
    return false;
}
#endif /// End of _MSC_VER
