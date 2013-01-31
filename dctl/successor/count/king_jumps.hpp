#pragma once
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/count/enumerator_fwd.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/successor/copy/king_jumps.hpp>
#include <dctl/successor/propagate/jumps.hpp>
#include <dctl/node/material.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps enumeration
template<bool Color, typename Position>
struct enumerator<Color, Material::king, Jumps, Position>
:
        // enforce reference semantics
        private boost::noncopyable
{
private:
        // typedefs

        typedef generator<Color, Material::king, Jumps, Position> KingJumps;
        typedef Propagate<Jumps, Position> State;

        // representation

        State& capture_;

public:
        // structors

        /*explicit*/ enumerator(State& c)
        :
                capture_() 
        {}

        // function call operators

        int operator()(Position const& p) const
        {
                KingJumps{capture_}(p);
                return capture_.size();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
