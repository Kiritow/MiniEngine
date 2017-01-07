Window::Window(int winw,int winh)
{
    pimpl=new impl;
    SDL_Window* wnd=SDL_CreateWindow("Engine",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,winw,winh,SDL_WINDOW_SHOWN);
    pimpl->set(wnd);
}

Window::Window(const Window& inc)
{
    pimpl=new impl;
    *pimpl=*(inc.pimpl);
}

Window& Window::operator = (const Window& inc)
{
    *pimpl=*(inc.pimpl);
    return *this;
}

Window::Window(Window&& inc)
{
    pimpl=inc.pimpl;
    inc.pimpl=nullptr;
}

Window& Window::operator = (Window&& inc)
{
    *pimpl=*(inc.pimpl);
    inc.pimpl=nullptr;
    return *this;
}


Window::~Window()
{
    delete pimpl;
}

Renderer Window::getRenderer()
{
    return pimpl->rnd;
}

Rect Window::getSize()
{
    int w,h;
    SDL_GetWindowSize(pimpl->getRawWindow(),&w,&h);
    Rect rect(0,0,w,h);
    return rect;
}

void Window::setSize(Rect rect)
{
    SDL_SetWindowSize(pimpl->getRawWindow(),rect.w,rect.h);
}
