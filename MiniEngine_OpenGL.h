#pragma once
#include "MiniEngine_Config.h"
#include "GL/glew.h"
#include <SDL2/SDL_opengl.h>
#include <memory>
#include "SDLWrapper/Window.h"

namespace MiniEngine
{

namespace OpenGL
{

class GLContext
{
public:
    GLContext(Window& wnd) throw (ErrorViewer);
private:
    void _clear();
    void _set(SDL_GLContext);
    SDL_GLContext _get() const;

    /// SDL_GLContext is void*
    std::shared_ptr<void> _sp;
};


}/// End of namespace MiniEngine::OpenGL

}/// End of namespace MiniEngine
