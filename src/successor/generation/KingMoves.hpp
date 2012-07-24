#pragma once
#include <boost/config.hpp>             // BOOST_MPL_ASSERT
#include <boost/mpl/bool_fwd.hpp>       // false_, true_
#include "../Driver_fwd.hpp"
#include "../Result.hpp"
#include "../Select.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Shift.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../rules/Enum.hpp"
#include "../../utility/IntegerTypes.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for king moves generation
template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::king, select::Moves, generation, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef angle::Compass<Color, Board> Compass;

public:
        template<typename Position>
        static void generate(Position const& p, Stack& moves)
        {
                select(unrestricted_kings(p, Color), not_occupied(p), moves);
        }

private:
        static void select(BitBoard active_kings, BitBoard not_occupied, Stack& moves)
        {
                serialize(active_kings, not_occupied, moves);
        }

        static void serialize(BitBoard active_kings, BitBoard not_occupied, Stack& moves)
        {
                // tag dispatching on restrictions on consecutive moves with the same king
                serialize_dispatch(active_kings, not_occupied, moves, typename Rules::is_restricted_same_king_moves());
        }

        // overload for unrestricted consecutive moves with the same king
        static void serialize_dispatch(BitBoard active_kings, BitBoard not_occupied, Stack& moves, boost::mpl::false_)
        {
                // loop cannot be empty because all active kings detected during
                // Dispatcher<...>::select are unrestricted to move
                BOOST_ASSERT(!bit::is_zero(active_kings));
                do {
                        branch(bit::get_first(active_kings), not_occupied, moves);
                        bit::clear_first(active_kings);
                } while (active_kings);
        }

        // overload for restricted consecutive moves with the same king
        static void serialize_dispatch(BitBoard active_kings, BitBoard not_occupied, Stack& moves, boost::mpl::true_)
        {
                // loop could be empty if the single active king detected during
                // Dispatcher<...>::select is restricted from moving
                while (active_kings) {
                        branch(bit::get_first(active_kings), not_occupied, moves);
                        bit::clear_first(active_kings);
                }
        }

        static void branch(BitIndex from_sq, BitBoard not_occupied, Stack& moves)
        {
                find<typename Compass::left_down >(from_sq, not_occupied, moves);
                find<typename Compass::right_down>(from_sq, not_occupied, moves);
                find<typename Compass::left_up   >(from_sq, not_occupied, moves);
                find<typename Compass::right_up  >(from_sq, not_occupied, moves);
        }

        template<typename Direction>
        static void find(BitIndex from_sq, BitBoard not_occupied, Stack& moves)
        {
                // tag dispatching on king range
                return find_dispatch<Direction>(from_sq, not_occupied, moves, typename Rules::king_range());
        }

        // overload for short ranged kings
        template<typename Direction>
        static void find_dispatch(BitIndex from_sq, BitBoard not_occupied, Stack& moves, rules::range::distance_1)
        {
                if (auto const dest_sq = Board::next<Direction>(from_sq) & not_occupied)
                        moves.push_back(Move::create<Color>(from_sq ^ dest_sq));
        }

        // overload for long ranged kings
        template<typename Direction>
        static void find_dispatch(BitIndex from_sq, BitBoard not_occupied, Stack& moves, rules::range::distance_N)
        {
                for (
                        auto dest_sq = Board::next<Direction>(from_sq);
                        bit::is_element(dest_sq, not_occupied);
                        Board::advance<Direction>(dest_sq)
                )
                        moves.push_back(Move::create<Color>(from_sq ^ dest_sq));
        }
};

}       // namespace successor
}       // namespace dctl
