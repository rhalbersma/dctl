#pragma once
#include <type_traits>                                  // false_type, true_type
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/aux/king_jumps.hpp>       // copy (king jumps specialization)
#include <dctl/successor/copy/aux/pawn_jumps.hpp>       // copy (pawn jumps specialization)
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/node/material.hpp>                       // Material
#include <dctl/rules/traits.hpp>                        // traits

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for combined king and pawn jumps
template<bool Color, typename Position>
struct copy<Color, Material::both, select::jumps, Position>
{
public:
        template<typename Vector>
        void operator()(Position const& p, Vector& moves) const
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
        template<typename Vector>
        void precedence_dispatch(Position const& p, Vector& moves, std::false_type) const
        {
                Propagate<select::jumps, Position> propagate(p);
                KingJumps{propagate, moves}(p.kings(Color));
                PawnJumps{propagate, moves}(p.pawns(Color));
        }

        // overload for absolute king jump precedence
        template<typename Vector>
        void precedence_dispatch(Position const& p, Vector& moves, std::true_type) const
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
