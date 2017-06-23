#pragma once
#include <exception>
#include <string>
#include "begin_code.h"
class ErrorViewer : public std::exception
{
public:
    void fetch();
    std::string getError() const;
    const char* what() const throw() override;
private:
    std::string str;
};
#include "end_code.h"
