#include <SDL2/SDL.h>
#undef main
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <string>
#include <memory>

#define _DECL_DEPRECATED __declspec(deprecated)
#define _DECL_DEPRECATED_MSG(InfoString) __declspec(deprecated(InfoString))

namespace MiniEngine
{

class Rect
{
public:
    int x, y, w, h;
    Rect(int X, int Y, int W, int H)
    {
        x = X;
        y = Y;
        w = W;
        h = H;
    }
    Rect()
    {
        x = y = w = h = 0;
    }
    SDL_Rect toSDLRect()
    {
        SDL_Rect r;
        r.x = x;
        r.y = y;
        r.w = w;
        r.h = h;
        return r;
    }
};

class Point
{
public:
    int x,y;
    Point(int X,int Y)
    {
        x=X;
        y=Y;
    }
    Point()
    {
        x=y=0;
    }
    SDL_Point toSDLPoint()
    {
        SDL_Point p;
        p.x=x;
        p.y=y;
        return p;
    }
    bool inRect(Rect rect)
    {
        auto p=toSDLPoint();
        auto r=rect.toSDLRect();
        return SDL_PointInRect(&p,&r);
    }
};

class ColorMode
{
public:
    int r,g,b;
    ColorMode(int R,int G,int B)
    {
        r=R;
        g=G;
        b=B;
    }
    ColorMode()
    {
        r=g=b=0;
    }
};

class RGBA
{
public:
    int r,g,b,a;
    RGBA(int R, int G, int B, int A)
    {
        r=R;
        g=G;
        b=B;
        a = A;
    }
    RGBA(ColorMode mode,int A)
    {
        r=mode.r;
        g=mode.g;
        b=mode.b;
        a=A;
    }
    RGBA()
    {
        r=g=b=a=0;
    }
    SDL_Color toSDLColor()
    {
        SDL_Color c;
        c.r = r;
        c.g = g;
        c.b = b;
        c.a = a;
        return c;
    }
    ColorMode toColorMode()
    {
        return ColorMode(r,g,b);
    }
};

class NonCopyable
{
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator = (const NonCopyable&) = delete;
};

class ErrorViewer : public std::exception
{
public:
    void fetch()
    {
        str = SDL_GetError();
    }
    std::string getError()
    {
        return str;
    }
    const char* what() const throw() override
    {
        return str.c_str();
    }
private:
    std::string str;
};

enum class BlendMode
{
    None=SDL_BLENDMODE_NONE,
    Blend=SDL_BLENDMODE_BLEND,
    Add=SDL_BLENDMODE_ADD,
    Mod=SDL_BLENDMODE_MOD
};

class Surface
{
public:
    ~Surface()=default;
protected:
    Surface() = default;
private:
    std::shared_ptr<SDL_Surface> surf;
    friend class Window;
    friend class Renderer;
    friend class Font;
};

class Texture
{
public:
    ~Texture()=default;
    Rect getSize()
    {
        return rect;
    }
    int getw()
    {
        return rect.w;
    }
    int geth()
    {
        return rect.h;
    }
    bool isReady()
    {
        return (text.get() != nullptr);
    }
    int setBlendMode(BlendMode mode)
    {
        return SDL_SetTextureBlendMode(text.get(),static_cast<SDL_BlendMode>(mode));
    }
    BlendMode getBlendMode()
    {
        SDL_BlendMode temp;
        SDL_GetTextureBlendMode(text.get(),&temp);
        return static_cast<BlendMode>(temp);
    }
    /// Alpha:  0: Transparent 255: opaque
    int setAlphaMode(int alpha)
    {
        Uint8 temp=std::max(std::min(alpha,255),0);
        return SDL_SetTextureAlphaMod(text.get(),temp);
    }
    int getAlphaMode()
    {
        Uint8 temp;
        SDL_GetTextureAlphaMod(text.get(),&temp);
        return temp;
    }

    ColorMode getColorMode()
    {
        ColorMode pack;
        Uint8 r,g,b;
        SDL_GetTextureColorMod(text.get(),&r,&g,&b);
        pack.r=r;
        pack.g=g;
        pack.b=b;
        return pack;
    }
    int setColorMode(ColorMode mode)
    {
        return SDL_SetTextureColorMod(text.get(),mode.r,mode.g,mode.b);
    }
    RGBA getRGBA()
    {
        return RGBA(getColorMode(),getAlphaMode());
    }
    void setRGBA(RGBA pack)
    {
        setColorMode(pack.toColorMode());
        setAlphaMode(pack.a);
    }
protected:
    Texture()=default;
    /// updateInfo() must be called after Texture is changed.
    void updateInfo()
    {
        SDL_QueryTexture(text.get(), NULL, NULL, &rect.w, &rect.h);
        rect.x = rect.y = 0;
    }
private:
    std::shared_ptr<SDL_Texture> text;
    Rect rect;
    friend class Renderer;
};

enum class RendererType
{
    Software=SDL_RENDERER_SOFTWARE,
    Accelerated=SDL_RENDERER_ACCELERATED,
    PresentSync=SDL_RENDERER_PRESENTVSYNC,
    TargetTexture=SDL_RENDERER_TARGETTEXTURE
};



class Renderer
{
public:
    int setColor(RGBA pack)
    {
        return SDL_SetRenderDrawColor(rnd.get(), pack.r, pack.g, pack.b, pack.a);
    }
    RGBA getColor()
    {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(rnd.get(), &r, &g, &b, &a);
        RGBA pack(r, g, b, a);
        return pack;
    }
    int setBlendMode(BlendMode mode)
    {
        return SDL_SetRenderDrawBlendMode(rnd.get(),static_cast<SDL_BlendMode>(mode));
    }
    BlendMode getBlendMode()
    {
        SDL_BlendMode temp;
        SDL_GetRenderDrawBlendMode(rnd.get(),&temp);
        return static_cast<BlendMode>(temp);
    }

    int setTarget(Texture& t)
    {
        return SDL_SetRenderTarget(rnd.get(),t.text.get());
    }
    int setTarget()
    {
        return SDL_SetRenderTarget(rnd.get(),nullptr);
    }

    int fillRect(Rect rect)
    {
        auto inr = rect.toSDLRect();
        return SDL_RenderFillRect(rnd.get(), &inr);
    }
    int drawRect(Rect rect)
    {
        auto inr = rect.toSDLRect();
        return SDL_RenderDrawRect(rnd.get(), &inr);
    }
    int clear()
    {
        return SDL_RenderClear(rnd.get());
    }
    void update()
    {
        SDL_RenderPresent(rnd.get());
    }
    int copy(Texture t, Rect src, Rect dst)
    {
        SDL_Rect s = src.toSDLRect();
        SDL_Rect d = dst.toSDLRect();
        return SDL_RenderCopy(rnd.get(), t.text.get(), &s, &d);
    }
    int copyTo(Texture t, Rect dst)
    {
        SDL_Rect d = dst.toSDLRect();
        return SDL_RenderCopy(rnd.get(), t.text.get(), NULL, &d);
    }
    int copyFill(Texture t, Rect src)
    {
        SDL_Rect s = src.toSDLRect();
        return SDL_RenderCopy(rnd.get(), t.text.get(), &s, NULL);
    }
    int copyFullFill(Texture t)
    {
        return SDL_RenderCopy(rnd.get(), t.text.get(), NULL, NULL);
    }
    Surface loadSurface(std::string FileName) throw(ErrorViewer)
    {
        Surface surf;
        SDL_Surface* temp = IMG_Load(FileName.c_str());
        if (temp == nullptr)
        {
            ErrorViewer e;
            e.fetch();
            throw e;
        }
        surf.surf.reset(temp, SDL_FreeSurface);
        return surf;
    }
    Texture render(Surface surf) throw (ErrorViewer)
    {
        Texture t;
        SDL_Texture* temp = SDL_CreateTextureFromSurface(rnd.get(), surf.surf.get());
        if (temp == nullptr)
        {
            ErrorViewer e;
            e.fetch();
            throw e;
        }
        t.text.reset(temp, SDL_DestroyTexture);
        t.updateInfo();
        return t;
    }
    Texture loadTexture(std::string FileName) throw(ErrorViewer)
    {
        Texture t;
        SDL_Texture* temp = IMG_LoadTexture(rnd.get(), FileName.c_str());
        if (temp == nullptr)
        {
            ErrorViewer e;
            e.fetch();
            throw e;
        }
        t.text.reset(temp, SDL_DestroyTexture);
        t.updateInfo();
        return t;
    }
    Texture createTexture(int Width,int Height) throw(ErrorViewer)
    {
        SDL_Texture* temp=SDL_CreateTexture(rnd.get(),SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Width, Height);
        if(temp==NULL)
        {
            ErrorViewer e;
            e.fetch();
            throw e;
        }
        Texture t;
        t.text.reset(temp,SDL_DestroyTexture);
        t.updateInfo();
        return t;
    }

protected:
    /// This function is called by class Window ONLY.
    Renderer()=default;
private:
    std::shared_ptr<SDL_Renderer> rnd;
    friend class Window;
};

class Window
{
public:
    Window(std::string Title, int Width, int Height,std::initializer_list<RendererType> RendererFlags= {RendererType::Accelerated,RendererType::TargetTexture}) throw(ErrorViewer)
    {
        SDL_Window* temp = SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_SHOWN);
        if (temp == NULL)
        {
            ErrorViewer e;
            e.fetch();
            throw e;
        }
        wnd.reset(temp, SDL_DestroyWindow);
        setRenderer(RendererFlags);
    }
    Renderer getRenderer() const
    {
        return winrnd;
    }

    void setRenderer(std::initializer_list<RendererType> RendererFlags)
    {
        int flag=0;
        for(auto v:RendererFlags)
        {
            flag|=static_cast<int>(v);
        }
        _setRenderer_Real(flag);
    }

    Rect getSize()
    {
        int w, h;
        SDL_GetWindowSize(wnd.get(), &w, &h);
        return Rect(0, 0, w, h);
    }
    void setSize(Rect sizeRect)
    {
        setSize(sizeRect.w, sizeRect.h);
    }
    void setSize(int w, int h)
    {
        SDL_SetWindowSize(wnd.get(),w,h);
    }

    Rect getPosition()
    {
        int x,y;
        SDL_GetWindowPosition(wnd.get(),&x,&y);
        return Rect(x,y,0,0);
    }
    void setPosition(int x,int y)
    {
        SDL_SetWindowPosition(wnd.get(),x,y);
    }
    /// FIXME: Use class Point or class Rect ?
    void setPosition(Point point)
    {
        SDL_SetWindowPosition(wnd.get(),point.x,point.y);
    }


    void setTitle(std::string Title)
    {
        SDL_SetWindowTitle(wnd.get(),Title.c_str());
    }
    std::string getTitle()
    {
        return std::string(SDL_GetWindowTitle(wnd.get()));
    }

    void setResizable(bool resizable)
    {
        SDL_SetWindowResizable(wnd.get(),static_cast<SDL_bool>(resizable));
    }

    void show()
    {
        SDL_ShowWindow(wnd.get());
    }
    void hide()
    {
        SDL_HideWindow(wnd.get());
    }
    void raise()
    {
        SDL_RaiseWindow(wnd.get());
    }
    void minimize()
    {
        SDL_MinimizeWindow(wnd.get());
    }
    void maximize()
    {
        SDL_MaximizeWindow(wnd.get());
    }
    void restore()
    {
        SDL_RestoreWindow(wnd.get());
    }


    _DECL_DEPRECATED Surface getSurface()
    {
        SDL_Surface* temp=SDL_GetWindowSurface(wnd.get());
        Surface s;
        /// Don't Free This Surface
        s.surf.reset(temp,[](SDL_Surface*){});
        return s;
    }
private:
    void _setRenderer_Real(Uint32 flags)
    {
        winrnd.rnd.reset(SDL_CreateRenderer(wnd.get(), -1, flags),SDL_DestroyRenderer);
    }
    std::shared_ptr<SDL_Window> wnd;
    Renderer winrnd;
};

class Font
{
public:
    Font()=default;
    Font(std::string FontFileName, int size) throw(ErrorViewer)
    {
        if (use(FontFileName, size) != 0)
        {
            ErrorViewer e;
            e.fetch();
            throw e;
        }
    }
    int use(std::string FontFileName, int size)
    {
        TTF_Font* temp = TTF_OpenFont(FontFileName.c_str(), size);
        if (temp == NULL) return -1;
        font.reset(temp, TTF_CloseFont);
        return 0;
    }
    Texture renderText(Renderer rnd, std::string Text, RGBA fg)
    {
        Surface surf;
        surf.surf.reset(TTF_RenderText_Blended(font.get(), Text.c_str(), fg.toSDLColor()));
        Texture t = rnd.render(surf);
        return t;
    }
    Texture renderUTF8(Renderer rnd, std::string Text, RGBA fg)
    {
        Surface surf;
        surf.surf.reset(TTF_RenderUTF8_Blended(font.get(), Text.c_str(), fg.toSDLColor()));
        Texture t = rnd.render(surf);
        return t;
    }
private:
    std::shared_ptr<TTF_Font> font;
};

class SDLSystem
{
public:
    static int SDLInit()
    {
        return SDL_Init(SDL_INIT_EVERYTHING);
    }
    static void SDLQuit()
    {
        SDL_Quit();
    }
    static int IMGInit()
    {
        return IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    }
    static void IMGQuit()
    {
        IMG_Quit();
    }
    static int TTFInit()
    {
        return TTF_Init();
    }
    static void TTFQuit()
    {
        TTF_Quit();
    }
    static int MixInit()
    {
        return Mix_Init(MIX_INIT_MP3);
    }
    static void MixQuit()
    {
        Mix_Quit();
    }

    static void Init()
    {
        SDLInit();
        IMGInit();
        TTFInit();
        MixInit();
    }
    static void Quit()
    {
        MixQuit();
        TTFQuit();
        IMGQuit();
        SDLQuit();
    }

    static void Delay(int ms)
    {
        SDL_Delay(ms);
    }
};


class AudioPlayer
{
public:
    AudioPlayer()
    {
        if(!_sysAudioCounter)
        {
            _sysAudio=new _Audio;
        }
        ++_sysAudioCounter;
    }
    ~AudioPlayer()
    {
        --_sysAudioCounter;
        if(!_sysAudioCounter)
        {
            delete _sysAudio;
        }
    }
private:
    class _Audio
    {
    public:
        _Audio()
        {
            Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
        }
        ~_Audio()
        {
            Mix_CloseAudio();
        }
    };

    static _Audio* _sysAudio;
    static int _sysAudioCounter;
};
AudioPlayer::_Audio* AudioPlayer::_sysAudio=NULL;
int AudioPlayer::_sysAudioCounter=0;


/// Forward Declaration
class Music
{
public:

protected:
    Music()=default;
private:
    std::shared_ptr<Mix_Music> music;
    friend class MusicPlayer;
};

class MusicPlayer : public AudioPlayer
{
public:
    Music loadMusic(std::string Filename) throw (ErrorViewer)
    {
        Mix_Music* temp=Mix_LoadMUS(Filename.c_str());
        if(temp==nullptr)
        {
            ErrorViewer e;
            e.fetch();
            throw e;
        }
        Music m;
        m.music.reset(temp,Mix_FreeMusic);
        return m;
    }

    void load(Music music)
    {
        m=music;
    }
    int play(int loops)
    {
        return Mix_PlayMusic(m.music.get(),loops);
    }
    void pause()
    {
        Mix_PauseMusic();
    }
    void resume()
    {
        Mix_ResumeMusic();
    }
    void rewind()
    {
        Mix_RewindMusic();
    }
    int stop()
    {
        return Mix_HaltMusic();
    }
    int fadeIn(int loops,int ms)
    {
        return Mix_FadeInMusic(m.music.get(),loops,ms);
    }
    int fadeOut(int ms)
    {
        return Mix_FadeOutMusic(ms);
    }

    bool isPlaying()
    {
        return Mix_PlayingMusic();
    }
    bool isPaused()
    {
        return Mix_PausedMusic();
    }
    int isFading()
    {
        switch(Mix_FadingMusic())
        {
        case MIX_NO_FADING:
            return 0;
        case MIX_FADING_IN:
            return 1;
        case MIX_FADING_OUT:
            return 2;
        default:
            return -1;
        }
    }

private:
    Music m;
};


class Sound
{
public:
protected:
    Sound()=default;
private:
    std::shared_ptr<Mix_Chunk> sound;
    friend class SoundPlayer;
};

typedef int ChannelID;

class SoundPlayer : public AudioPlayer
{
public:
    SoundPlayer(int Channels=16)
    {
        Mix_AllocateChannels(Channels);
    }
    Sound loadSound(std::string Filename) throw (ErrorViewer)
    {
        Mix_Chunk* temp=Mix_LoadWAV(Filename.c_str());
        if(temp==NULL)
        {
            ErrorViewer e;
            e.fetch();
            throw e;
        }
        Sound s;
        s.sound.reset(temp,Mix_FreeChunk);
        return s;
    }
    ChannelID playSound(Sound sound,int loops) throw (ErrorViewer)
    {
        ChannelID id;
        if(-1==(id=Mix_PlayChannel(-1,sound.sound.get(),loops)))
        {
            ErrorViewer e;
            e.fetch();
            throw e;
        }
        return id;
    }
    ChannelID fadein(Sound sound,int loops,int ms) throw (ErrorViewer)
    {
        ChannelID id;
        if(-1==(id=Mix_FadeInChannel(-1,sound.sound.get(),loops,ms)))
        {
            ErrorViewer e;
            e.fetch();
            throw e;
        }
        return id;
    }
    int fadeout(ChannelID id,int ms)
    {
        return Mix_FadeOutChannel(id,ms);
    }
    void pause(ChannelID id)
    {
        Mix_Pause(id);
    }
    void resume(ChannelID id)
    {
        Mix_Resume(id);
    }
    int stop(ChannelID id)
    {
        return Mix_HaltChannel(id);
    }

private:

};

class Event
{
public:
    int gettype()
    {
        return e.type;
    }
protected:
    Event()=default;
    SDL_Event e;
private:
    friend class EventEngine;
};

class MouseButtonEvent : public Event
{
public:
    int getx()
    {
        return e.button.x;
    }
    int gety()
    {
        return e.button.y;
    }
    int getbutton()
    {
        return e.button.button;
    }
};

class EventEngine
{
public:
    Event poll(bool mustNew=false) /// mustNew: false=SDL_PollEvent(&e) returns 0 ; true=SDL_PollEvent(&e) returns 1
    {
        Event e;
        while(1)
        {
            int ret=SDL_PollEvent(&e.e);
            if((mustNew&&ret)||!mustNew) break;
        }
        return e;
    }
    Event wait()
    {
        Event e;
        SDL_WaitEvent(&e.e);
        return e;
    }
    Event waitfor(int ms)
    {
        Event e;
        SDL_WaitEventTimeout(&e.e,ms);
        return e;
    }
};

}/// End of namespace MiniEngine

namespace App
{

int main(int argc,char* argv[]);

}/// End of namespace App


/// Default Setup Code
int main(int argc,char* argv[])
{
    MiniEngine::SDLSystem::Init();
    int ret=App::main(argc,argv);
    MiniEngine::SDLSystem::Quit();
    return ret;
}
