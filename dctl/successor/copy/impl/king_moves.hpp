#pragma once
#include <boost/utility.hpp>                            // noncopyable
#include <dctl/successor/copy/impl/primary_fwd.hpp>     // copy (primary template)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
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
namespace impl {

// partial specialization for king moves generation
template<bool Color, typename Position>
struct copy<Color, Material::king, select::moves, Position>
:
        // enforce reference semantics
        private boost::noncopyable
{
private:
        // typedefs

        typedef typename Position::rules_type Rules;
        typedef typename Position::board_type Board;
        typedef board::Compass<Color, Board> Compass;
        typedef Propagate<select::moves, Position> State;

        // representation

        State const& propagate_;
        Vector<Move>& moves_;

public:
        // structors

        explicit copy(State const& p, Vector<Move>& m)
        :
                propagate_(p),
                moves_(m)
        {}

        // function call operators

        void operator()(BitBoard active_kings) const
        {
                serialize(active_kings);
        }

private:
        void serialize(BitBoard active_kings) const
        {
                while(active_kings) {
                        branch(bit::first::equal(active_kings));
                        bit::first::clear(active_kings);
                }
        }

        void branch(BitIndex from_sq) const
        {
                find<typename Compass::left_down >(from_sq);
                find<typename Compass::right_down>(from_sq);
                find<typename Compass::left_up   >(from_sq);
                find<typename Compass::right_up  >(from_sq);
        }

        template<typename Direction>
        void find(BitIndex from_sq) const
        {
                // tag dispatching on king range
                find_dispatch<Direction>(from_sq, typename rules::traits<Rules>::king_range());
        }

        // overload for short ranged kings
        template<typename Direction>
        void find_dispatch(BitIndex from_sq, rules::range::distance_1) const
        {
                if (auto const dest_sq = Next<Board, Direction>()(from_sq) & propagate_.path())
                        moves_.push_back(Move::template create<Color>(from_sq ^ dest_sq));
        }

        // overload for long ranged kings
        template<typename Direction>
        void find_dispatch(BitIndex from_sq, rules::range::distance_N) const
        {
                for (
                        auto dest_sq = Next<Board, Direction>()(from_sq);
                        bit::is_element(dest_sq, propagate_.path());
                        Increment<Board, Direction>()(dest_sq)
                )
                        moves_.push_back(Move::template create<Color>(from_sq ^ dest_sq));
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
