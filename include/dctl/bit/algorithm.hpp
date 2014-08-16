#pragma once
#include <utility>                      // make_pair

namespace dctl {
namespace bit {

template<class Set>
auto set_single(Set const& s) noexcept
{
        return s.count() == 1;
}

template<class Set>
auto set_double(Set const& s) noexcept
{
        return s.count() == 2;
}

template<class Set>
auto set_multiple(Set const& s) noexcept
{
        return s.count() >= 2;
}

template<class Set>
auto min_element(Set const& s) noexcept
{
        return s.begin();
}

template<class Set>
auto max_element(Set const& s) noexcept
{
        return s.rbegin().base();
}

template<class Set>
auto minmax_element(Set const& s) noexcept
{
        return std::make_pair(set_min_element(s), set_max_element(s));
}

}       // namespace bit
}       // namespace dctl
