#pragma once
#include <dctl/angle/transform.hpp>     // inverse, rotate, mirror

namespace dctl {
namespace group {
namespace cyclic {

struct Inverse
{
        template<class T>
        constexpr auto operator()(T const& alpha) const noexcept
        {
                return angle::inverse(alpha);
        }
};

struct Rotate
{
        template<class T>
        constexpr auto operator()(T const& alpha, T const& theta) const noexcept
        {
                return angle::rotate(alpha, theta);
        }
/*
        template<class Object, class T>
        constexpr auto operator()(Object const& object, T const& theta) noexcept
        {
                return rotate(object, theta);
        }*/
};

struct Mirror
{
        template<class T>
        constexpr auto operator()(T const& alpha, T const& theta) const noexcept
        {
                return angle::mirror(alpha, theta);
        }
/*
        template<class Object, class T>
        constexpr auto operator()(Object const& object, T const& theta) noexcept
        {
                return mirror(object, theta);
        }*/
};

}       // namespace cyclic
}       // namespace group
}       // namespace dctl
