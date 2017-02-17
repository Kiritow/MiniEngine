#pragma once
#include <SDL2/SDL.h>
#undef main
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <memory>

namespace MiniEngine
{
	using namespace std;


	class Rect
	{
	public:
		int x, y, w, h;
		Rect(int X, int Y, int W, int H)
		{
			x = X;y = Y;w = W;h = H;
		}
		Rect()
		{
			x = y = w = h = 0;
		}
		SDL_Rect toSDLRect()
		{
			SDL_Rect r;
			r.x = x;r.y = y;r.w = w;r.h = h;
			return r;
		}
	};

	class Point
	{
    public:
        int x,y;
        Point(int X,int Y)
        {
            x=X;y=Y;
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

	class RGBA
	{
	public:
		int r, g, b, a;
		RGBA(int R, int G, int B, int A)
		{
			r = R;g = G;b = B;a = A;
		}
		RGBA()
		{
			r = g = b = a = 0;
		}
		SDL_Color toSDLColor()
		{
			SDL_Color c;
			c.r = r;c.g = g;c.b = b;c.a = a;
			return c;
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

	class Window;
	class Renderer;

	class ErrorViewer : public exception
	{
	public:
		void fetch()
		{
			str = SDL_GetError();
		}
		string getError()
		{
			return str;
		}
		const char* what() const throw() override
		{
			return str.c_str();
		}
	private:
		string str;
	};

	class Surface
	{
	public:
		~Surface()
		{

		}
	protected:
		Surface() = default;
	private:
		shared_ptr<SDL_Surface> surf;
		friend class Renderer;
		friend class Font;
	};

	class Texture
	{
	public:
		~Texture()
		{

		}
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
	protected:
		Texture()
		{

		}
		void updateInfo()
		{
			SDL_QueryTexture(text.get(), NULL, NULL, &rect.w, &rect.h);
			rect.x = rect.y = 0;
		}
	private:
		shared_ptr<SDL_Texture> text;
		Rect rect;
		friend class Renderer;
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
		Surface loadSurface(string FileName) throw(ErrorViewer)
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
		Texture loadTexture(string FileName) throw(ErrorViewer)
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

	protected:
		Renderer()
		{
			/// This function is called by Window ONLY.
		}
	private:
		shared_ptr<SDL_Renderer> rnd;
		friend class Window;
	};

	class Window
	{
	public:
		Window(string Title, int Width, int Height) throw(ErrorViewer)
		{
			SDL_Window* temp = SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_SHOWN);
			if (temp == NULL)
			{
				ErrorViewer e;
				e.fetch();
				throw e;
			}
			wnd.reset(temp, SDL_DestroyWindow);
			setRenderer();
		}
		Renderer getRenderer() const
		{
			return winrnd;
		}
		void setRenderer()
		{
			winrnd.rnd.reset(SDL_CreateRenderer(wnd.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE),SDL_DestroyRenderer);
		}
		Rect getSize()
		{
			int w, h;
			SDL_GetWindowSize(wnd.get(), &w, &h);
			Rect rect(0, 0, w, h);
			return rect;
		}
		void setSize(Rect sizeRect)
		{
			setSize(sizeRect.w, sizeRect.h);
		}
		void setSize(int w, int h)
		{
			SDL_SetWindowSize(wnd.get(),w,h);
		}
	private:
		shared_ptr<SDL_Window> wnd;
		Renderer winrnd;
	};

	class Font
	{
	public:
		Font()
		{

		}
		Font(string FontFileName, int size) throw(ErrorViewer)
		{
			if (use(FontFileName, size) != 0)
			{
				ErrorViewer e;
				e.fetch();
				throw e;
			}
		}
		int use(string FontFileName, int size)
		{
			TTF_Font* temp = TTF_OpenFont(FontFileName.c_str(), size);
			if (temp == NULL) return -1;
			font.reset(temp, TTF_CloseFont);
			return 0;
		}
		Texture renderText(Renderer rnd, string Text, RGBA fg)
		{
			Surface surf;
			surf.surf.reset(TTF_RenderText_Blended(font.get(), Text.c_str(), fg.toSDLColor()));
			Texture t = rnd.render(surf);
			return t;
		}

		Texture renderUTF8(Renderer rnd, string Text, RGBA fg)
		{
			Surface surf;
			surf.surf.reset(TTF_RenderUTF8_Blended(font.get(), Text.c_str(), fg.toSDLColor()));
			Texture t = rnd.render(surf);
			return t;
		}
	private:
		shared_ptr<TTF_Font> font;
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

		static void delay(int ms)
		{
			SDL_Delay(ms);
		}
	};



}/// End of namespace MiniEngine
