#pragma once
#include <type_traits>                                  // false_type, true_type
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/impl/king_jumps.hpp>      // copy (king jumps specialization)
#include <dctl/successor/copy/impl/pawn_jumps.hpp>      // copy (pawn jumps specialization)
#include <dctl/successor/material/piece.hpp>            // piece
#include <dctl/successor/material/king.hpp>             // king
#include <dctl/successor/material/pawn.hpp>             // pawn
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/rules/traits.hpp>                        // traits

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for piece jumps
template<bool Color>
struct copy<Color, material::piece, select::jumps>
{
public:
        template<typename Position, typename Vector>
        void operator()(Position const& p, Vector& moves) const
        {
                typedef typename Position::rules_type Rules;

                // tag dispatching on absolute king jump precedence
                precedence_dispatch(p, moves, rules::is_absolute_king_precedence<Rules>());
        }

private:
        // template aliases

        template<typename Position, typename Vector>
        struct KingJumps
        {
                typedef impl::copy<Color, material::king, select::jumps, Position, Vector> type;
        };

        template<typename Position, typename Vector>
        struct PawnJumps
        {
                typedef impl::copy<Color, material::pawn, select::jumps, Position, Vector> type;
        };

        // overload for no absolute king jump precedence
        template<typename Position, typename Vector>
        void precedence_dispatch(Position const& p, Vector& moves, std::false_type) const
        {
                Propagate<select::jumps, Position> propagate(p);
                typename KingJumps<Position, Vector>::type{propagate, moves}(p.material().kings(Color));
                typename PawnJumps<Position, Vector>::type{propagate, moves}(p.material().pawns(Color));
        }

        // overload for absolute king jump precedence
        template<typename Position, typename Vector>
        void precedence_dispatch(Position const& p, Vector& moves, std::true_type) const
        {
                Propagate<select::jumps, Position> propagate(p);
                typename KingJumps<Position, Vector>::type{propagate, moves}(p.material().kings(Color));
                if (moves.empty())
                        typename PawnJumps<Position, Vector>::type{propagate, moves}(p.material().pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
