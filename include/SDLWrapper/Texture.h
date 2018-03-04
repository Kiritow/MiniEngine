#pragma once
#include "include.h"
#include "Rect.h"
#include "RGBA.h"
#include "ColorMode.h"
#include "BlendMode.h"
#include "__Plugin.h"
#include <memory>

namespace MiniEngine {

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
    Rect _rect;
    friend class Renderer;

    friend class _internal::Plugin;
};

} /// End of namespace MiniEngine
