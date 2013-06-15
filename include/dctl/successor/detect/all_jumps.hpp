#pragma once
#include <type_traits>                                  // integral_constant, is_same, false_type, true_type
#include <boost/mpl/logical.hpp>                        // and_
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/successor/detect/impl/king_jumps.hpp>
#include <dctl/successor/detect/impl/pawn_jumps.hpp>
#include <dctl/successor/propagate/jumps.hpp>           // Propagate (jumps specialization)
#include <dctl/successor/select/jumps.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/pieces/pieces.hpp>                // all, king, pawn

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, class Range>
struct detect<Color, pieces::all, select::jumps, Range>
{
public:
        template<class Position>
        bool operator()(Position const& p) const
        {
                typedef typename Position::rules_type Rules;

                // tag dispatching on piece jump detection
                // kings and pawns need to jump identically: i.e. have the same
                // (a) range, (b) jump directions, and (c) jump targets
                return combined_dispatch(
                        p,
                        std::integral_constant<
                                bool,
                                boost::mpl::and_<
                                        std::is_same<
                                                Range,
                                                rules::range::distance_1
                                        >,
                                        std::is_same< typename
                                                rules::directions::king_jump<Rules>, typename
                                                rules::directions::pawn_jump<Rules>
                                        >,
                                        rules::can_jump<Rules, pieces::pawn, pieces::king>
                                >::value
                        >()
                );
        }

private:
        // template aliases

        // the existence of pawn jumps is independent of Range,
        // but we always use rules::range::distance_1 to avoid template bloat
        template<class Position>
        struct PawnJumps
        {
                typedef impl::detect<Color, pieces::pawn, select::jumps, Position, rules::range::distance_1> type;
        };

        template<class Position>
        struct KingJumps
        {
                typedef impl::detect<Color, pieces::king, select::jumps, Position, Range> type;
        };

        // overload for piece jump detection
        template<class Position>
        bool combined_dispatch(Position const& p, std::true_type) const
        {
                Propagate<select::jumps, Position> propagate(p);
                return typename PawnJumps<Position>::type{propagate}(p.material().pieces(Color));
        }

        // overload for separate king and pawn jump detection
        template<class Position>
        bool combined_dispatch(Position const& p, std::false_type) const
        {
                Propagate<select::jumps, Position> propagate(p);

                // speculate #pawns > #kings so that the logical OR is more likely to short-circuit
                return typename PawnJumps<Position>::type{propagate}(p.material().pawns(Color)) || typename KingJumps<Position>::type{propagate}(p.material().kings(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
