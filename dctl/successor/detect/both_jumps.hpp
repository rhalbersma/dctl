#pragma once
#include <type_traits>                  // integral_constant, is_same, false_type, true_type
#include <boost/mpl/logical.hpp>        // and_
#include <dctl/successor/detect/detector_fwd.hpp>
#include <dctl/successor/detect/king_jumps.hpp>
#include <dctl/successor/detect/pawn_jumps.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/successor/jumps.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position, typename Range>
struct detector<Color, Material::both, Jumps, Position, Range>
{
private:
        // typedefs

        // the existence of pawn jumps is independent of Range,
        // but we always use rules::range::distance_1 to avoid template bloat
        typedef detector<Color, Material::pawn, Jumps, Position, rules::range::distance_1> PawnJumps;

        typedef detector<Color, Material::king, Jumps, Position, Range> KingJumps;
        typedef typename Position::rules_type Rules;

public:
        bool operator()(Position const& p) const
        {
                return combined(p);
        }

private:
        bool combined(Position const& p) const
        {
                // tag dispatching on combined king and pawn jump detection
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
                                                rules::traits<Rules>::king_jump_directions, typename
                                                rules::traits<Rules>::pawn_jump_directions
                                        >, typename
                                        rules::traits<Rules>::is_pawns_jump_kings
                                >::value
                        >()
                );
        }

        // overload for combined king and pawn jump detection
        bool combined_dispatch(Position const& p, std::true_type) const
        {
                typedef PawnJumps PiecesJumps;

                auto const active_pieces = p.pieces(Color);
                return active_pieces? PiecesJumps().select(active_pieces, targets<Color>(p), not_occupied(p)) : false;
        }

        // overload for separate king and pawn jump detection
        bool combined_dispatch(Position const& p, std::false_type) const
        {
                // speculate #pawns > #kings so that the logical OR is more likely to short-circuit
                return PawnJumps()(p) || KingJumps()(p);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
