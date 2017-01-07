Window::Window(int winw,int winh)
{
    pimpl=new impl;
    SDL_Window* wnd=SDL_CreateWindow("Engine",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,winw,winh,SDL_WINDOW_SHOWN);
    pimpl->set(wnd);
}

Window::~Window()
{
    delete pimpl;
}

Renderer Window::getRenderer()
{
    return pimpl->rnd;
}

Window::Window(const Window& inc) : Window(DEFAULT_WIDTH,DEFAULT_HEIGHT)
{
    *pimpl=*(inc.pimpl);
}
