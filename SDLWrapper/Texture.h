#pragma once
#include "include.h"
#include "begin_code.h"

class Texture
{
public:
    Texture();
    ~Texture() = default;
    Rect getSize();
    int getw() const;
    int geth() const;
    bool isReady() const;
    int setBlendMode(BlendMode mode);
    BlendMode getBlendMode() const;
    /// Alpha:  0: Transparent 255: opaque
    int setAlphaMode(int alpha);
    int getAlphaMode() const;

    ColorMode getColorMode() const;
    int setColorMode(ColorMode mode);
    RGBA getRGBA() const;
    void setRGBA(const RGBA& pack);

    void release();
protected:
    /// updateInfo() must be called after Texture is changed.
    void updateInfo();
private:
    std::shared_ptr<SDL_Texture> _text;
    void _set(SDL_Texture*);
    /// Just used for "SDL_GetRenderTarget"
    void _set_no_delete(SDL_Texture*);
    void _clear();
    SDL_Texture* _get() const;
    Rect rect;
    friend class Renderer;
};

#include "end_code.h"
