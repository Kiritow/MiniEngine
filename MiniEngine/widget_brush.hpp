struct Brush::impl
{
    Renderer rnd;
    Rect area;
    impl(const Renderer& incrnd) : rnd(incrnd)
    {

    }
};
Brush::Brush(const Window& wnd,Rect DrawableArea) /// Protected
{
    pimpl=new impl(wnd.getRenderer());
    pimpl->area=DrawableArea;
}
void Brush::copy(Texture t,Rect src,Rect dst,bool autoZoom)
{
    dst.x+=pimpl->area.x;
    dst.y+=pimpl->area.y;
    if(dst.w>pimpl->area.w) dst.w=pimpl->area.w;
    if(dst.h>pimpl->area.h) dst.h=pimpl->area.h;
    if(autoZoom)
    {

    }
    else
    {
        if(src.w>pimpl->area.w) src.w=pimpl->area.w;
        if(src.h>pimpl->area.h) src.h=pimpl->area.h;
    }
    pimpl->rnd.copy(t,src,dst);
}
void Brush::copyTo(Texture t,Rect dst,bool autoZoom)
{
    dst.x+=pimpl->area.x;
    dst.y+=pimpl->area.y;

    if(dst.w>pimpl->area.w) dst.w=pimpl->area.w;
    if(dst.h>pimpl->area.h) dst.h=pimpl->area.h;

    if(autoZoom)
    {
        pimpl->rnd.copyTo(t,dst);
    }
    else
    {
        int w=t.getw();
        int h=t.geth();
        if(w>pimpl->area.w) w=pimpl->area.w;
        if(h>pimpl->area.h) h=pimpl->area.h;
        Rect src(0,0,w,h);
        pimpl->rnd.copy(t,src,dst);
    }
}
void Brush::copyFill(Texture t,Rect src)
{
    Rect dst=pimpl->area;
    pimpl->rnd.copy(t,src,dst);
}

void Brush::copyFullFill(Texture t)
{
    Rect dst=pimpl->area;
    pimpl->rnd.copyTo(t,dst);
}

Brush::~Brush()
{
    delete pimpl;
}
