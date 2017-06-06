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

/// Compare two surfaces. Currently, Surface::getRawPointer() does not has constant attribute.
int CompareSurface(Surface& surface1, Surface& surface2, int allowableError)
{
    return SDLTest_CompareSurfaces(surface1.getRawPointer(),surface2.getRawPointer(),allowableError);
}

//private
struct UniRandom::_impl
{
    SDLTest_RandomContext context;
};

UniRandom::UniRandom()
{
    _sp.reset(new _impl);
    SDLTest_RandomInitTime(&(_sp.get()->context));
}

UniRandom::UniRandom(unsigned int A, unsigned int B)
{
    _sp.reset(new _impl);
    SDLTest_RandomInit(&(_sp.get()->context),A,B);
}

uint32_t UniRandom::get()
{
    return SDLTest_Random(&(_sp.get()->context));
}


}/// End of namespace MiniEngine::Test

}/// End of namespace MiniEngine
