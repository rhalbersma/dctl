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
#include <dctl/color_piece.hpp>                         // Color, color_constant, king_type, pawn_type

namespace dctl {
namespace detail {

template<Color Side, class DuplicatesPolicy, class Reverse>
class Actions<color_constant<Side>, select::push, DuplicatesPolicy, Reverse>
{
        using color_type = color_constant<Side>;
public:
        template<class State, class SequenceContainer>
        auto generate(State const& state, SequenceContainer& actions) const
        {
                using king_push = Generate<color_type, king_type, select::push, Reverse, State, SequenceContainer>;
                using pawn_push = Generate<color_type, pawn_type, select::push, Reverse, State, SequenceContainer>;

                king_push{actions}(state);
                pawn_push{actions}(state);
        }

        template<class State>
        auto count(State const& state) const noexcept
        {
                using king_push = Count<color_type, king_type, select::push, Reverse, State>;
                using pawn_push = Count<color_type, pawn_type, select::push, Reverse, State>;

                return king_push{}(state) + pawn_push{}(state);
        }

        template<class State>
        auto detect(State const& state) const noexcept
        {
                using pawn_push = Detect<color_type, pawn_type, select::push, Reverse, State>;
                using king_push = Detect<color_type, king_type, select::push, Reverse, State>;

                return pawn_push{}(state) || king_push{}(state);
        }
};

}       // namespace detail
}       // namespace dctl
