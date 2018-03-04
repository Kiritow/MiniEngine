#pragma once
namespace MiniEngine {
class NonMoveable
{
public:
    NonMoveable()=default;
    ~NonMoveable()=default;
    NonMoveable(NonMoveable&&) =delete;
    NonMoveable& operator = (NonMoveable&&)=delete;
};
} /// End of namespace MiniEngine
