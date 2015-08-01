#pragma once
#include <dctl/board/angle.hpp>                               // left_up, right_up
#include <dctl/color.hpp>                               // Player
#include <dctl/piece.hpp>                               // pawn
#include <dctl/actions/detect/detail/primary_fwd.hpp> // Detect (primary template)
#include <dctl/actions/select/push.hpp>               // push

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/utility/type_traits.hpp>                         // board_type_t, set_type_t
#include <dctl/board/wave/patterns.hpp>                       // Sink
#include <type_traits>                                  // false_type

namespace dctl {
namespace actions {
namespace detail {

template<Color ToMove, bool Reverse, class State>
class Detect<ToMove, Piece::pawn, select::push, Reverse, State>
{
        using board_type = board_type_t<State>;
        using   set_type =   set_type_t<State>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse>;
        State const& position;

public:
        explicit Detect(State const& p)
        :
                position{p}
        {}

        auto operator()() const
        {
                return detect(position.pieces(ToMove, Piece::pawn));
        }

        auto operator()(set_type const& active_pawns) const
        {
                return detect(active_pawns);
        }

private:
        auto detect(set_type const& active_pawns) const
        {
                if (active_pawns.none())
                        return false;

                return
                        parallelize<left_up (orientation)>(active_pawns) ||
                        parallelize<right_up(orientation)>(active_pawns)
                ;
        }

        template<int Direction>
        auto parallelize(set_type const& active_pawns) const
        {
                return Sink<board_type, Direction, short_ranged_tag>{}(
                        active_pawns, position.not_occupied()
                ).any();
        }
};

}       // namespace detail
}       // namespace actions
}       // namespace dctl
