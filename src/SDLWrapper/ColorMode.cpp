#include "SDLWrapper/ColorMode.h"
namespace MiniEngine {
ColorMode::ColorMode(int R, int G, int B)
{
    r = R;
    g = G;
    b = B;
}

ColorMode::ColorMode()
{
    r = g = b = 0;
}
} /// End of namespace MiniEngine
