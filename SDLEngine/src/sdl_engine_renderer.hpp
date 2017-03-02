Renderer::Renderer()
{
    pimpl=new impl;
}
Renderer::Renderer(const Renderer& inc) : Renderer()
{
    *pimpl=*(inc.pimpl);
}
Renderer& Renderer::operator = (const Renderer& inc)
{
    *pimpl=*(inc.pimpl);
    return *this;
}
Renderer::Renderer(Renderer&& inc)
{
    pimpl=inc.pimpl;
    inc.pimpl=nullptr;
}
Renderer& Renderer::operator = (Renderer&& inc)
{
    *pimpl=*(inc.pimpl);
    inc.pimpl=nullptr;
    return *this;
}

Renderer::~Renderer()
{
    delete pimpl;
}

int Renderer::clear()
{
    return SDL_RenderClear(pimpl->sRnd.get());
}
Texture Renderer::loadImage(const char* FileName)
{
    Texture t;
    t.pimpl->set(IMG_LoadTexture(pimpl->sRnd.get(),FileName));
    return t;
}
Texture Renderer::render(Surface surface)
{
    Texture t;
    t.pimpl->set(SDL_CreateTextureFromSurface(pimpl->sRnd.get(),surface.pimpl->getRawSurface()));
    return t;
}

void Renderer::update()
{
    SDL_RenderPresent(pimpl->sRnd.get());
}

int Renderer::copy(Texture t,Rect src,Rect dst)
{
    SDL_Rect s=src.toSDLRect();
    SDL_Rect d=dst.toSDLRect();
    return SDL_RenderCopy(pimpl->sRnd.get(),t.pimpl->getRawTexture(),&s,&d);
}
int Renderer::copyTo(Texture t,Rect dst)
{
    SDL_Rect d=dst.toSDLRect();
    return SDL_RenderCopy(pimpl->sRnd.get(),t.pimpl->getRawTexture(),NULL,&d);
}
int Renderer::copyFill(Texture t,Rect src)
{
    SDL_Rect s=src.toSDLRect();
    return SDL_RenderCopy(pimpl->sRnd.get(),t.pimpl->getRawTexture(),&s,NULL);
}
int Renderer::copyFullFill(Texture t)
{
    return SDL_RenderCopy(pimpl->sRnd.get(),t.pimpl->getRawTexture(),NULL,NULL);
}

int Renderer::setColor(RGBA pack)
{
    return SDL_SetRenderDrawColor(pimpl->sRnd.get(),pack.r,pack.g,pack.b,pack.a);
}

RGBA Renderer::getColor(int* pstatus)
{
    Uint8 r,g,b,a;
    int ret=SDL_GetRenderDrawColor(pimpl->sRnd.get(),&r,&g,&b,&a);
    RGBA pack(r,g,b,a);
    if(pstatus) *pstatus=ret;
    return pack;
}

int Renderer::fillRect(Rect rect)
{
    auto inr=rect.toSDLRect();
    return SDL_RenderFillRect(pimpl->sRnd.get(),&inr);
}

int Renderer::drawRect(Rect rect)
{
    auto inr=rect.toSDLRect();
    return SDL_RenderDrawRect(pimpl->sRnd.get(),&inr);
}
