#include "ErrorViewer.h"
#include "include.h"
#include "begin_code.h"
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
#include "end_code.h"
