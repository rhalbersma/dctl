#pragma once
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/copy/generator_fwd.hpp>
#include <dctl/successor/copy/pawn_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/stack.hpp>
#include <dctl/node/unary_projections.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves generation
template<bool Color, typename Position>
struct generator<Color, Material::pawn, Reverse, Position>
:
        // enforce reference semantics
        private boost::noncopyable
{
private:
        // typedefs

        typedef generator<!Color, Material::pawn, Moves, Position> PassivePawnMoves;

        // representation

        Vector<Move>& moves_;

public:
        // structors

        /*explicit*/ generator(Vector<Move>& m)
        :
                moves_(m)
        {}

        // function call operators

        void operator()(Position const& p) const
        {
                if (auto const active_pawns = p.pawns(Color))
                        PassivePawnMoves{moves_}.select(active_pawns, not_occupied(p));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
