#pragma once
#include "include.h"
#include "_MessageBoxType.h"
#include <string>
#include <functional>
#include <vector>
#include "begin_code.h"
class WindowMessageBoxButton
{
public:
    WindowMessageBoxButton();
    WindowMessageBoxButton(const std::string& ButtonText,const std::function<void()>& CallbackFunc=[]() {});

    std::string text;
    std::function<void()> callback;

    /// Default: no hit option set.
    void setHitAsReturn(bool);
    void setHitAsEscape(bool);

    bool isHitAsReturn() const;
    bool isHitAsEscape() const;
private:
    int _hitoption;
};

class WindowMessageBox
{
public:
    WindowMessageBox();
    WindowMessageBox(const std::string& Title,const std::string& Text,MessageBoxType BoxType=MessageBoxType::Information,const std::function<void()>& DefaultCallback=[]() {});

    MessageBoxType boxtype;
    std::string title;
    std::string text;
    std::function<void()> defaultcallback;

    void addButton(const WindowMessageBoxButton& button);
    int getButtonNum() const;
    WindowMessageBoxButton& getButton(int index);
    const WindowMessageBoxButton& getButtonConst(int index) const;
private:
    std::vector<WindowMessageBoxButton> _vec;
};
#include "end_code.h"