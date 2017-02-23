Font::Font()
{
    pimpl=new impl;
}

Font::Font(const char* FontFileName,int sz) : Font()
{
    use(FontFileName,sz);
}

Font::Font(const Font& inc) : Font()
{
    *pimpl=*(inc.pimpl);
}
Font& Font::operator = (const Font& inc)
{
    *pimpl=*(inc.pimpl);
    return *this;
}
Font::Font(Font&& inc)
{
    pimpl=inc.pimpl;
    inc.pimpl=nullptr;
}
Font& Font::operator = (Font&& inc)
{
    *pimpl=*(inc.pimpl);
    inc.pimpl=nullptr;
    return *this;
}

Font::~Font()
{
    delete pimpl;
}

int Font::use(const char* FontFileName,int sz)
{
    TTF_Font* font=TTF_OpenFont(FontFileName,sz);
    if(font==NULL) return -1;
    pimpl->sTTF.reset(font,TTF_CloseFont);
    return 0;
}



Texture Font::renderText(Renderer rnd,const char* Word,RGBA fg)
{
    Surface surf;
    surf.pimpl->set(TTF_RenderText_Blended(pimpl->sTTF.get(),Word,fg.toSDLColor()));
    Texture t=rnd.render(surf);
    return t;
}

Texture Font::renderUTF8(Renderer rnd,const char* Word,RGBA fg)
{
    Surface surf;
    surf.pimpl->set(TTF_RenderUTF8_Blended(pimpl->sTTF.get(),Word,fg.toSDLColor()));
    Texture t=rnd.render(surf);
    return t;
}
