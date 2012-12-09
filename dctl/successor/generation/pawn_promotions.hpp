#pragma once
#include <functional>                   // function
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/generation/generator_fwd.hpp>
#include <dctl/successor/generation/pawn_moves.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/promotion.hpp>
#include <dctl/node/stack.hpp>
#include <dctl/node/unary_projections.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves generation
template<bool Color, typename Position>
struct generator<Color, Material::pawn, Promotions, Position>
:
        // enforce reference semantics
        private boost::noncopyable
{
private:
        // typedefs

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
                if (auto const active_promotors = promoting_pawns<Color>(p))
                        PawnMoves{moves_}.select(active_promotors, not_occupied(p));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
