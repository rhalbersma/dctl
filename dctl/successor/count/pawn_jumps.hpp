#pragma once
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/count/enumerator_fwd.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/successor/copy/pawn_jumps.hpp>
#include <dctl/capture/state.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps enumeration
template<bool Color, typename Position>
struct enumerator<Color, Material::pawn, Jumps, Position>
:
        // enforce reference semantics
        private boost::noncopyable
{
private:
        // typedefs

        typedef generator<Color, Material::pawn, Jumps, Position> PawnJumps;
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
                PawnJumps{capture_}(p);
                return capture_.size();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
