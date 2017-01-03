#pragma once
#include "basic_config.h"

SDL_Texture* RenderText(SDL_Renderer* rnd,TTF_Font* font,const char* Text,SDL_Color color,int* pw,int* ph);
SDL_Texture* RenderUTF8(SDL_Renderer* rnd,TTF_Font* font,const char* Text,SDL_Color color,int* pw,int* ph);
void TextureDraw(SDL_Renderer* rnd,SDL_Texture* texture,int dstx,int dsty);
SDL_Texture* MyLoadImage(SDL_Renderer* rnd,const char* FileName,int* pw,int* ph);
bool isInRect(int x,int y,SDL_Rect rect);
bool isInRect(int x,int y,int LU_x,int LU_y,int RD_x,int RD_y);
void ClearMessageQueue();

int MyChangeDir(const char* DirName);

extern SDL_Color color_white;
extern SDL_Color color_black;
