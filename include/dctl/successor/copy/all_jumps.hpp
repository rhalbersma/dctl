#pragma once
#include <type_traits>                                  // false_type, true_type
#include <dctl/successor/copy/primary_fwd.hpp>          // copy (primary template)
#include <dctl/successor/copy/impl/king_jumps.hpp>      // copy (king jumps specialization)
#include <dctl/successor/copy/impl/pawn_jumps.hpp>      // copy (pawn jumps specialization)
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>              // jumps
#include <dctl/rules/traits.hpp>                        // traits
#include <dctl/pieces/pieces.hpp>                  // all, king, pawn

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for piece jumps
template<bool Color>
struct copy<Color, pieces::all, select::jumps>
{
public:
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                typedef typename Position::rules_type Rules;

                // tag dispatching on absolute king jump precedence
                precedence_dispatch(p, moves, rules::precedence::is_absolute_king<Rules>());
        }

private:
        // TODO: use template aliases

        template<class Position, class Sequence>
        struct KingJumps
        {
                typedef impl::copy<Color, pieces::king, select::jumps, Position, Sequence> type;
        };

        template<class Position, class Sequence>
        struct PawnJumps
        {
                typedef impl::copy<Color, pieces::pawn, select::jumps, Position, Sequence> type;
        };

        // overload for no absolute king jump precedence
        template<class Position, class Sequence>
        void precedence_dispatch(Position const& p, Sequence& moves, std::false_type) const
        {
                Propagate<select::jumps, Position> propagate(p);
                typename KingJumps<Position, Sequence>::type{propagate, moves}(p.material().kings(Color));
                typename PawnJumps<Position, Sequence>::type{propagate, moves}(p.material().pawns(Color));
        }

        // overload for absolute king jump precedence
        template<class Position, class Sequence>
        void precedence_dispatch(Position const& p, Sequence& moves, std::true_type) const
        {
                Propagate<select::jumps, Position> propagate(p);
                typename KingJumps<Position, Sequence>::type{propagate, moves}(p.material().kings(Color));
                if (moves.empty())
                        typename PawnJumps<Position, Sequence>::type{propagate, moves}(p.material().pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
