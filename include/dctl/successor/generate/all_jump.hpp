#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/king_jump.hpp>        // Generate (king jump specialization)
#include <dctl/successor/generate/pawn_jump.hpp>        // Generate (pawn jump specialization)
#include <dctl/successor/propagate/jump.hpp>            // Propagate (jump specialization)
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/rules/traits.hpp>                        // traits
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn
#include <type_traits>                                  // false_type, true_type

namespace dctl {
namespace successor {

// partial specialization for piece jumps
template<bool Color>
struct Generate<Color, pieces::all, select::jump>
{
public:
        template<class Position, class Sequence>
        void operator()(Position const& p, Sequence& moves) const
        {
                using Rules = typename Position::rules_type;

                // tag dispatching on absolute king jump precedence
                precedence_dispatch(p, moves, rules::is_absolute_king_jump_precedence_t<Rules>{});
        }

private:
        template<class Position, class Sequence>
        using KingJump = Generate<Color, pieces::king, select::jump, Position, Sequence>;

        template<class Position, class Sequence>
        using PawnJump = Generate<Color, pieces::pawn, select::jump, Position, Sequence>;

        // overload for no absolute king jump precedence
        template<class Position, class Sequence>
        void precedence_dispatch(Position const& p, Sequence& moves, std::false_type) const
        {
                auto propagate = Propagate<select::jump, Position>{p};
                KingJump<Position, Sequence>{propagate, moves}(p.kings(Color));
                PawnJump<Position, Sequence>{propagate, moves}(p.pawns(Color));
        }

        // overload for absolute king jump precedence
        template<class Position, class Sequence>
        void precedence_dispatch(Position const& p, Sequence& moves, std::true_type) const
        {
                auto propagate = Propagate<select::jump, Position>{p};
                KingJump<Position, Sequence>{propagate, moves}(p.kings(Color));
                if (moves.empty())
                        PawnJump<Position, Sequence>{propagate, moves}(p.pawns(Color));
        }
};

}       // namespace successor
}       // namespace dctl
