#pragma once
#include "include.h"
#include "_FontStyle.h"
#include "_FontHint.h"
#include "Rect.h"
#include "Surface.h"
#include "Texture.h"
#include "Renderer.h"
#include <vector>
#include "__Plugin.h"
namespace MiniEngine
{
class Font
{
public:
    Font() = default;
    Font(const std::string& FontFileName, size_t size);
    int use(const std::string& FontFileName, size_t size);
    bool isReady() const;

    bool isNormal() const;
    bool isBold() const;
    bool isItalic() const;
    bool isUnderLine() const;
    bool isStrikeThrough() const;

    void setNormal();
    void setBold(bool);
    void setItalic(bool);
    void setUnderLine(bool);
    void setStrikeThrough(bool);

    template<typename... Args>
    void setFontStyle(FontStyle style,Args&&... args)
    {
        int fontcalc=0;
        _setFontStyle(fontcalc,style,args...);
    }

    void setFontStyle(FontStyle style)
    {
        int fontcalc=0;
        _setFontStyle(fontcalc,style);
    }

    std::vector<FontStyle> getFontStyles() const;

    int getFontHeight() const;
    int getFontAscent() const;
    int getFontDescent() const;
    int getFontLineSkip() const;

    bool isFontKerning() const;
    void setFontKerning(bool enableKerning);

    long getFontFaceNum() const;
    int getFontFaceIsFixedWidth() const;
    std::string getFontFaceFamilyName() const;
    std::string getFontFaceStyleName() const;

    FontHint getFontHint() const;
    void setFontHint(FontHint hint);


    Rect sizeText(const std::string& Text) const;
    Rect sizeUTF8(const std::string& Text) const;
    Rect sizeUnicode(const uint16_t* Text) const;

    /// Surface Rendering Functions.
    Surface renderText(const std::string& Text, const RGBA& fg) const;
    Surface renderTextWrapped(const std::string& Text, const RGBA& fg, size_t WrapLength) const;
    Surface renderTextShaded(const std::string& Text, const RGBA& fg, const RGBA& bg) const;
    Surface renderTextSolid(const std::string& Text, const RGBA& fg) const;

    Surface renderUTF8(const std::string& Text, const RGBA& fg) const;
    Surface renderUTF8Wrapped(const std::string& Text, const RGBA& fg, size_t WrapLength) const;
    Surface renderUTF8Shaded(const std::string& Text, const RGBA& fg, const RGBA& bg) const;
    Surface renderUTF8Solid(const std::string& Text, const RGBA& fg) const;

    Surface renderUnicode(const uint16_t* Text,const RGBA& fg) const;
    Surface renderUnicodeWrapped(const uint16_t* Text,const RGBA& fg,size_t WrapLength) const;
    Surface renderUnicodeShaded(const uint16_t* Text,const RGBA& fg,const RGBA& bg) const;
    Surface renderUnicodeSolid(const uint16_t* Text,const RGBA& fg) const;

    /// Texture Rendering Functions.
    Texture renderText(const Renderer& rnd, const std::string& Text, const RGBA& fg) const;
    Texture renderTextWrapped(const Renderer& rnd, const std::string& Text, const RGBA& fg, size_t WrapLength) const;
    Texture renderTextShaded(const Renderer& rnd, const std::string& Text, const RGBA& fg, const RGBA& bg) const;
    Texture renderTextSolid(const Renderer& rnd, const std::string& Text, const RGBA& fg) const;

    Texture renderUTF8(const Renderer& rnd, const std::string& Text, const RGBA& fg) const;
    Texture renderUTF8Wrapped(const Renderer& rnd, const std::string& Text, const RGBA& fg, size_t WrapLength) const;
    Texture renderUTF8Shaded(const Renderer& rnd, const std::string& Text, const RGBA& fg, const RGBA& bg) const;
    Texture renderUTF8Solid(const Renderer& rnd, const std::string& Text, const RGBA& fg) const;

    Texture renderUnicode(const Renderer& rnd,const uint16_t* Text,const RGBA& fg) const;
    Texture renderUnicodeWrapped(const Renderer& rnd,const uint16_t* Text,const RGBA& fg,size_t WrapLength) const;
    Texture renderUnicodeShaded(const Renderer& rnd,const uint16_t* Text,const RGBA& fg,const RGBA& bg) const;
    Texture renderUnicodeSolid(const Renderer& rnd,const uint16_t* Text,const RGBA& fg) const;

    void release();
protected:
    template<typename... Args>
    void _setFontStyle(int& fontcalc,FontStyle style,Args&&... args)
    {
        fontcalc|=_style_caster(style);
        _setFontStyle(fontcalc,args...);
    }

    void _setFontStyle(int& fontcalc,FontStyle style)
    {
        fontcalc|=_style_caster(style);
        _real_setFontStyle(fontcalc);
    }
private:
    void _real_setFontStyle(int);
    int _style_caster(FontStyle);

    std::shared_ptr<TTF_Font> _font;
    void _set(TTF_Font*);
    void _clear();
    TTF_Font* _get() const;

    friend class _internal::Plugin;
};
} /// End of namespace MiniEngine 

