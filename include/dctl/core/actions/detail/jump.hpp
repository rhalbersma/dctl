#pragma once
#include <dctl/core/action/action.hpp>                       // Action
#include <dctl/core/actions/detail/builder.hpp>              // builder
#include <dctl/core/actions/detail/detect_primary_fwd.hpp>   // detect (primary template)
#include <dctl/core/actions/detail/detect_king_jump.hpp>     // detect (king jump specialization)
#include <dctl/core/actions/detail/detect_pawn_jump.hpp>     // detect (pawn jump specialization)
#include <dctl/core/actions/detail/generate_primary_fwd.hpp> // generate (primary template)
#include <dctl/core/actions/detail/generate_king_jump.hpp>   // generate (king jump specialization)
#include <dctl/core/actions/detail/generate_pawn_jump.hpp>   // generate (pawn jump specialization)
#include <dctl/core/actions/select/jump.hpp>                 // jump
#include <dctl/core/state/color_piece.hpp>                         // color, color_, kings_, pawn_
#include <dctl/core/rules/traits.hpp>
#include <dctl/util/static_vector.hpp>               // static_vector
#include <dctl/util/type_traits.hpp>                 // rules_t, board_t
#include <cassert>                                      // assert
#include <type_traits>

namespace dctl {
namespace core {
namespace detail {

class MoveCounter
{
        int n{};
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
        auto generate(State const& s, SequenceContainer& a) const
        {
                using builder_type = /*std::conditional_t<
                        std::is_same_v<Sequence, MoveCounter> && !DuplicatesPolicy,
                        Counter<to_move_, DuplicatesPolicy, State>,*/
                        builder<to_move_, DuplicatesPolicy, State, SequenceContainer>;
                //>;

                using king_jump = detail::generate<to_move_, kings_, select::jump, Reverse, State, builder_type>;
                using pawn_jump = detail::generate<to_move_, pawns_, select::jump, Reverse, State, builder_type>;

                auto b = builder_type{s, a};
                king_jump{b}();
                pawn_jump{b}();
        }

        template<class State>
        auto count(State const& s) const
        {
                using counter_container = /* std::conditional_t<
                        is_trivial_precedence_v<rules_t<State>> && !DuplicatesPolicy{},
                        MoveCounter,*/
                        static_vector<action<rules_t<State>, board_t<State>>>;
                //>;

                counter_container a;
                generate(s, a);
                return static_cast<int>(a.size());
        }

        template<class State>
        auto detect(State const& s) const noexcept
        {
                using pawn_jump = detail::detect<to_move_, pawns_, select::jump, Reverse, State>;
                using king_jump = detail::detect<to_move_, kings_, select::jump, Reverse, State>;

                return pawn_jump{}(s) || king_jump{}(s);
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
