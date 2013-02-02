#pragma once
#include <type_traits>                                  // false_type, true_type
#include <dctl/successor/copy/primary_fwd.hpp>
#include <dctl/successor/copy/aux/king_jumps.hpp>
#include <dctl/successor/copy/aux/pawn_jumps.hpp>
#include <dctl/successor/propagate/jumps.hpp>
#include <dctl/successor/select/jumps.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/move.hpp>
#include <dctl/node/stack.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position>
struct copy<Color, Material::both, select::jumps, Position>
{
public:
        void operator()(Position const& p, Vector<Move>& moves) const
        {
                typedef typename Position::rules_type Rules;

                // tag dispatching on absolute king jump precedence
                precedence_dispatch(p, moves, typename rules::traits<Rules>::is_absolute_king_precedence());
        }

private:
        // typedefs

        typedef aux::copy<Color, Material::king, select::jumps, Position> KingJumps;
        typedef aux::copy<Color, Material::pawn, select::jumps, Position> PawnJumps;

        // overload for no absolute king jump precedence
        void precedence_dispatch(Position const& p, Vector<Move>& moves, std::false_type) const
        {
                Propagate<select::jumps, Position> propagate(p);
                KingJumps{propagate, moves}(p.kings(Color));
                PawnJumps{propagate, moves}(p.pawns(Color));
        }

        // overload for absolute king jump precedence
        void precedence_dispatch(Position const& p, Vector<Move>& moves, std::true_type) const
        {
                Propagate<select::jumps, Position> propagate(p);
                KingJumps{propagate, moves}(p.kings(Color));
                if (moves.empty())
                        PawnJumps{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
