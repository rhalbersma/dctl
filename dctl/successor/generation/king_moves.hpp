#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/generation/generator_fwd.hpp>
#include <dctl/successor/select.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>
#include <dctl/board/iterator.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/stack.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for king moves generation
template<bool Color, typename Position>
struct generator<Color, Material::king, Moves, Position>
:
        // enforce reference semantics
        private boost::noncopyable
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef board::Compass<Color, Board> Compass;

        // representation

        Vector<Move>& moves_;

public:
        // structors

        /*explicit*/ generator(Vector<Move>& m)
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
                select_dispatch(active_kings, not_occupied, p, typename rules::traits<Rules>::king_move_orthogonality());
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
                        branch_dispatch(bit::first::equal(active_kings), not_occupied, Directions());
                        bit::first::clear(active_kings);
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
                find_dispatch<Direction>(from_sq, not_occupied, typename rules::traits<Rules>::king_range());
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
                        Increment<Board, Direction>()(dest_sq)
                )
                        moves_.push_back(Move::template create<Color>(from_sq ^ dest_sq));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
