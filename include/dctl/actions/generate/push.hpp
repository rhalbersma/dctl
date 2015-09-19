#pragma once
#include <dctl/actions/generate/primary_fwd.hpp>        // Generate (primary template)
#include <dctl/actions/generate/detail/king_push.hpp>   // Generate (king push specialization)
#include <dctl/actions/generate/detail/pawn_push.hpp>   // Generate (pawn push specialization)
#include <dctl/actions/select/push.hpp>                 // push
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king, pawn

namespace dctl {
namespace actions {

template<Color ToMove, class Unique, class Reverse>
class Generate<ToMove, select::push, Unique, Reverse>
{
public:
        template<class State, class Sequence>
        auto operator()(State const& s, Sequence& moves) const
        {
                using KingPush = detail::Generate<ToMove, Piece::king, select::push, Reverse, State, Sequence>;
                using PawnPush = detail::Generate<ToMove, Piece::pawn, select::push, Reverse, State, Sequence>;

                KingPush{s, moves}();
                PawnPush{s, moves}();
        }
};

}       // namespace actions
}       // namespace dctl
