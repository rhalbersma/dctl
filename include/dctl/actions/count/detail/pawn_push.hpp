#pragma once
#include <dctl/board/angle.hpp>                               // left_up, right_up
#include <dctl/color.hpp>                               // Player
#include <dctl/piece.hpp>                               // pawn
#include <dctl/actions/count/detail/primary_fwd.hpp>  // Count (primary template)
#include <dctl/actions/select/push.hpp>               // push

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/utility/type_traits.hpp>                         // board_t, set_t
#include <dctl/board/wave/patterns.hpp>                       // Sink
#include <xstd/cstddef.hpp>                             // _z
#include <type_traits>                                  // false_type

namespace dctl {
namespace actions {
namespace detail {

template<Color ToMove, bool Reverse, class State>
class Count<ToMove, Piece::pawn, select::push, Reverse, State>
{
        using board_type = board_t<State>;
        using   set_type =   set_t<State>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse>;
        State const& state;

public:
        explicit Count(State const& s)
        :
                state{s}
        {}

        auto operator()() const
        {
                return count(state.pieces(ToMove, Piece::pawn));
        }

        auto operator()(set_type const& active_pawns) const
        {
                return count(active_pawns);
        }

private:
        auto count(set_type const& active_pawns) const
        {
                using namespace xstd::support_literals;

                if (active_pawns.none())
                        return 0_z;

                return
                        parallelize<left_up (orientation)>(active_pawns) +
                        parallelize<right_up(orientation)>(active_pawns)
                ;
        }

        template<int Direction>
        auto parallelize(set_type const& active_pawns) const
        {
                return Sink<board_type, Direction, short_ranged_tag>{}(
                        active_pawns, state.not_occupied()
                ).count();
        }
};

}       // namespace detail
}       // namespace actions
}       // namespace dctl
