#pragma once
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/generation/generator_fwd.hpp>
#include <dctl/successor/generation/king_moves.hpp>
#include <dctl/successor/generation/pawn_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct generator<Color, Material::both, Moves, Position>
:
        // enforce reference semantics
        private boost::noncopyable
{
private:
        // typedefs

        typedef generator<Color, Material::king, Moves, Position> KingMoves;
        typedef generator<Color, Material::pawn, Moves, Position> PawnMoves;

        // representation

        Stack& moves_;

public:
        // structors

        /*explicit*/ generator(Stack& m)
        :
                moves_(m)
        {}

        // function call operators

        void operator()(Position const& p) const
        {
                KingMoves{moves_}(p);
                PawnMoves{moves_}(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
