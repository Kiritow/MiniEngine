Window::Window(int winw,int winh)
{
    pimpl=new impl;
    pimpl->sWnd.reset(SDL_CreateWindow("Engine",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,winw,winh,SDL_WINDOW_SHOWN),SDL_DestroyWindow);
    pimpl->rnd.pimpl->sRnd.reset(SDL_CreateRenderer(pimpl->sWnd.get(),-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_TARGETTEXTURE),SDL_DestroyRenderer);
}

Window::~Window()
{
    delete pimpl;
}

Renderer Window::getRenderer()
{
    return pimpl->rnd;
}
