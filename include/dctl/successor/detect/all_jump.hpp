#pragma once
#include <dctl/successor/detect/primary_fwd.hpp>        // Detect (primary template)
#include <dctl/successor/detect/king_jump.hpp>          // Detect (king jump specialization)
#include <dctl/successor/detect/pawn_jump.hpp>          // Detect (pawn jump specialization)
#include <dctl/successor/propagate/jump.hpp>            // Propagate (jump specialization)
#include <dctl/successor/select/jump.hpp>               // jump
#include <dctl/rules/traits.hpp>                        // traits
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn
#include <type_traits>                                  // integral_constant, is_same, false_type, true_type

namespace dctl {
namespace successor {

template<bool Color, class Range>
struct Detect<Color, pieces::all, select::jump, Range>
{
public:
        template<class Position>
        bool operator()(Position const& p) const
        {
                using Rules = typename Position::rules_type;

                // tag dispatching on piece jump detection
                // kings and pawns need to jump identically: i.e. have the same
                // (a) range, (b) jump directions, and (c) jump targets
                return combined_dispatch(
                        p,
                        std::integral_constant<
                                bool,
                                std::is_same<
                                        Range,
                                        rules::range::distance_1
                                >::value &&
                                std::is_same< typename
                                        rules::directions::king_jump<Rules>, typename
                                        rules::directions::pawn_jump<Rules>
                                >::value &&
                                is_pawn_jump_king_v<Rules>
                        >{}
                );
        }

private:
        // the existence of pawn jumps is independent of Range,
        // but we always use rules::range::distance_1 to avoid template bloat
        template<class Position>
        using PawnJump = Detect<Color, pieces::pawn, select::jump, Position, rules::range::distance_1>;

        template<class Position>
        using KingJump = Detect<Color, pieces::king, select::jump, Position, Range>;

        // overload for piece jump detection
        template<class Position>
        bool combined_dispatch(Position const& p, std::true_type) const
        {
                Propagate<select::jump, Position> propagate{p};
                return PawnJump<Position>{propagate}(p.pieces(Color));
        }

        // overload for separate king and pawn jump detection
        template<class Position>
        bool combined_dispatch(Position const& p, std::false_type) const
        {
                Propagate<select::jump, Position> propagate{p};

                // EFFICIENCY: logical instead of bitwise OR to enable short-circuiting
                // SPECULATIE: #pawns > #kings for earliest possible short-circuiting
                return
                        PawnJump<Position>{propagate}(p.pawns(Color)) ||
                        KingJump<Position>{propagate}(p.kings(Color))
                ;
        }
};

}       // namespace successor
}       // namespace dctl
