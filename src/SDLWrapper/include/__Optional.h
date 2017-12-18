#pragma once
#include <experimental/optional>
#include "begin_code.h"
template<typename T>
using Optional = std::experimental::optional<T>;

using BadOptionalAccess = std::experimental::bad_optional_access;

constexpr std::experimental::nullopt_t NullOpt = std::experimental::nullopt;
#include "end_code.h"
