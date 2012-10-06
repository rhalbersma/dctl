#pragma once
#include <functional>                   // function
#include <boost/utility.hpp>            // noncopyable
#include "Enumerator_fwd.hpp"
#include "../Select.hpp"
#include "../generation/BothJumps.hpp"
#include "../../capture/State.hpp"
#include "../../node/Material.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king jumps enumeration
template<bool Color, typename Position>
struct enumerator<Color, Material::both, Jumps, Position>
:
        // enforce reference semantics
        private boost::noncopyable,
        public std::function<int(Position const&)>
{
private:
        // typedefs

        typedef generator<Color, Material::both, Jumps, Position> BothJumps;
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
                (BothJumps(capture_))(p);
                return capture_.size();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
