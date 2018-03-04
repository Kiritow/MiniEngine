#pragma once
#include <experimental/optional>
namespace MiniEngine {
template<typename T>
using Optional = std::experimental::optional<T>;

using BadOptionalAccess = std::experimental::bad_optional_access;

constexpr std::experimental::nullopt_t NullOpt = std::experimental::nullopt;
} /// End of namespace MiniEngine
