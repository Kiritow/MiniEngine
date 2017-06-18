#include "Texture.h"
#include "begin_code.h"
//private
void Texture::_set(SDL_Texture* p)
{
    _text.reset(p,SDL_DestroyTexture);
    updateInfo();
}

//private
void Texture::_set_no_delete(SDL_Texture* p)
{
    _text.reset(p,[](SDL_Texture*) {});
    updateInfo();
}

//private
void Texture::_clear()
{
    _text.reset();
    updateInfo();
}

//private
SDL_Texture* Texture::_get() const
{
    return _text.get();
}

Texture::Texture()
{
    updateInfo();
}

Rect Texture::getSize()
{
    return rect;
}

int Texture::getw() const
{
    return rect.w;
}

int Texture::geth() const
{
    return rect.h;
}

bool Texture::isReady() const
{
    return (_get() != nullptr);
}

int Texture::setBlendMode(BlendMode mode)
{
    return SDL_SetTextureBlendMode(_get(), _internal::getSDLBlendModeFromBlendMode(mode));
}

BlendMode Texture::getBlendMode() const
{
    SDL_BlendMode temp;
    SDL_GetTextureBlendMode(_get(), &temp);
    return _internal::getBlendModeFromSDLBlendMode(temp);
}

/// Alpha:  0: Transparent 255: opaque

int Texture::setAlphaMode(int alpha)
{
    Uint8 temp = std::max(std::min(alpha, 255), 0);
    return SDL_SetTextureAlphaMod(_get(), temp);
}

int Texture::getAlphaMode() const
{
    Uint8 temp;
    SDL_GetTextureAlphaMod(_get(), &temp);
    return temp;
}

ColorMode Texture::getColorMode() const
{
    ColorMode pack;
    Uint8 r, g, b;
    SDL_GetTextureColorMod(_get(), &r, &g, &b);
    pack.r = r;
    pack.g = g;
    pack.b = b;
    return pack;
}

int Texture::setColorMode(ColorMode mode)
{
    return SDL_SetTextureColorMod(_get(), mode.r, mode.g, mode.b);
}

RGBA Texture::getRGBA() const
{
    return RGBA(getColorMode(), getAlphaMode());
}

void Texture::setRGBA(const RGBA& pack)
{
    setColorMode(pack.toColorMode());
    setAlphaMode(pack.a);
}

/// updateInfo() must be called after Texture is changed.
//protected
void Texture::updateInfo()
{
    if(_get()==nullptr)
    {
        rect.x=rect.y=rect.w=rect.h=0;
    }
    SDL_QueryTexture(_get(), NULL, NULL, &rect.w, &rect.h);
    rect.x = rect.y = 0;
}

void Texture::release()
{
    _clear();
}
#include "end_code.h"
