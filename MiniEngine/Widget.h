#pragma once
#include "../config.h"
#include <string>

#define _MINI_ENGINE_IMPL_COPY_DECL(ClassName) _SDL_ENGINE_IMPL_COPY_DECL(ClassName)
#define _MINI_ENGINE_IMPL _SDL_ENGINE_IMPL
namespace MiniEngine
{

class Stage;

class Brush
{
public:
    ~Brush();
    _MINI_ENGINE_IMPL_COPY_DECL(Brush)

    void copy(Engine::Texture t,Engine::Rect src,Engine::Rect dst,bool autoZoom);
    void copyFill(Engine::Texture t,Engine::Rect src,bool autoZoom);
    void copyTo(Engine::Texture t,Engine::Rect dst,bool autoZoom);
    void copyFullFill(Engine::Texture t);
public:
    Brush(const Engine::Window& rnd,Engine::Rect DrawableArea);
private:
    _MINI_ENGINE_IMPL
    friend class Stage;
};

class Stage
{
public:
    Stage();
    ~Stage();
};

class Drawable
{
public:
    virtual int Draw(const Brush& brush)=0;
    virtual Engine::Rect getSize()=0;
};

class BaseButton
{
public:
    BaseButton();
    BaseButton(const BaseButton&);
    BaseButton& operator = (const BaseButton&);
    BaseButton(BaseButton&&);
    BaseButton& operator = (BaseButton&&);

    virtual int Draw(const Brush& brush);

    virtual ~BaseButton();
private:
    struct impl;
    impl* pimpl;
};

class SimpleButton : public BaseButton
{
public:
    /// Default Style
    SimpleButton();
    SimpleButton(std::string word,
                 Engine::RGBA color_word,
                 Engine::RGBA color_background,
                 Engine::RGBA color_highlight);
    SimpleButton(const SimpleButton&);
    SimpleButton& operator = (const SimpleButton&);
    SimpleButton(SimpleButton&&);
    SimpleButton& operator = (SimpleButton&&);
    ~SimpleButton();
};

}/// End of namespace MiniEngine.
