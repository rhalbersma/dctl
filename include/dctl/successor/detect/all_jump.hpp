#pragma once
#include <type_traits>                                  // integral_constant, is_same, false_type, true_type
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/successor/detect/impl/king_jump.hpp>
#include <dctl/successor/detect/impl/pawn_jump.hpp>
#include <dctl/successor/propagate/jump.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jump.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/pieces/pieces.hpp>                       // all, king, pawn

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, class Range>
struct detect<Color, pieces::all, select::jump, Range>
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
                                rules::can_jump<Rules, pieces::pawn, pieces::king>::value
                        >()
                );
        }

private:
        // the existence of pawn jumps is independent of Range,
        // but we always use rules::range::distance_1 to avoid template bloat
        template<class Position>
        using PawnJumps = impl::detect<Color, pieces::pawn, select::jump, Position, rules::range::distance_1>;

        template<class Position>
        using KingJumps = impl::detect<Color, pieces::king, select::jump, Position, Range>;

        // overload for piece jump detection
        template<class Position>
        bool combined_dispatch(Position const& p, std::true_type) const
        {
                Propagate<select::jump, Position> propagate(p);
                return PawnJumps<Position>{propagate}(p.pieces(Color));
        }

        // overload for separate king and pawn jump detection
        template<class Position>
        bool combined_dispatch(Position const& p, std::false_type) const
        {
                Propagate<select::jump, Position> propagate(p);

                // speculate #pawns > #kings so that the logical OR is more likely to short-circuit
                return
                        PawnJumps<Position>{propagate}(p.pawns(Color)) ||
                        KingJumps<Position>{propagate}(p.kings(Color))
                ;
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
