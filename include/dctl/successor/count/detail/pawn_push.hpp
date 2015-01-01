#pragma once
#include <dctl/angle.hpp>                               // left_up, right_up
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // pawn
#include <dctl/successor/count/detail/primary_fwd.hpp>  // Count (primary template)
#include <dctl/successor/select/push.hpp>               // push

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/type_traits.hpp>                         // board_type_t, set_type_t
#include <dctl/wave/patterns.hpp>                       // Sink
#include <xstd/cstddef.hpp>                             // _z
#include <type_traits>                                  // false_type

namespace dctl {
namespace successor {
namespace detail {

template<Color ToMove, bool IsReverse, class Position>
class Count<ToMove, Piece::pawn, select::push, IsReverse, Position>
{
        using board_type = board_type_t<Position>;
        using   set_type =   set_type_t<Position>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, IsReverse>;
        Position const& position;

public:
        explicit Count(Position const& p)
        :
                position{p}
        {}

        auto operator()() const
        {
                return count(position.pieces(ToMove, Piece::pawn));
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
                        active_pawns, position.not_occupied()
                ).count();
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
