#include "SDLWrapper/ErrorViewer.h"
#include "SDLWrapper/include.h"
namespace MiniEngine {
void ErrorViewer::fetch()
{
    str = SDL_GetError();
}

std::string ErrorViewer::getError() const
{
    return str;
}

const char * ErrorViewer::what() const noexcept
{
    return str.c_str();
}
} /// End of namespace MiniEngine
