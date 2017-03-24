#include "MiniEngine.h"
using namespace std;

#include <windows.h>

//GBK编码转换到UTF8编码
int _GBKToUTF8(unsigned char * lpGBKStr,unsigned char * lpUTF8Str,int nUTF8StrLen)
{
    wchar_t * lpUnicodeStr = NULL;
    int nRetLen = 0;

    if(!lpGBKStr)  //如果GBK字符串为NULL则出错退出
        return 0;

    nRetLen = MultiByteToWideChar(CP_ACP,0,(char *)lpGBKStr,-1,NULL,0);  //获取转换到Unicode编码后所需要的字符空间长度
    lpUnicodeStr = new WCHAR[nRetLen + 1];  //为Unicode字符串空间
    nRetLen = ::MultiByteToWideChar(CP_ACP,0,(char *)lpGBKStr,-1,lpUnicodeStr,nRetLen);  //转换到Unicode编码
    if(!nRetLen)  //转换失败则出错退出
        return 0;

    nRetLen = ::WideCharToMultiByte(CP_UTF8,0,lpUnicodeStr,-1,NULL,0,NULL,0);  //获取转换到UTF8编码后所需要的字符空间长度

    if(!lpUTF8Str)  //输出缓冲区为空则返回转换后需要的空间大小
    {
        if(lpUnicodeStr)
            delete []lpUnicodeStr;
        return nRetLen;
    }

    if(nUTF8StrLen < nRetLen)  //如果输出缓冲区长度不够则退出
    {
        if(lpUnicodeStr)
            delete []lpUnicodeStr;
        return 0;
    }

    nRetLen = ::WideCharToMultiByte(CP_UTF8,0,lpUnicodeStr,-1,(char *)lpUTF8Str,nUTF8StrLen,NULL,NULL);  //转换到UTF8编码

    if(lpUnicodeStr)
        delete []lpUnicodeStr;

    return nRetLen;
}

// UTF8编码转换到GBK编码
int _UTF8ToGBK(unsigned char * lpUTF8Str,unsigned char * lpGBKStr,int nGBKStrLen)
{
    wchar_t * lpUnicodeStr = NULL;
    int nRetLen = 0;

    if(!lpUTF8Str)  //如果UTF8字符串为NULL则出错退出
        return 0;

    nRetLen = ::MultiByteToWideChar(CP_UTF8,0,(char *)lpUTF8Str,-1,NULL,0);  //获取转换到Unicode编码后所需要的字符空间长度
    lpUnicodeStr = new WCHAR[nRetLen + 1];  //为Unicode字符串空间
    nRetLen = ::MultiByteToWideChar(CP_UTF8,0,(char *)lpUTF8Str,-1,lpUnicodeStr,nRetLen);  //转换到Unicode编码
    if(!nRetLen)  //转换失败则出错退出
        return 0;

    nRetLen = ::WideCharToMultiByte(CP_ACP,0,lpUnicodeStr,-1,NULL,0,NULL,NULL);  //获取转换到GBK编码后所需要的字符空间长度

    if(!lpGBKStr)  //输出缓冲区为空则返回转换后需要的空间大小
    {
        if(lpUnicodeStr)
            delete []lpUnicodeStr;
        return nRetLen;
    }

    if(nGBKStrLen < nRetLen)  //如果输出缓冲区长度不够则退出
    {
        if(lpUnicodeStr)
            delete []lpUnicodeStr;
        return 0;
    }

    nRetLen = ::WideCharToMultiByte(CP_ACP,0,lpUnicodeStr,-1,(char *)lpGBKStr,nRetLen,NULL,NULL);  //转换到GBK编码

    if(lpUnicodeStr)
        delete []lpUnicodeStr;

    return nRetLen;
}

string UTF8ToGBK(string UTF8String)
{
    int sz=UTF8String.size()+32;
    auto utf8str=new unsigned char[sz];
    memset(utf8str,0,sz);
    memcpy(utf8str,UTF8String.c_str(),UTF8String.size());

    auto gbkstr=new unsigned char[sz];
    memset(gbkstr,0,sz);

    _UTF8ToGBK(utf8str,gbkstr,sz);

    string s((char*)gbkstr);

    delete[] gbkstr;
    delete[] utf8str;
    return s;
}

string GBKToUTF8(string GBKString)
{
    int sz=GBKString.size()+32;
    auto gbkstr=new unsigned char[sz];
    memset(gbkstr,0,sz);
    memcpy(gbkstr,GBKString.c_str(),GBKString.size());

    auto utf8str=new unsigned char[sz];
    memset(utf8str,0,sz);

    _GBKToUTF8(gbkstr,utf8str,sz);

    string s((char*)utf8str);

    delete[] gbkstr;
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
