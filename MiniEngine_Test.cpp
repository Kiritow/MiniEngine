#include "MiniEngine_Test.h"
#include <SDL2/SDL_test.h>
#include <cstring>

namespace MiniEngine
{

namespace Test
{

void GetMD5Raw(unsigned char* buffer,unsigned int bufferLen,unsigned char* outbuff)
{
    SDLTest_Md5Context ct;
    SDLTest_Md5Init(&ct);
    SDLTest_Md5Update(&ct,buffer,bufferLen);
    SDLTest_Md5Final(&ct);
    memcpy(outbuff,ct.digest,16);
}

std::string GetMD5(unsigned char* buffer,unsigned int bufferLen)
{
    unsigned char buff[16];
    char tmp[8];
    GetMD5Raw(buffer,bufferLen,buff);
    std::string str;
    for(int i=0;i<16;i++)
    {
        sprintf(tmp,"%02x",buff[i]);
        str.append(tmp);
    }
    return str;
}


}/// End of namespace MiniEngine::Test

}/// End of namespace MiniEngine
