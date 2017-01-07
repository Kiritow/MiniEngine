Texture::Texture()
{
    pimpl=new impl;
}
Texture::Texture(const Texture& inc) : Texture()
{
    *pimpl=*(inc.pimpl);
}
Texture& Texture::operator = (const Texture& inc)
{
    *pimpl=*(inc.pimpl);
    return *this;
}
Texture::Texture(Texture&& inc)
{
    pimpl=inc.pimpl;
    inc.pimpl=nullptr;
}
Texture& Texture::operator = (Texture&& inc)
{
    *(pimpl)=*(inc.pimpl);
    inc.pimpl=nullptr;
    return *this;
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
