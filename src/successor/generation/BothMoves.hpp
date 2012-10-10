#pragma once
#include <functional>                   // function
#include <boost/utility.hpp>            // noncopyable
#include "Generator_fwd.hpp"
#include "KingMoves.hpp"
#include "PawnMoves.hpp"
#include "../Select.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct generator<Color, Material::both, Moves, Position>
:
        // enforce reference semantics
        private boost::noncopyable,
        public std::function<void(Position const&)>
{
private:
        // typedefs

        typedef generator<Color, Material::king, Moves, Position> KingMoves;
        typedef generator<Color, Material::pawn, Moves, Position> PawnMoves;

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
