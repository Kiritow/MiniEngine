#pragma once
#include "basic_config.h"

SDL_Texture* RenderText(SDL_Renderer* rnd,TTF_Font* font,const char* Text,SDL_Color color,int* pw,int* ph);
SDL_Texture* RenderUTF8(SDL_Renderer* rnd,TTF_Font* font,const char* Text,SDL_Color color,int* pw,int* ph);
void TextureDraw(SDL_Renderer* rnd,SDL_Texture* texture,int dstx,int dsty);
bool isInRect(int x,int y,SDL_Rect rect);
bool isInRect(int x,int y,int LU_x,int LU_y,int RD_x,int RD_y);
void ClearMessageQueue();

int MyChangeDir(const char* DirName);

namespace Engine
{
class Rect
{
public:
    Rect();
    Rect(int incx,int incy,int incw,int inch);
    SDL_Rect toSDLRect();
    ~Rect();

    int x,y,w,h;
};

class Renderer;
class Texture;
class Font;

#define _SDL_ENGINE_IMPL_COPY_DECL(ClassName) \
    ClassName(const ClassName&); \
    ClassName(ClassName&&); \
    ClassName& operator = (const ClassName&);

#define _SDL_ENGINE_IMPL \
    struct impl; \
    impl* pimpl;

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768

class Window
{
public:
    Window(int winw,int winh);
    ~Window();

    _SDL_ENGINE_IMPL_COPY_DECL(Window);

    Renderer getRenderer();
    void resetRenderer();

    Rect getSize();
    void setSize(Rect r);

private:
    _SDL_ENGINE_IMPL
};

class Surface
{
public:
    ~Surface();
    _SDL_ENGINE_IMPL_COPY_DECL(Surface);
protected:
    Surface();
private:
    _SDL_ENGINE_IMPL
    friend class Renderer;
    friend class Font;
};

class Renderer
{
public:
    ~Renderer();
    _SDL_ENGINE_IMPL_COPY_DECL(Renderer);

    int clear();
    void update();
    int copy(Texture t,Rect src,Rect dst);
    int copyTo(Texture t,Rect dst);
    int copyFill(Texture t,Rect src);
    int copyFullFill(Texture t);
    Texture loadImage(const char* FileName);
    Texture render(Surface surface);

protected:
    Renderer();
private:
    _SDL_ENGINE_IMPL
    friend class Window;
};

class Texture
{
public:
    ~Texture();
    int getw();
    int geth();
    _SDL_ENGINE_IMPL_COPY_DECL(Texture);
protected:
    Texture();
private:
    _SDL_ENGINE_IMPL
    friend class Renderer;
};

class RGBA
{
public:
    RGBA();
    RGBA(int incr,int incg,int incb,int inca);
    ~RGBA();

    SDL_Color toSDLColor();

    int r,g,b,a;
};

class Font
{
public:
    Font();
    Font(const char* FontFileName,int sz);
    int use(const char* FontFileName,int sz);
    ~Font();

    _SDL_ENGINE_IMPL_COPY_DECL(Font);

    Texture renderText(Renderer rnd,const char* Word,RGBA fg);
    Texture renderTextShaded(Renderer rnd,const char* Word,RGBA fg,RGBA bg);
    Texture renderTextSolid(Renderer rnd,const char* Word,RGBA fg);

    Texture renderUTF8(Renderer rnd,const char* Word,RGBA fg);
    Texture renderUTF8Shaded(Renderer rnd,const char* Word,RGBA fg,RGBA bg);
    Texture renderUTF8Solid(Renderer rnd,const char* Word,RGBA fg);
private:
    _SDL_ENGINE_IMPL
};

}/// End of namespace Engine
