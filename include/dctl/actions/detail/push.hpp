#pragma once
#include <dctl/actions/detail/count_primary_fwd.hpp>    // Count (primary template)
#include <dctl/actions/detail/count_king_push.hpp>      // Count (king push specialization)
#include <dctl/actions/detail/count_pawn_push.hpp>      // Count (pawn push specialization)
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/detail/detect_king_push.hpp>     // Detect (king push specialization)
#include <dctl/actions/detail/detect_pawn_push.hpp>     // Detect (pawn push specialization)
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/detail/generate_king_push.hpp>   // Generate (king push specialization)
#include <dctl/actions/detail/generate_pawn_push.hpp>   // Generate (pawn push specialization)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king, pawn

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class DuplicatesPolicy, class Reverse>
class Actions<ToMove, select::push, DuplicatesPolicy, Reverse>
{
public:
        template<class State, class Sequence>
        auto generate(State const& state, Sequence& actions) const
        {
                using KingPush = Generate<ToMove, Piece::king, select::push, Reverse, State, Sequence>;
                using PawnPush = Generate<ToMove, Piece::pawn, select::push, Reverse, State, Sequence>;

                KingPush{state, actions}();
                PawnPush{state, actions}();
        }

        template<class State>
        auto count(State const& state) const noexcept
        {
                using KingPush = Count<ToMove, Piece::king, select::push, Reverse, State>;
                using PawnPush = Count<ToMove, Piece::pawn, select::push, Reverse, State>;

                return KingPush{}(state) + PawnPush{}(state);
        }

        template<class State>
        auto detect(State const& state) const noexcept
        {
                using PawnPush = Detect<ToMove, Piece::pawn, select::push, Reverse, State>;
                using KingPush = Detect<ToMove, Piece::king, select::push, Reverse, State>;

                return PawnPush{}(state) || KingPush{}(state);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
