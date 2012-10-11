#pragma once
#include <functional>                   // function
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/mpl/bool.hpp>           // false_, true_
#include <boost/utility.hpp>            // noncopyable
#include "Generator_fwd.hpp"
#include "../Select.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Shift.hpp"
#include "../../node/Material.hpp"
#include "../../node/Stack.hpp"
#include "../../node/UnaryProjections.hpp"
#include "../../rules/Enum.hpp"
#include "../../utility/IntegerTypes.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king moves generation
template<bool Color, typename Position>
struct generator<Color, Material::king, Moves, Position>
:
        // enforce reference semantics
        private boost::noncopyable,
        public std::function<void(Position const&)>
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef Compass<Color, Board> Compass;

        // representation

        Stack& moves_;

public:
        // structors

        explicit generator(Stack& m)
        :
                moves_(m)
        {}

        // function call operators

        void operator()(Position const& p) const
        {
                if (auto const active_kings = moveable_kings(p, Color))
                        select(active_kings, not_occupied(p), p);
        }

private:
        void select(BitBoard active_kings, BitBoard not_occupied, Position const& p) const
        {
                // tag dispatching on king move orthogonality
                select_dispatch(active_kings, not_occupied, p, typename Rules::king_move_orthogonality());
        }

        // overload for kings that move in the 4 diagonal directions
        void select_dispatch(BitBoard active_kings, BitBoard not_occupied, Position const& /* p */, rules::orthogonality::none) const
        {
                serialize<rules::directions::diag>(active_kings, not_occupied);
        }

        // overload for kings that move in the 8 diagonal and orthogonal directions 
        // if they belong to the side with a king majority, 
        // and that otherwise move in the 4 diagonal directions
        void select_dispatch(BitBoard active_kings, BitBoard not_occupied, Position const& p, rules::orthogonality::relative) const
        {
                if (bit::count(p.kings(Color)) > bit::count(p.kings(!Color)))
                        serialize<rules::directions::all >(active_kings, not_occupied);
                else
                        serialize<rules::directions::diag>(active_kings, not_occupied);
        }

        // overload for kings that move in the 8 diagonal and orthogonal directions
        void select_dispatch(BitBoard active_kings, BitBoard not_occupied, Position const& /* p */, rules::orthogonality::absolute) const
        {
                serialize<rules::directions::all>(active_kings, not_occupied);
        }

        template<typename Directions>
        void serialize(BitBoard active_kings, BitBoard not_occupied) const
        {
                BOOST_ASSERT(!bit::is_zero(active_kings));
                do {
                        branch_dispatch(bit::get_first(active_kings), not_occupied, Directions());
                        bit::clear_first(active_kings);
                } while (active_kings);
        }

        // overload for kings that move in the 8 diagonal and orthogonal directions
        void branch_dispatch(BitIndex from_sq, BitBoard not_occupied, rules::directions::all) const
        {
                branch_dispatch(from_sq, not_occupied, rules::directions::diag());
                branch_dispatch(from_sq, not_occupied, rules::directions::orth());
        }

        // overload for kings that move in the 4 diagonal directions
        void branch_dispatch(BitIndex from_sq, BitBoard not_occupied, rules::directions::diag) const
        {
                find<typename Compass::left_down >(from_sq, not_occupied);
                find<typename Compass::right_down>(from_sq, not_occupied);
                find<typename Compass::left_up   >(from_sq, not_occupied);
                find<typename Compass::right_up  >(from_sq, not_occupied);
        }

        // overload for kings that move in the 4 orthogonal directions
        void branch_dispatch(BitIndex from_sq, BitBoard not_occupied, rules::directions::orth) const
        {
                find<typename Compass::left >(from_sq, not_occupied);
                find<typename Compass::right>(from_sq, not_occupied);
                find<typename Compass::up   >(from_sq, not_occupied);
                find<typename Compass::down >(from_sq, not_occupied);
        }

        template<typename Direction>
        void find(BitIndex from_sq, BitBoard not_occupied) const
        {
                // tag dispatching on king range
                return find_dispatch<Direction>(from_sq, not_occupied, typename Rules::king_range());
        }

        // overload for short ranged kings
        template<typename Direction>
        void find_dispatch(BitIndex from_sq, BitBoard not_occupied, rules::range::distance_1) const
        {
                if (auto const dest_sq = Next<Board, Direction>()(from_sq) & not_occupied)
                        moves_.push_back(Move::template create<Color>(from_sq ^ dest_sq));
        }

        // overload for long ranged kings
        template<typename Direction>
        void find_dispatch(BitIndex from_sq, BitBoard not_occupied, rules::range::distance_N) const
        {
                for (
                        auto dest_sq = Next<Board, Direction>()(from_sq);
                        bit::is_element(dest_sq, not_occupied);
                        Advance<Board, Direction>()(dest_sq)
                )
                        moves_.push_back(Move::template create<Color>(from_sq ^ dest_sq));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
