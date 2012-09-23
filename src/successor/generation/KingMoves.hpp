#pragma once
#include <functional>                   // function
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/mpl/bool.hpp>           // false_, true_
#include "Generator_fwd.hpp"
#include "../Select.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Shift.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../rules/Enum.hpp"
#include "../../rules/Traits.hpp"
#include "../../utility/IntegerTypes.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king moves generation
template<bool Color, typename Position>
struct generator<Color, Material::king, Moves, Position>
:
        public std::function<void(Position const&, Stack&)>
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef angle::Compass<Color, Board> Compass;

public:
        void operator()(Position const& p, Stack& moves) const
        {
                if (auto const active_kings = moveable_kings(p, Color))
                        select(active_kings, not_occupied(p), moves);
        }

private:
        void select(BitBoard active_kings, BitBoard not_occupied, Stack& moves) const
        {
                serialize(active_kings, not_occupied, moves);
        }

        void serialize(BitBoard active_kings, BitBoard not_occupied, Stack& moves) const
        {
                BOOST_ASSERT(!bit::is_zero(active_kings));
                do {
                        branch(bit::get_first(active_kings), not_occupied, moves);
                        bit::clear_first(active_kings);
                } while (active_kings);
        }

        void branch(BitIndex from_sq, BitBoard not_occupied, Stack& moves) const
        {
                find<typename Compass::left_down >(from_sq, not_occupied, moves);
                find<typename Compass::right_down>(from_sq, not_occupied, moves);
                find<typename Compass::left_up   >(from_sq, not_occupied, moves);
                find<typename Compass::right_up  >(from_sq, not_occupied, moves);
        }

        template<typename Direction>
        void find(BitIndex from_sq, BitBoard not_occupied, Stack& moves) const
        {
                // tag dispatching on king range
                return find_dispatch<Direction>(from_sq, not_occupied, moves, typename Rules::king_range());
        }

        // overload for short ranged kings
        template<typename Direction>
        void find_dispatch(BitIndex from_sq, BitBoard not_occupied, Stack& moves, rules::range::distance_1) const
        {
                if (auto const dest_sq = Board::next<Direction>(from_sq) & not_occupied)
                        moves.push_back(Move::create<Color>(from_sq ^ dest_sq));
        }

        // overload for long ranged kings
        template<typename Direction>
        void find_dispatch(BitIndex from_sq, BitBoard not_occupied, Stack& moves, rules::range::distance_N) const
        {
                for (
                        auto dest_sq = Board::next<Direction>(from_sq);
                        bit::is_element(dest_sq, not_occupied);
                        Board::advance<Direction>(dest_sq)
                )
                        moves.push_back(Move::create<Color>(from_sq ^ dest_sq));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
