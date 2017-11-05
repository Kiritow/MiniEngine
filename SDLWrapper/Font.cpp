#include "Font.h"
#include "_caster.h"
namespace MiniEngine
{
// private
void Font::_set(TTF_Font* p)
{
    _font.reset(p,TTF_CloseFont);
}
// private
void Font::_clear()
{
    _font.reset();
}
// private
TTF_Font* Font::_get() const
{
    return const_cast<TTF_Font*>(_font.get());
}

Font::Font(const std::string& FontFileName, size_t size)
{
    if (use(FontFileName, size) != 0)
    {
        ErrorViewer e;
        e.fetch();
        throw e;
    }
}

int Font::use(const std::string& FontFileName, size_t size)
{
    TTF_Font* temp = TTF_OpenFont(FontFileName.c_str(), size);
    if (temp == NULL) return -1;
    _set(temp);
    return 0;
}

bool Font::isReady() const
{
    return (_get() != nullptr);
}

bool Font::isNormal() const
{
    return !(TTF_GetFontStyle(_get()));
}

bool Font::isBold() const
{
    return (TTF_GetFontStyle(_get()) & TTF_STYLE_BOLD );
}

bool Font::isItalic() const
{
    return (TTF_GetFontStyle(_get()) & TTF_STYLE_ITALIC );
}

bool Font::isUnderLine() const
{
    return (TTF_GetFontStyle(_get()) & TTF_STYLE_UNDERLINE );
}

bool Font::isStrikeThrough() const
{
    return (TTF_GetFontStyle(_get()) & TTF_STYLE_STRIKETHROUGH );
}

void Font::setNormal()
{
    _real_setFontStyle(TTF_STYLE_NORMAL);
}

void Font::setBold(bool enable)
{
    if( enable!=isBold() )
    {
        _real_setFontStyle( TTF_GetFontStyle(_get()) | (enable?TTF_STYLE_BOLD:!TTF_STYLE_BOLD) );
    }
}

void Font::setItalic(bool enable)
{
    if( enable!=isItalic() )
    {
        _real_setFontStyle( TTF_GetFontStyle(_get()) | (enable?TTF_STYLE_ITALIC:!TTF_STYLE_ITALIC) );
    }
}

void Font::setUnderLine(bool enable)
{
    if( enable!=isUnderLine() )
    {
        _real_setFontStyle( TTF_GetFontStyle(_get()) | (enable?TTF_STYLE_UNDERLINE:!TTF_STYLE_UNDERLINE) );
    }
}

void Font::setStrikeThrough(bool enable)
{
    if( enable!=isStrikeThrough() )
    {
        _real_setFontStyle( TTF_GetFontStyle(_get()) | (enable?TTF_STYLE_STRIKETHROUGH:!TTF_STYLE_STRIKETHROUGH) );
    }
}

void Font::_real_setFontStyle(int Style)
{
    TTF_SetFontStyle(_get(),Style);
}

int Font::_style_caster(FontStyle style)
{
    return _internal::getTTFFontStyleFromFontStyle(style);
}

std::vector<FontStyle> Font::getFontStyles() const
{
    int styles=TTF_GetFontStyle(_get());
    return _internal::getFontStyleVecFromMixedTTFFontStyle(styles);
}

int Font::getFontHeight() const
{
    return TTF_FontHeight(_get());
}

int Font::getFontAscent() const
{
    return TTF_FontAscent(_get());
}

int Font::getFontDescent() const
{
    return TTF_FontDescent(_get());
}

int Font::getFontLineSkip() const
{
    return TTF_FontLineSkip(_get());
}

bool Font::isFontKerning() const
{
    return (TTF_GetFontKerning(_get())!=0);
}

void Font::setFontKerning(bool enableKerning)
{
    TTF_SetFontKerning(_get(),enableKerning?1:0);
}

long Font::getFontFaceNum() const
{
    return TTF_FontFaces(_get());
}

int Font::getFontFaceIsFixedWidth() const
{
    return TTF_FontFaceIsFixedWidth(_get());
}

std::string Font::getFontFaceFamilyName() const
{
    return std::string(TTF_FontFaceFamilyName(_get()));
}

std::string Font::getFontFaceStyleName() const
{
    return std::string(TTF_FontFaceStyleName(_get()));
}

FontHint Font::getFontHint() const
{
    switch(TTF_GetFontHinting(_get()))
    {
    case TTF_HINTING_NORMAL:
        return FontHint::Normal;
    case TTF_HINTING_LIGHT:
        return FontHint::Light;
    case TTF_HINTING_MONO:
        return FontHint::Mono;
    case TTF_HINTING_NONE:
        return FontHint::None;
    }
    /// Return Error on default.
    return FontHint::Error;
}

void Font::setFontHint(FontHint hint)
{
    int v=0;
    switch(hint)
    {
    case FontHint::Normal:
        v=TTF_HINTING_NORMAL;
        break;
    case FontHint::Light:
        v=TTF_HINTING_LIGHT;
        break;
    case FontHint::Mono:
        v=TTF_HINTING_MONO;
        break;
    case FontHint::None:
        v=TTF_HINTING_NONE;
        break;
    case FontHint::Error:
        /// No Action on FontHint::Error.
        return;
    }
    TTF_SetFontHinting(_get(),v);
}

Rect Font::sizeText(const std::string& Text) const
{
    int w=0,h=0;
    if(TTF_SizeText(_get(),Text.c_str(),&w,&h)!=0)
    {
        /// Something might be wrong
        ErrorViewer e;
        e.fetch();
        throw e;
    }
    return Rect(0,0,w,h);
}

Rect Font::sizeUTF8(const std::string& Text) const
{
    int w=0,h=0;
    if(TTF_SizeUTF8(_get(),Text.c_str(),&w,&h)!=0)
    {
        /// Something might be wrong
        ErrorViewer e;
        e.fetch();
        throw e;
    }
    return Rect(0,0,w,h);
}

Rect Font::sizeUnicode(const uint16_t* Text) const
{
    int w=0,h=0;
    if(TTF_SizeUNICODE(_get(),Text,&w,&h)!=0)
    {
        /// Something might be wrong
        ErrorViewer e;
        e.fetch();
        throw e;
    }
    return Rect(0,0,w,h);
}

/// rendering surfaces...
Surface Font::renderText(const std::string& Text,const RGBA& fg) const
{
    Surface surf;
    surf._set(TTF_RenderText_Blended(_get(), Text.c_str(), fg.toSDLColor()));
    return surf;
}

Surface Font::renderTextWrapped(const std::string& Text, const RGBA& fg, size_t WrapLength) const
{
    Surface surf;
    surf._set(TTF_RenderText_Blended_Wrapped(_get(), Text.c_str(), fg.toSDLColor(), WrapLength));
    return surf;
}

Surface Font::renderTextShaded(const std::string& Text, const RGBA& fg,const RGBA& bg) const
{
    Surface surf;
    surf._set(TTF_RenderText_Shaded(_get(), Text.c_str(), fg.toSDLColor(), bg.toSDLColor()));
    return surf;
}

Surface Font::renderTextSolid(const std::string& Text,const RGBA& fg) const
{
    Surface surf;
    surf._set(TTF_RenderText_Solid(_get(), Text.c_str(), fg.toSDLColor()));
    return surf;
}

Surface Font::renderUTF8(const std::string& Text,const RGBA& fg) const
{
    Surface surf;
    surf._set(TTF_RenderUTF8_Blended(_get(), Text.c_str(), fg.toSDLColor()));
    return surf;
}

Surface Font::renderUTF8Wrapped(const std::string& Text, const RGBA& fg, size_t WrapLength) const
{
    Surface surf;
    surf._set(TTF_RenderUTF8_Blended_Wrapped(_get(), Text.c_str(), fg.toSDLColor(), WrapLength));
    return surf;
}

Surface Font::renderUTF8Shaded(const std::string& Text, const RGBA& fg,const RGBA& bg) const
{
    Surface surf;
    surf._set(TTF_RenderUTF8_Shaded(_get(), Text.c_str(), fg.toSDLColor(), bg.toSDLColor()));
    return surf;
}

Surface Font::renderUTF8Solid(const std::string& Text,const RGBA& fg) const
{
    Surface surf;
    surf._set(TTF_RenderUTF8_Solid(_get(), Text.c_str(), fg.toSDLColor()));
    return surf;
}

Surface Font::renderUnicode(const uint16_t* Text, const RGBA& fg) const
{
    Surface surf;
    surf._set(TTF_RenderUNICODE_Blended(_get(),Text,fg.toSDLColor()));
    return surf;
}

Surface Font::renderUnicodeWrapped(const uint16_t* Text, const RGBA& fg, size_t WrapLength) const
{
    Surface surf;
    surf._set(TTF_RenderUNICODE_Blended_Wrapped(_get(),Text,fg.toSDLColor(),WrapLength));
    return surf;
}

Surface Font::renderUnicodeShaded(const uint16_t* Text, const RGBA& fg, const RGBA& bg) const
{
    Surface surf;
    surf._set(TTF_RenderUNICODE_Shaded(_get(),Text,fg.toSDLColor(),bg.toSDLColor()));
    return surf;
}

Surface Font::renderUnicodeSolid(const uint16_t* Text, const RGBA& fg) const
{
    Surface surf;
    surf._set(TTF_RenderUNICODE_Solid(_get(),Text,fg.toSDLColor()));
    return surf;
}

/// rendering textures...
Texture Font::renderText(const Renderer& rnd, const std::string& Text, const RGBA& fg) const
{
    return rnd.render(renderText(Text,fg));
}

Texture Font::renderTextWrapped(const Renderer& rnd, const std::string& Text, const RGBA& fg, size_t WrapLength) const
{
    return rnd.render(renderTextWrapped(Text,fg,WrapLength));
}

Texture Font::renderTextShaded(const Renderer& rnd, const std::string& Text, const RGBA& fg, const RGBA& bg) const
{
    return rnd.render(renderTextShaded(Text,fg,bg));
}

Texture Font::renderTextSolid(const Renderer& rnd, const std::string& Text, const RGBA& fg) const
{
    return rnd.render(renderTextSolid(Text,fg));
}

Texture Font::renderUTF8(const Renderer& rnd, const std::string& Text, const RGBA& fg) const
{
    return rnd.render(renderUTF8(Text,fg));
}

Texture Font::renderUTF8Wrapped(const Renderer& rnd, const std::string& Text, const RGBA& fg, size_t WrapLength) const
{
    return rnd.render(renderUTF8Wrapped(Text,fg,WrapLength));
}

Texture Font::renderUTF8Shaded(const Renderer& rnd, const std::string& Text, const RGBA& fg, const RGBA& bg) const
{
    return rnd.render(renderUTF8Shaded(Text,fg,bg));
}

Texture Font::renderUTF8Solid(const Renderer& rnd, const std::string& Text, const RGBA& fg) const
{
    return rnd.render(renderUTF8Solid(Text,fg));
}


Texture Font::renderUnicode(const Renderer& rnd, const uint16_t* Text, const RGBA& fg) const
{
    return rnd.render(renderUnicode(Text,fg));
}

Texture Font::renderUnicodeWrapped(const Renderer& rnd, const uint16_t* Text, const RGBA& fg, size_t WrapLength) const
{
    return rnd.render(renderUnicodeWrapped(Text,fg,WrapLength));
}

Texture Font::renderUnicodeShaded(const Renderer& rnd, const uint16_t* Text, const RGBA& fg, const RGBA& bg) const
{
    return rnd.render(renderUnicodeShaded(Text,fg,bg));
}

Texture Font::renderUnicodeSolid(const Renderer& rnd, const uint16_t* Text, const RGBA& fg) const
{
    return rnd.render(renderUnicodeSolid(Text,fg));
}

void Font::release()
{
    _clear();
}
} /// End of namespace MiniEngine

