#include "SDLWrapper/MessageBox.h"
namespace MiniEngine {
WindowMessageBoxButton::WindowMessageBoxButton()
{
    _hitoption=0;
    text="Button";
    callback=[]() {};
}

WindowMessageBoxButton::WindowMessageBoxButton(const std::string& ButtonText,const std::function<void()>& CallbackFunc) : text(ButtonText)
{
    _hitoption=0;
    callback=CallbackFunc;
}

void WindowMessageBoxButton::setHitAsEscape(bool enable)
{
    _hitoption=enable?1:0;
}

void WindowMessageBoxButton::setHitAsReturn(bool enable)
{
    _hitoption=enable?2:0;
}

bool WindowMessageBoxButton::isHitAsEscape() const
{
    return _hitoption==1;
}

bool WindowMessageBoxButton::isHitAsReturn() const
{
    return _hitoption==2;
}

WindowMessageBox::WindowMessageBox()
{
    boxtype=MessageBoxType::Information;
}

WindowMessageBox::WindowMessageBox(const std::string& Title,const std::string& Text,MessageBoxType BoxType,const std::function<void()>& DefaultCallback) : title(Title), text(Text)
{
    boxtype=BoxType;
    defaultcallback=DefaultCallback;
}

void WindowMessageBox::addButton(const WindowMessageBoxButton& button)
{
    _vec.push_back(button);
}

int WindowMessageBox::getButtonNum() const
{
    return _vec.size();
}

WindowMessageBoxButton& WindowMessageBox::getButton(int index)
{
    return _vec.at(index);
}

const WindowMessageBoxButton& WindowMessageBox::getButtonConst(int index) const
{
    return _vec.at(index);
}
} /// End of namespace MiniEngine
