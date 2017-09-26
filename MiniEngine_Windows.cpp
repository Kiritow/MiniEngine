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

bool _ANSIToUnicode(const string& str,wstring& _out_wstr)
{
    int unicodeLen=MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,NULL,0);
    unique_ptr<wchar_t[]> pUnicode(new wchar_t[unicodeLen+1]);
    if(!pUnicode.get()) { return false; }
    memset(pUnicode.get(),0,sizeof(wchar_t)*(unicodeLen+1));
    MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,pUnicode.get(),unicodeLen);
    _out_wstr=wstring(pUnicode.get());
    return true;
}

bool _UnicodeToANSI(const wstring& str,string& _out_str)
{
    int iTextLen=WideCharToMultiByte(CP_ACP,0,str.c_str(),-1,NULL,0,NULL,NULL);
    unique_ptr<char[]> pElementText(new char[iTextLen+1]);
    if(!pElementText.get()) { return false; }
    memset(pElementText.get(),0,sizeof(char)*(iTextLen+1));
    WideCharToMultiByte(CP_ACP,0,str.c_str(),-1,pElementText.get(),iTextLen,NULL,NULL);
    _out_str=string(pElementText.get());
    return true;
}

bool _UTF8ToUnicode(const string& str,wstring& _out_wstr)
{
    int unicodeLen=MultiByteToWideChar(CP_UTF8,0,str.c_str(),-1,NULL,0);
    unique_ptr<wchar_t[]> pUnicode(new wchar_t[unicodeLen+1]);
    if(!pUnicode.get()) { return false; }
    memset(pUnicode.get(),0,(unicodeLen+1)*sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8,0,str.c_str(),-1,pUnicode.get(),unicodeLen);
    _out_wstr=wstring(pUnicode.get());
    return true;
}

bool _UnicodeToUTF8(const wstring& str,string& _out_str)
{
    int iTextLen=WideCharToMultiByte(CP_UTF8,0,str.c_str(),-1,NULL,0,NULL,NULL);
    unique_ptr<char[]> pElementText(new char[iTextLen+1]);
    if(!pElementText.get()) { return false; }
    memset(pElementText.get(),0,sizeof(char)*(iTextLen+1));
    WideCharToMultiByte(CP_UTF8,0,str.c_str(),-1,pElementText.get(),iTextLen,NULL,NULL);
    _out_str=string(pElementText.get());
    return true;
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
