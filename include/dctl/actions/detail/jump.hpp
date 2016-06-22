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
#include <dctl/color.hpp>                               // color
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/utility/static_vector.hpp>               // static_vector
#include <dctl/utility/type_traits.hpp>                 // rules_t, board_t
#include <cassert>                                      // assert
#include <dctl/rule_traits.hpp>
#include <type_traits>

namespace dctl {
namespace core {
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

template<color ToMove, class DuplicatesPolicy, class Reverse>
class Actions<ToMove, select::jump, DuplicatesPolicy, Reverse>
{
public:
        template<class State, class SequenceContainer>
        auto generate(State const& state, SequenceContainer& actions) const
        {
                using Builder = /*std::conditional_t<
                        std::is_same_v<Sequence, MoveCounter> && !DuplicatesPolicy{},
                        Counter<ToMove, DuplicatesPolicy, State>,*/
                        Builder<ToMove, DuplicatesPolicy, State, SequenceContainer>;
                //>;

                using KingJump = Generate<ToMove, piece::king, select::jump, Reverse, State, Builder>;
                using PawnJump = Generate<ToMove, piece::pawn, select::jump, Reverse, State, Builder>;

                Builder builder{state, actions};
                KingJump{builder}();
                PawnJump{builder}();
        }

        template<class State>
        auto count(State const& state) const
        {
                using counter_container = /* std::conditional_t<
                        is_trivial_precedence_v<rules_t<State>> && !DuplicatesPolicy{},
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
                using PawnJump = Detect<ToMove, piece::pawn, select::jump, Reverse, State>;
                using KingJump = Detect<ToMove, piece::king, select::jump, Reverse, State>;

                return PawnJump{}(state) || KingJump{}(state);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
