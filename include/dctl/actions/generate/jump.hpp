#pragma once
#include <dctl/actions/detail/builder.hpp>            // Builder
#include <dctl/actions/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/actions/generate/detail/king_jump.hpp> // Generate (king jump specialization)
#include <dctl/actions/generate/detail/pawn_jump.hpp> // Generate (pawn jump specialization)
#include <dctl/actions/select/jump.hpp>               // jump
#include <dctl/color.hpp>                             // Color
#include <dctl/piece.hpp>                             // king, pawn

namespace dctl {
namespace actions {

template<Color ToMove, class Unique, class Reverse>
class Generate<ToMove, select::jump, Unique, Reverse>
{
public:
        template<class State, class Sequence>
        auto operator()(State const& s, Sequence& moves) const
        {
                using Builder = detail::Builder<ToMove, Unique, State>;
                using KingJump = detail::Generate<ToMove, Piece::king, select::jump, Reverse, Builder, Sequence>;
                using PawnJump = detail::Generate<ToMove, Piece::pawn, select::jump, Reverse, Builder, Sequence>;

                Builder builder{s};
                KingJump{builder, moves}(s.pieces(ToMove, Piece::king));
                PawnJump{builder, moves}(s.pieces(ToMove, Piece::pawn));
        }
};

}       // namespace actions
}       // namespace dctl
