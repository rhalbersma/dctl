#pragma once
#include <set>                          // set
#include <dctl/angle/transform.hpp>     // rotate, inverse
#include <dctl/group/primitives.hpp>    // Group, make_group

namespace dctl {
namespace group {
namespace cyclic {

using Element = int;
using Set = std::set<Element>;

struct Op
{
        constexpr auto operator()(Element const& a, Element const& b) const
        {
                return angle::rotate(a, b);
        }

        template<class Object>
        constexpr auto operator()(Object const& obj, Element const& a) const
        {
                return rotate(obj, a);  // ADL on Object
        }
};

using Id = Element;

struct Inv
{
        constexpr auto operator()(Element const& a) const
        {
                return angle::inverse(a);
        }
};

using CyclicGroup = Group<Set, Op, Id, Inv>;

auto make_cyclic_group(Set const& elements)
{
        return group::make_group(elements, Op(), Id(), Inv());
}

}       // namespace cyclic
}       // namespace group
}       // namespace dctl
