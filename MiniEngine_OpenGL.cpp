#include "MiniEngine_OpenGL.h"

namespace MiniEngine
{

namespace OpenGL
{

//private
void GLContext::_clear()
{
    _sp.reset();
}
//private
void GLContext::_set(SDL_GLContext Context)
{
    _sp.reset(Context,SDL_GL_DeleteContext);
}
//private
SDL_GLContext GLContext::_get() const
{
    return _sp.get();
}

GLContext::GLContext(Window& wnd) throw (ErrorViewer)
{
    SDL_GLContext context=SDL_GL_CreateContext(_internal::Plugin::get(wnd));
    if(!context)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
    _set(context);
}

}/// End of namespace MiniEngine::OpenGL

}/// End of namespace MiniEngine
