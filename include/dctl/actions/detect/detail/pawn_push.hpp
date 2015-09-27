#pragma once
#include <dctl/board/angle.hpp>                               // left_up, right_up
#include <dctl/color.hpp>                               // Player
#include <dctl/piece.hpp>                               // pawn
#include <dctl/actions/detect/detail/primary_fwd.hpp> // Detect (primary template)
#include <dctl/actions/select/push.hpp>               // push

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/utility/type_traits.hpp>                         // board_t, set_t
#include <dctl/board/wave/patterns.hpp>                       // Sink
#include <type_traits>                                  // false_type

namespace dctl {
namespace actions {
namespace detail {

template<Color ToMove, class Reverse, class State>
class Detect<ToMove, Piece::pawn, select::push, Reverse, State>
{
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;
        State const& state;

public:
        explicit Detect(State const& s) noexcept
        :
                state{s}
        {}

        auto operator()() const noexcept
        {
                return detect(state.pieces(ToMove, Piece::pawn));
        }

        auto operator()(set_type const& active_pawns) const noexcept
        {
                return detect(active_pawns);
        }

private:
        auto detect(set_type const& active_pawns) const noexcept
        {
                if (active_pawns.none())
                        return false;

                return
                        parallelize<left_up (orientation)>(active_pawns) ||
                        parallelize<right_up(orientation)>(active_pawns)
                ;
        }

        template<int Direction>
        auto parallelize(set_type const& active_pawns) const noexcept
        {
                return Sink<board_type, Direction, short_ranged_tag>{}(
                        active_pawns, state.not_occupied()
                ).any();
        }
};

}       // namespace detail
}       // namespace actions
}       // namespace dctl
