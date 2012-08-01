#pragma once
#include <type_traits>                  // is_same
#include <boost/mpl/bool_fwd.hpp>       // false_, true_
#include <boost/mpl/logical.hpp>        // and_
#include "Detector_fwd.hpp"
#include "KingJumps.hpp"
#include "PawnJumps.hpp"
#include "../Select.hpp"
#include "../../capture/State.hpp"
#include "../../node/Material.hpp"
#include "../../rules/Enum.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool Color, typename Position, typename Range>
struct detector<Color, Material::both, Jumps, Position, Range>
{
private:
        // typedefs

        // the implementation of pawn jump detection is independent of Range, 
        // but we explicitly pass rules::range::distance_1 here to avoid code bloat
        typedef detector<Color, Material::pawn, Jumps, Position, rules::range::distance_1> PawnJumps;

        typedef detector<Color, Material::king, Jumps, Position, Range> KingJumps;
        typedef typename Position::rules_type Rules;

public:
        bool operator()(Position const& p)
        {
                return combined(p);
        }

private:
        bool combined(Position const& p)
        {
                // tag dispatching on combined king and pawn jump detection
                // kings and pawns need to jump identically: i.e. have the same 
                // (a) range, (b) jump directions, and (c) jump targets
                return combined_dispatch(
                        p,
                        boost::mpl::and_<
                                std::is_same< 
                                        Range, 
                                        rules::range::distance_1
                                >,
                                std::is_same< typename 
                                        Rules::king_jump_directions, typename 
                                        Rules::pawn_jump_directions
                                >, typename 
                                Rules::is_pawns_jump_kings
                        >()
                );
        }

        // overload for combined king and pawn jump generation
        bool combined_dispatch(Position const& p, boost::mpl::true_)
        {
                if (auto const active_pieces = p.pieces(Color))
                        return PawnJumps().select(active_pieces, targets<Color>(p), not_occupied(p));
                else
                        return false;
        }

        // overload for separate king and pawn jump generation
        bool combined_dispatch(Position const& p, boost::mpl::false_)
        {
                // speculate #pawns > #kings so that the || is likely to short-circuit
                return (
                        PawnJumps()(p) ||
                        KingJumps()(p)
                );
        }        
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
