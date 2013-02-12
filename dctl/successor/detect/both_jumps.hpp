#pragma once
#include <type_traits>                  // integral_constant, is_same, false_type, true_type
#include <boost/mpl/logical.hpp>        // and_
#include <dctl/successor/detect/primary_fwd.hpp>
#include <dctl/successor/detect/impl/king_jumps.hpp>
#include <dctl/successor/detect/impl/pawn_jumps.hpp>
#include <dctl/successor/select/jumps.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/rules/traits.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Range>
struct detect<Color, Material::both, select::jumps, Range>
{
public:
        template<typename Position>
        bool operator()(Position const& p) const
        {
                typedef typename Position::rules_type Rules;

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

private:
        // template aliases

        // the existence of pawn jumps is independent of Range,
        // but we always use rules::range::distance_1 to avoid template bloat
        template<typename Position>
        using PawnJumps = impl::detect<Color, Material::pawn, select::jumps, Position, rules::range::distance_1>;

        template<typename Position>
        using KingJumps = impl::detect<Color, Material::king, select::jumps, Position, Range>;

        // overload for combined king and pawn jump detection
        template<typename Position>
        bool combined_dispatch(Position const& p, std::true_type) const
        {
                Propagate<select::jumps, Position> propagate(p);
                return PawnJumps<Position>{propagate}(p.pieces(Color));
        }

        // overload for separate king and pawn jump detection
        template<typename Position>
        bool combined_dispatch(Position const& p, std::false_type) const
        {
                Propagate<select::jumps, Position> propagate(p);

                // speculate #pawns > #kings so that the logical OR is more likely to short-circuit
                return PawnJumps<Position>{propagate}(p.pawns(Color)) || KingJumps<Position>{propagate}(p.kings(Color));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
