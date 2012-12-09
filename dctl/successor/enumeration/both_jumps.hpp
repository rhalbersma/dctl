#pragma once
#include <functional>                   // function
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/enumeration/enumerator_fwd.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/successor/generation/both_jumps.hpp>
#include <dctl/capture/state.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps enumeration
template<bool Color, typename Position>
struct enumerator<Color, Material::both, Jumps, Position>
:
        // enforce reference semantics
        private boost::noncopyable
{
private:
        // typedefs

        typedef generator<Color, Material::both, Jumps, Position> BothJumps;
        typedef capture::State<Position> State;

        // representation

        State& capture_;

public:
        // structors

        /*explicit*/ enumerator(State& c)
        :
                capture_(c)
        {}

        // function call operators

        int operator()(Position const& p) const
        {
                BothJumps{capture_}(p);
                return capture_.size();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
