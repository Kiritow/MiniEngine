#pragma once
#include "basic_config.h"

SDL_Texture* RenderText(SDL_Renderer* rnd,TTF_Font* font,const char* Text,SDL_Color color,int* pw,int* ph);
SDL_Texture* RenderUTF8(SDL_Renderer* rnd,TTF_Font* font,const char* Text,SDL_Color color,int* pw,int* ph);
void TextureDraw(SDL_Renderer* rnd,SDL_Texture* texture,int dstx,int dsty);
bool isInRect(int x,int y,SDL_Rect rect);
bool isInRect(int x,int y,int LU_x,int LU_y,int RD_x,int RD_y);
void ClearMessageQueue();

int MyChangeDir(const char* DirName);

extern SDL_Color color_white;
extern SDL_Color color_black;

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

class Window
{
public:
    Window(int winw,int winh);
    ~Window();
    Renderer getRenderer();
    Rect getSize();
    void setSize(Rect r);
private:
    struct impl;
    impl* pimpl;
};

class Surface
{
public:
    ~Surface();
protected:
    Surface();
private:
    struct impl;
    impl* pimpl;
    friend class Renderer;
};

class Renderer
{
public:
    ~Renderer();
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
    struct impl;
    impl* pimpl;
    friend class Window;
};

class Texture
{
public:
    ~Texture();
    int getw();
    int geth();
protected:
    Texture();
    int updateSize();
private:
    struct impl;
    impl* pimpl;
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
    ~Font();

    int use(const char* FontFileName,int sz);

    Texture renderText(Renderer rnd,const char* Word,RGBA fg);
    Texture renderTextShaded(Renderer rnd,const char* Word,RGBA fg,RGBA bg);
    Texture renderTextSolid(Renderer rnd,const char* Word,RGBA fg);

    Texture renderUTF8(Renderer rnd,const char* Word,RGBA fg);
    Texture renderUTF8Shaded(Renderer rnd,const char* Word,RGBA fg,RGBA bg);
    Texture renderUTF8Solid(Renderer rnd,const char* Word,RGBA fg);

private:
    struct impl;
    impl* pimpl;
};

}/// End of namespace Engine
