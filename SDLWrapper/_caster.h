#pragma once
#include "include.h"
#include "_BlendMode.h"
#include "_WindowType.h"
#include "_SystemCursorType.h"
#include "_FontStyle.h"
#include "_FlipMode.h"
#include <vector>
#include "begin_code.h"
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
}/// End of namespace _internal
#include "end_code.h"
