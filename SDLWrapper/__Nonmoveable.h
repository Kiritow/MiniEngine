#pragma once
#include "begin_code.h"
class NonMoveable
{
public:
    NonMoveable(NonMoveable&&) =delete;
    NonMoveable& operator = (NonMoveable&&)=delete;
};
#include "end_code.h"
