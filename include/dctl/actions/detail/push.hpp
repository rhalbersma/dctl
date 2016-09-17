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
#include <dctl/piece.hpp>                               // king_type, pawn_type

namespace dctl {
namespace detail {

template<class Color, class DuplicatesPolicy, class Reverse>
class Actions<Color, select::push, DuplicatesPolicy, Reverse>
{
public:
        template<class State, class SequenceContainer>
        auto generate(State const& state, SequenceContainer& actions) const
        {
                using king_push = Generate<Color, king_type, select::push, Reverse, State, SequenceContainer>;
                using pawn_push = Generate<Color, pawn_type, select::push, Reverse, State, SequenceContainer>;

                king_push{actions}(state);
                pawn_push{actions}(state);
        }

        template<class State>
        auto count(State const& state) const noexcept
        {
                using king_push = Count<Color, king_type, select::push, Reverse, State>;
                using pawn_push = Count<Color, pawn_type, select::push, Reverse, State>;

                return king_push{}(state) + pawn_push{}(state);
        }

        template<class State>
        auto detect(State const& state) const noexcept
        {
                using pawn_push = Detect<Color, pawn_type, select::push, Reverse, State>;
                using king_push = Detect<Color, king_type, select::push, Reverse, State>;

                return pawn_push{}(state) || king_push{}(state);
        }
};

}       // namespace detail
}       // namespace dctl
