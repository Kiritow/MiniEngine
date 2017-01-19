#include "Widget.h"

using namespace Engine;
using namespace std;

namespace MiniEngine
{
/// Brush
#include "widget_brush.hpp"

struct SimpleProgressBar::impl
{
    int percentage;
    int w,h;
    RGBA ucolor,lcolor;
};

SimpleProgressBar::SimpleProgressBar(int incw,int inch,RGBA upper_color,RGBA lower_color)
{
    pimpl=new impl;
    pimpl->w=incw;
    pimpl->h=inch;
    pimpl->ucolor=upper_color;
    pimpl->lcolor=lower_color;
    pimpl->percentage=0;
}
void SimpleProgressBar::DrawAt(Renderer& rnd,int x,int y)
{
    RGBA prev=rnd.getColor();
    if(pimpl->percentage)
    {
        rnd.setColor(pimpl->ucolor);
        rnd.fillRect(Rect(x,y,pimpl->w*pimpl->percentage/100,pimpl->h));
    }
    if(pimpl->percentage-100)
    {
        rnd.setColor(pimpl->lcolor);
        rnd.fillRect(Rect(x+pimpl->w*pimpl->percentage/100,y,pimpl->w*(100-pimpl->percentage)/100,pimpl->h));
    }
    rnd.setColor(prev);
}
void SimpleProgressBar::setPercentage(int iPercentage)
{
    pimpl->percentage=min(max(0,iPercentage),100);
}

}/// End of namespace MiniEngine
