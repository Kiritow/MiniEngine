RGBA::RGBA()
{
    r=g=b=a=0;
}
RGBA::RGBA(int incr,int incg,int incb,int inca)
{
    r=incr;
    g=incg;
    b=incb;
    a=inca;
}
SDL_Color RGBA::toSDLColor()
{
    SDL_Color color;
    color.r=r;
    color.g=g;
    color.b=b;
    color.a=a;
    return color;
}
RGBA::~RGBA()
{

}
