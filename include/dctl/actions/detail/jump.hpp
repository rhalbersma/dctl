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
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/utility/static_vector.hpp>               // static_vector
#include <dctl/utility/type_traits.hpp>                 // rules_t, board_t
#include <cassert>                                      // assert

namespace dctl {
namespace core {
namespace detail {

template<Color ToMove, class DropDuplicates, class Reverse>
class Actions<ToMove, select::jump, DropDuplicates, Reverse>
{
public:
        template<class State, class Sequence>
        auto generate(State const& state, Sequence& actions) const
        {
                using Builder = Builder<ToMove, DropDuplicates, State>;
                using KingJump = Generate<ToMove, Piece::king, select::jump, Reverse, Builder, Sequence>;
                using PawnJump = Generate<ToMove, Piece::pawn, select::jump, Reverse, Builder, Sequence>;

                Builder builder{state};
                KingJump{builder, actions}(state.pieces(ToMove, Piece::king));
                PawnJump{builder, actions}(state.pieces(ToMove, Piece::pawn));
        }

        template<class State>
        auto count(State const& state) const
        {
                static_vector<Action<rules_t<State>, board_t<State>>> actions;
                generate(state, actions);
                return actions.size();
        }

        template<class State>
        auto detect(State const& state) const noexcept
        {
                using Builder = Builder<ToMove, std::true_type, State>;
                using PawnJump = Detect<ToMove, Piece::pawn, select::jump, Reverse, Builder>;
                using KingJump = Detect<ToMove, Piece::king, select::jump, Reverse, Builder>;

                Builder builder{state};
                return
                        PawnJump{builder}(state.pieces(ToMove, Piece::pawn)) ||
                        KingJump{builder}(state.pieces(ToMove, Piece::king))
                ;
        }
};

}       // namespace detail
}       // namespace core
}       // namespace dctl
