#pragma once
#include <utility>                      // pair

namespace dctl {
namespace util {

template<class T>
constexpr auto is_element(T const& value, std::pair<T, T> const& range) noexcept
{
        return range.first <= value && value < range.second;
}

}       // util
}       // dctl
