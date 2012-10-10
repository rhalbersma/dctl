#pragma once
#include <functional>                   // function
#include <boost/utility.hpp>            // noncopyable
#include "Generator_fwd.hpp"
#include "KingReverse.hpp"
#include "PawnReverse.hpp"
#include "../Select.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct generator<Color, Material::both, Reverse, Position>
:
        // enforce reference semantics
        private boost::noncopyable,
        public std::function<void(Position const&)>
{
private:
        // typedefs

        typedef generator<Color, Material::king, Reverse, Position> KingMoves;
        typedef generator<Color, Material::pawn, Reverse, Position> PawnMoves;

        // representation

        Stack& moves_;

public:
        // structors

        explicit generator(Stack& m)
        : 
                moves_(m) 
        {}

        // function call operators

        void operator()(Position const& p) const
        {
                // parentheses around function objects to avoid "C++'s most vexing parse"
                (KingMoves(moves_))(p);
                (PawnMoves(moves_))(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
