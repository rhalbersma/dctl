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
                        select(active_kings, not_occupied(p), moves, p);
        }

private:
        void select(BitBoard active_kings, BitBoard not_occupied, Stack& moves, Position const& p) const
        {
                // tag dispatching on king move orthogonality
                select_dispatch(active_kings, not_occupied, moves, p, typename Rules::king_move_orthogonality());
        }

        // overload for kings that move in the 4 diagonal directions
        void select_dispatch(BitBoard active_kings, BitBoard not_occupied, Stack& moves, Position const& /* p */, rules::orthogonality::none) const
        {
                serialize<rules::directions::diag>(active_kings, not_occupied, moves);
        }

        // overload for kings that move in the 8 diagonal and orthogonal directions 
        // if they belong to the side with a king majority, 
        // and that otherwise move in the 4 diagonal directions
        void select_dispatch(BitBoard active_kings, BitBoard not_occupied, Stack& moves, Position const& p, rules::orthogonality::relative) const
        {
                if (bit::count(p.kings(Color)) > bit::count(p.kings(!Color)))
                        serialize<rules::directions::all >(active_kings, not_occupied, moves);
                else
                        serialize<rules::directions::diag>(active_kings, not_occupied, moves);
        }

        // overload for kings that move in the 8 diagonal and orthogonal directions
        void select_dispatch(BitBoard active_kings, BitBoard not_occupied, Stack& moves, Position const& /* p */, rules::orthogonality::absolute) const
        {
                serialize<rules::directions::all>(active_kings, not_occupied, moves);
        }

        template<typename Directions>
        void serialize(BitBoard active_kings, BitBoard not_occupied, Stack& moves) const
        {
                BOOST_ASSERT(!bit::is_zero(active_kings));
                do {
                        branch_dispatch(bit::get_first(active_kings), not_occupied, moves, Directions());
                        bit::clear_first(active_kings);
                } while (active_kings);
        }

        // overload for kings that move in the 8 diagonal and orthogonal directions
        void branch_dispatch(BitIndex from_sq, BitBoard not_occupied, Stack& moves, rules::directions::all) const
        {
                branch_dispatch(from_sq, not_occupied, moves, rules::directions::diag());
                branch_dispatch(from_sq, not_occupied, moves, rules::directions::orth());
        }

        // overload for kings that move in the 4 diagonal directions
        void branch_dispatch(BitIndex from_sq, BitBoard not_occupied, Stack& moves, rules::directions::diag) const
        {
                find<typename Compass::left_down >(from_sq, not_occupied, moves);
                find<typename Compass::right_down>(from_sq, not_occupied, moves);
                find<typename Compass::left_up   >(from_sq, not_occupied, moves);
                find<typename Compass::right_up  >(from_sq, not_occupied, moves);
        }

        // overload for kings that move in the 4 orthogonal directions
        void branch_dispatch(BitIndex from_sq, BitBoard not_occupied, Stack& moves, rules::directions::orth) const
        {
                find<typename Compass::left >(from_sq, not_occupied, moves);
                find<typename Compass::right>(from_sq, not_occupied, moves);
                find<typename Compass::up   >(from_sq, not_occupied, moves);
                find<typename Compass::down >(from_sq, not_occupied, moves);
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
