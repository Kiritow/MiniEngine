#pragma once
#include <string>

namespace MiniEngine
{

namespace Test
{

std::string GetMD5(unsigned char* buffer,unsigned int bufferLen);
void GetMD5Raw(unsigned char* buffer,unsigned int bufferLen,unsigned char* outbuff);

}/// End of namespace MiniEngine::Test

}/// End of namespace MiniEngine
