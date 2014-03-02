#pragma once
#include <type_traits>                                  // false_type, true_type
#include <dctl/successor/generate/primary_fwd.hpp>      // generate (primary template)
#include <dctl/successor/generate/impl/king_jump.hpp>  // generate (king jumps specialization)
#include <dctl/successor/generate/impl/pawn_jump.hpp>  // generate (pawn jumps specialization)
#include <dctl/successor/propagate/jump.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jump.hpp>              // jumps
#include <dctl/rules/traits.hpp>                        // traits
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for piece jumps
template<bool Color>
struct generate<Color, pieces::all, select::jump>
{
public:
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                using Rules = typename Position::rules_type;

                // tag dispatching on absolute king jump precedence
                precedence_dispatch(p, moves, rules::precedence::is_absolute_king<Rules>{});
        }

private:
        template<class Position, class Sequence>
        using KingJumps = impl::generate<Color, pieces::king, select::jump, Position, Sequence>;

        template<class Position, class Sequence>
        using PawnJumps = impl::generate<Color, pieces::pawn, select::jump, Position, Sequence>;

        // overload for no absolute king jump precedence
        template<class Position, class Sequence>
        void precedence_dispatch(Position const& p, Sequence& moves, std::false_type) const
        {
                Propagate<select::jump, Position> propagate{p};
                KingJumps<Position, Sequence>{propagate, moves}(p.kings(Color));
                PawnJumps<Position, Sequence>{propagate, moves}(p.pawns(Color));
        }

        // overload for absolute king jump precedence
        template<class Position, class Sequence>
        void precedence_dispatch(Position const& p, Sequence& moves, std::true_type) const
        {
                Propagate<select::jump, Position> propagate{p};
                KingJumps<Position, Sequence>{propagate, moves}(p.kings(Color));
                if (moves.empty())
                        PawnJumps<Position, Sequence>{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
