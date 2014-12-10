#pragma once
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PieceKingType, PiecePawnType
#include <dctl/successor/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/successor/detect/king_jump.hpp>          // Detect (king jump specialization)
#include <dctl/successor/detect/pawn_jump.hpp>          // Detect (pawn jump specialization)
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/successor/tracker.hpp>                   // Tracker
#include <dctl/rule_traits.hpp>                         // is_backward_pawn_jump_v, is_pawn_jump_king_v
#include <dctl/type_traits.hpp>                         // rules_type_t
#include <type_traits>                                  // integral_constant, is_same, false_type, true_type

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse, class Range>
class Detect<ToMove, IsReverse, select::jump, Range>
{
public:
        template<class Position>
        auto operator()(Position const& p) const
        {
                using rules_type = rules_type_t<Position>;

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
                                is_backward_pawn_jump_v<rules_type> &&
                                is_pawn_jump_king_v<rules_type>
                        >{}
                );
        }

private:
        // the existence of pawn jumps is independent of Range,
        // but we always use short ranged movement to avoid template bloat
        template<class Position>
        using PawnJump = Detect<ToMove, IsReverse, PiecePawnType, select::jump, Position, std::false_type>;

        template<class Position>
        using KingJump = Detect<ToMove, IsReverse, PieceKingType, select::jump, Position, Range>;

        // piece jump detection
        template<class Position>
        auto combined_dispatch(Position const& p, std::true_type) const
        {
                Tracker<ToMove, Position> tracker{p};
                return PawnJump<Position>{tracker}(p.pieces(ToMove));
        }

        // separate king and pawn jump detection
        template<class Position>
        auto combined_dispatch(Position const& p, std::false_type) const
        {
                Tracker<ToMove, Position> tracker{p};

                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
                // SPECULATE: #pawns > #kings for earliest possible short-circuiting
                return
                        PawnJump<Position>{tracker}(p.pawns(ToMove)) ||
                        KingJump<Position>{tracker}(p.kings(ToMove))
                ;
        }
};

}       // namespace successor
}       // namespace dctl
