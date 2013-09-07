#pragma once
#include <dctl/utility/abs.hpp>         // abs_remainder
#include <dctl/utility/range.hpp>       // is_element

namespace dctl {

template<class T>
constexpr auto make_angle(T const& n) noexcept
{
        return util::abs_remainder(n, 360);
}

template<class T>
constexpr auto is_angle(T const& n) noexcept
{
        return util::is_element(n, {0, 360});
}

}       // namespace dctl
