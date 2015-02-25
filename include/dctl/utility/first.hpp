#pragma once

namespace dctl {
namespace util {

// primary template
template<bool>
struct First;

template<>
struct First<false>
{
        template<class Set>
        constexpr auto operator()(Set const& s) const
        {
                return *s.begin();
        }
};

template<>
struct First<true>
{
        template<class Set>
        constexpr auto operator()(Set const& s) const
        {
                return *s.rbegin();
        }
};

}       // namespace util
}       // namespace dctl
