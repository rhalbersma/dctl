#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/successor/detect/king_jump.hpp>          // Detect (king jump specialization)
#include <dctl/successor/detect/pawn_jump.hpp>          // Detect (pawn jump specialization)
#include <dctl/successor/tracker.hpp>            // Propagate (jump specialization)
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/rule_traits.hpp>                         // traits
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn
#include <dctl/type_traits.hpp>                         // rules_type_t
#include <type_traits>                                  // integral_constant, is_same, false_type, true_type

namespace dctl {
namespace successor {

template<bool Color, class Range>
class Detect<Color, pieces::all, select::jump, Range>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using Rules = rules_type_t<Position>;

                // tag dispatching on piece jump detection
                // kings and pawns need to jump identically: i.e. have the same
                // (a) range, (b) jump directions, and (c) jump targets
                return combined_dispatch(
                        p,
                        std::integral_constant<
                                bool,
                                std::is_same<
                                        Range,
                                        std::false_type
                                >::value &&
                                is_backward_pawn_jump_v<Rules> &&
                                is_pawn_jump_king_v<Rules>
                        >{}
                );
        }

private:
        // the existence of pawn jumps is independent of Range,
        // but we always use short ranged movement to avoid template bloat
        template<class Position>
        using PawnJump = Detect<Color, pieces::pawn, select::jump, Position, std::false_type>;

        template<class Position>
        using KingJump = Detect<Color, pieces::king, select::jump, Position, Range>;

        // piece jump detection
        template<class Position>
        auto combined_dispatch(Position const& p, std::true_type) const
        {
                Propagate<Color, Position> propagate{p};
                return PawnJump<Position>{propagate}(p.pieces(Color));
        }

        // separate king and pawn jump detection
        template<class Position>
        auto combined_dispatch(Position const& p, std::false_type) const
        {
                Propagate<Color, Position> propagate{p};

                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
                // SPECULATE: #pawns > #kings for earliest possible short-circuiting
                return
                        PawnJump<Position>{propagate}(p.pawns(Color)) ||
                        KingJump<Position>{propagate}(p.kings(Color))
                ;
        }
};

}       // namespace successor
}       // namespace dctl
