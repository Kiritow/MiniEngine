#pragma once
#include "SDLWrapper/include.h"
#include "SDLWrapper/BlendMode.h"
#include "SDLWrapper/WindowType.h"
#include "SDLWrapper/SystemCursorType.h"
#include "SDLWrapper/FontStyle.h"
#include "SDLWrapper/FlipMode.h"
#include "SDLWrapper/SDLInitFlag.h"
#include "SDLWrapper/IMGInitFlag.h"
#include "SDLWrapper/MixInitFlag.h"
#include "SDLWrapper/MusicType.h"
#include <vector>
namespace MiniEngine {
namespace _internal
{
BlendMode getBlendModeFromSDLBlendMode(SDL_BlendMode mode);
SDL_BlendMode getSDLBlendModeFromBlendMode(BlendMode mode);
/// FIXME: return SDL_WindowFlags or Uint32 ?
Uint32 getSDLWindowFlagsFromWindowType(WindowType type);
SystemCursorType getCursorTypeFromSDLSystemCursor(SDL_SystemCursor id);
SDL_SystemCursor getSDLSystemCursorFromSystemCursorType(SystemCursorType type);
int getTTFFontStyleFromFontStyle(FontStyle style);
std::vector<FontStyle> getFontStyleVecFromMixedTTFFontStyle(int Mixed_TTF_Font_Style);
SDL_RendererFlip getSDLRendererFlipFromFlipMode(FlipMode mode);
Uint32 getUint32FromSDLInitFlag(SDLInitFlag flag);
int getIntFromIMGInitFlag(IMGInitFlag flag);
int getIntFromMixInitFlag(MixInitFlag flag);
MusicType getMusicTypeFromMixMusicType(Mix_MusicType);
Mix_MusicType getMixMusicTypeFromMusicType(MusicType);
}/// End of namespace _internal
}/// End of namespace MiniEngine
