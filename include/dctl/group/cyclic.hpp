#pragma once
#include <set>                          // set
#include <dctl/angle/angle.hpp>         // Angle
#include <dctl/angle/transform.hpp>     // rotate2, inverse2
#include <dctl/group/primitives.hpp>    // make

namespace dctl {
namespace group {
namespace cyclic {

struct Op
{
        template<class Object>
        auto operator()(Object const& obj, Angle const& a) const
        {
                return rotate2(obj, a);
        }
};

struct Inv
{
        auto operator()(Angle const& a) const
        {
                return inverse2(a);
        }
};

}       // namespace cyclic

auto make_cyclic(std::initializer_list<Angle> set)
{
        return group::make(std::set<Angle>{set}, cyclic::Op{}, cyclic::Inv{});
}

}       // namespace group
}       // namespace dctl
