#pragma once
#include <dctl/actions/detail/count_primary_fwd.hpp>    // count (primary template)
#include <dctl/actions/detail/count_king_push.hpp>      // count (king push specialization)
#include <dctl/actions/detail/count_pawn_push.hpp>      // count (pawn push specialization)
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // detect (primary template)
#include <dctl/actions/detail/detect_king_push.hpp>     // detect (king push specialization)
#include <dctl/actions/detail/detect_pawn_push.hpp>     // detect (pawn push specialization)
#include <dctl/actions/detail/generate_primary_fwd.hpp> // generate (primary template)
#include <dctl/actions/detail/generate_king_push.hpp>   // generate (king push specialization)
#include <dctl/actions/detail/generate_pawn_push.hpp>   // generate (pawn push specialization)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/color_piece.hpp>                         // color, color_, kings_, pawn_

namespace dctl {
namespace detail {

template<color Side, class DuplicatesPolicy, class Reverse>
class Actions<color_<Side>, select::push, DuplicatesPolicy, Reverse>
{
        using to_move_ = color_<Side>;
public:
        template<class State, class SequenceContainer>
        auto generate(State const& s, SequenceContainer& a) const
        {
                using king_push = detail::generate<to_move_, kings_, select::push, Reverse, State, SequenceContainer>;
                using pawn_push = detail::generate<to_move_, pawns_, select::push, Reverse, State, SequenceContainer>;

                king_push{a}(s);
                pawn_push{a}(s);
        }

        template<class State>
        auto count(State const& s) const noexcept
        {
                using king_push = detail::count<to_move_, kings_, select::push, Reverse, State>;
                using pawn_push = detail::count<to_move_, pawns_, select::push, Reverse, State>;

                return king_push{}(s) + pawn_push{}(s);
        }

        template<class State>
        auto detect(State const& s) const noexcept
        {
                using pawn_push = detail::detect<to_move_, pawns_, select::push, Reverse, State>;
                using king_push = detail::detect<to_move_, kings_, select::push, Reverse, State>;

                return pawn_push{}(s) || king_push{}(s);
        }
};

}       // namespace detail
}       // namespace dctl
