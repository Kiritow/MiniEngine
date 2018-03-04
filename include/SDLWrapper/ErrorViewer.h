#pragma once
#include <exception>
#include <string>

namespace MiniEngine {

class ErrorViewer : public std::exception
{
public:
    void fetch();
    std::string getError() const;
    const char* what() const noexcept override;
private:
    std::string str;
};

} /// End of namespace MiniEngine
