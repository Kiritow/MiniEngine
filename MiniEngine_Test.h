#pragma once
#include "MiniEngine.h"
#include <string>

namespace MiniEngine
{

namespace Test
{

std::string GetMD5(unsigned char* buffer,unsigned int bufferLen);
void GetMD5Raw(unsigned char* buffer,unsigned int bufferLen,unsigned char* outbuff);
int CompareSurface(const Surface& surface1,const Surface& surface2,int allowableError);

class UniRandom
{
public:
    /// Default Constructor is based on system current time.
    UniRandom();
    UniRandom(unsigned int A,unsigned int B);
    uint32_t get();
private:
    struct _impl;
    std::shared_ptr<_impl> _sp;
};

}/// End of namespace MiniEngine::Test

}/// End of namespace MiniEngine
