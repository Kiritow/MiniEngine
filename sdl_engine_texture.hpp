Texture::Texture()
{
    pimpl=new impl;
}
Texture::Texture(const Texture& inc) : Texture()
{
    *pimpl=*(inc.pimpl);
}
int Texture::getw()
{
    return pimpl->w;
}
int Texture::geth()
{
    return pimpl->h;
}
Texture::~Texture()
{
    delete pimpl;
}
int Texture::updateSize()
{
    return SDL_QueryTexture(pimpl->sText.get(),NULL,NULL,&pimpl->w,&pimpl->h);
}
