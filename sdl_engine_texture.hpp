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
