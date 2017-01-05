Renderer::Renderer()
{
    pimpl=new impl;
}
Renderer::Renderer(const Renderer& inc) : Renderer()
{
    *pimpl=*(inc.pimpl);
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
    t.pimpl->sText.reset(IMG_LoadTexture(pimpl->sRnd.get(),FileName),SDL_DestroyTexture);
    t.updateSize();
    return t;
}
Texture Renderer::render(Surface surface)
{
    Texture t;
    t.pimpl->sText.reset(SDL_CreateTextureFromSurface(pimpl->sRnd.get(),surface.pimpl->sSurf.get()),SDL_DestroyTexture);
    t.updateSize();
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
    return SDL_RenderCopy(pimpl->sRnd.get(),t.pimpl->sText.get(),&s,&d);
}
int Renderer::copyTo(Texture t,Rect dst)
{
    SDL_Rect d=dst.toSDLRect();
    return SDL_RenderCopy(pimpl->sRnd.get(),t.pimpl->sText.get(),NULL,&d);
}
int Renderer::copyFill(Texture t,Rect src)
{
    SDL_Rect s=src.toSDLRect();
    return SDL_RenderCopy(pimpl->sRnd.get(),t.pimpl->sText.get(),&s,NULL);
}
int Renderer::copyFullFill(Texture t)
{
    return SDL_RenderCopy(pimpl->sRnd.get(),t.pimpl->sText.get(),NULL,NULL);
}
