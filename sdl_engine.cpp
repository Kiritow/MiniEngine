#include "sdl_engine.h"
#include "unistd.h"

SDL_Texture* RenderText(SDL_Renderer* rnd,TTF_Font* font,const char* Text,SDL_Color color,int* pw,int* ph)
{
    SDL_Surface* surf=TTF_RenderText_Blended(font,Text,color);
    if(surf==NULL) return NULL;
    SDL_Texture* texture=SDL_CreateTextureFromSurface(rnd,surf);
    SDL_FreeSurface(surf);
    if(pw&&ph) SDL_QueryTexture(texture,NULL,NULL,pw,ph);
    return texture;
}

SDL_Texture* RenderUTF8(SDL_Renderer* rnd,TTF_Font* font,const char* Text,SDL_Color color,int* pw,int* ph)
{
    SDL_Surface* surf=TTF_RenderUTF8_Blended(font,Text,color);
    if(surf==NULL) return NULL;
    SDL_Texture* texture=SDL_CreateTextureFromSurface(rnd,surf);
    SDL_FreeSurface(surf);
    if(pw&&ph) SDL_QueryTexture(texture,NULL,NULL,pw,ph);
    return texture;
}

void TextureDraw(SDL_Renderer* rnd,SDL_Texture* texture,int dstx,int dsty)
{
    if(!rnd||!texture) return;
    int w,h;
    SDL_QueryTexture(texture,NULL,NULL,&w,&h);
    SDL_Rect rect;
    rect.x=dstx;
    rect.y=dsty;
    rect.w=w;
    rect.h=h;
    SDL_RenderCopy(rnd,texture,NULL,&rect);
}

SDL_Texture* MyLoadImage(SDL_Renderer* rnd,const char* FileName,int* pw,int* ph)
{
    SDL_Surface* surf=IMG_Load(FileName);
    if(surf==NULL) return NULL;
    SDL_Texture* texture=SDL_CreateTextureFromSurface(rnd,surf);
    SDL_FreeSurface(surf);
    if(texture==NULL) return NULL;
    if(pw&&ph) SDL_QueryTexture(texture,NULL,NULL,pw,ph);
    return texture;
}

bool isInRect(int x,int y,SDL_Rect rect)
{
    return ((x>=rect.x&&x<=rect.x+rect.w)&&(y>=rect.y&&y<=rect.y+rect.h));
}

bool isInRect(int x,int y,int LU_x,int LU_y,int RD_x,int RD_y)
{
    return ((x>=LU_x&&x<=RD_x)&&(y>=LU_y&&y<=RD_y));
}

void ClearMessageQueue()
{
    /// Clear Message Queue
    while(SDL_PeepEvents(NULL,1,SDL_GETEVENT,SDL_FIRSTEVENT,SDL_LASTEVENT));
}

SDL_Color color_white { 255,255,255 };
SDL_Color color_black { 0,0,0 };

int MyChangeDir(const char* DirName)
{
    mlog("Change Dir to \"%s\"",DirName);
    int ret=chdir(DirName);
    mlog("Change Dir returns %d",ret);
    return ret;
}
