Rect::Rect()
{
    x=y=w=h=0;
}
Rect::Rect(int incx,int incy,int incw,int inch)
{
    x=incx;
    y=incy;
    w=incw;
    h=inch;
}
Rect::~Rect()
{

}

SDL_Rect Rect::toSDLRect()
{
    SDL_Rect rect;
    rect.x=x;
    rect.y=y;
    rect.w=w;
    rect.h=h;
    return rect;
}
