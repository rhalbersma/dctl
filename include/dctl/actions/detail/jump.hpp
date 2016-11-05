#pragma once
#include <dctl/action/action.hpp>                       // Action
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/detail/detect_primary_fwd.hpp>   // Detect (primary template)
#include <dctl/actions/detail/detect_king_jump.hpp>     // Detect (king jump specialization)
#include <dctl/actions/detail/detect_pawn_jump.hpp>     // Detect (pawn jump specialization)
#include <dctl/actions/detail/generate_primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/detail/generate_king_jump.hpp>   // Generate (king jump specialization)
#include <dctl/actions/detail/generate_pawn_jump.hpp>   // Generate (pawn jump specialization)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/color_piece.hpp>                         // color, color_, king_, pawn_
#include <dctl/utility/static_vector.hpp>               // static_vector
#include <dctl/utility/type_traits.hpp>                 // rules_t, board_t
#include <cassert>                                      // assert
#include <dctl/rule_traits.hpp>
#include <type_traits>

namespace dctl {
namespace detail {

class MoveCounter
{
        std::size_t n{};
public:
        auto& operator++()
        {
                ++n; return *this;
        }

        auto size() const
        {
                return n;
        }
};

template<color Side, class DuplicatesPolicy, class Reverse>
class Actions<color_<Side>, select::jump, DuplicatesPolicy, Reverse>
{
        using to_move_ = color_<Side>;
public:
        template<class State, class SequenceContainer>
        auto generate(State const& state, SequenceContainer& actions) const
        {
                using Builder = /*std::conditional_t<
                        std::is_same<Sequence, MoveCounter> && !DuplicatesPolicy{},
                        Counter<to_move_, DuplicatesPolicy, State>,*/
                        Builder<to_move_, DuplicatesPolicy, State, SequenceContainer>;
                //>;

                using king_jump = Generate<to_move_, king_, select::jump, Reverse, State, Builder>;
                using pawn_jump = Generate<to_move_, pawn_, select::jump, Reverse, State, Builder>;

                Builder builder{state, actions};
                king_jump{builder}();
                pawn_jump{builder}();
        }

        template<class State>
        auto count(State const& state) const
        {
                using counter_container = /* std::conditional_t<
                        !is_nontrivial_precedence_v<rules_t<State>> && !DuplicatesPolicy{},
                        MoveCounter,*/
                        static_vector<action<rules_t<State>, board_t<State>>>;
                //>;

                counter_container actions;
                generate(state, actions);
                return actions.size();
        }

        template<class State>
        auto detect(State const& state) const noexcept
        {
                using pawn_jump = Detect<to_move_, pawn_, select::jump, Reverse, State>;
                using king_jump = Detect<to_move_, king_, select::jump, Reverse, State>;

                return pawn_jump{}(state) || king_jump{}(state);
        }
};

}       // namespace detail
}       // namespace dctl
