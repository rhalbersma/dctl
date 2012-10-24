#pragma once
#include <functional>                   // function
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/enumeration/Enumerator_fwd.hpp>
#include <dctl/successor/Select.hpp>
#include <dctl/successor/generation/PawnJumps.hpp>
#include <dctl/capture/State.hpp>
#include <dctl/node/Material.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps enumeration
template<bool Color, typename Position>
struct enumerator<Color, Material::pawn, Jumps, Position>
:
        // enforce reference semantics
        private boost::noncopyable,
        public std::function<int(Position const&)>
{
private:
        // typedefs

        typedef generator<Color, Material::pawn, Jumps, Position> PawnJumps;
        typedef capture::State<Position> State;

        // representation

        State& capture_;

public:
        // structors
        
        explicit enumerator(State& c)
        : 
                capture_(c) 
        {}

        // function call operators

        int operator()(Position const& p) const
        {
                // parentheses around function objects to avoid "C++'s most vexing parse"
                (PawnJumps(capture_))(p);
                return capture_.size();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
