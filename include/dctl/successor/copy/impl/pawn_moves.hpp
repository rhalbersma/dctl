#pragma once
#include <boost/utility.hpp>                            // noncopyable
#include <dctl/successor/copy/impl/primary_fwd.hpp>     // copy (primary template)
#include <dctl/pieces/pawn.hpp>             // pawn
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // select

#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>
#include <dctl/board/iterator.hpp>
#include <dctl/node/promotion.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

template<bool Color, class Position, class Sequence>
struct copy<Color, pieces::pawn, select::moves, Position, Sequence>
:
        // enforce reference semantics
        boost::noncopyable
{
private:
        // typedefs

        typedef typename Position::board_type Board;
        typedef typename Sequence::value_type Move;
        typedef board::Compass<Color, Board> Compass;
        typedef Propagate<select::moves, Position> State;

        // representation

        State const& propagate_;
        Sequence& moves_;

public:
        // structors

        explicit copy(State const& p, Sequence& m)
        :
                propagate_(p),
                moves_(m)
        {}

        // function call operators

        void operator()(BitBoard active_pawns) const
        {
                if (active_pawns)
                        branch(active_pawns);
        }

private:
        void branch(BitBoard active_pawns) const
        {
                serialize<typename Compass::left_up >(active_pawns);
                serialize<typename Compass::right_up>(active_pawns);
        }

        template<class Direction>
        void serialize(BitBoard active_pawns) const
        {
                for (
                        active_pawns &= Prev<Board, Direction>()(propagate_.path());
                        active_pawns;
                        bit::pop_front(active_pawns)
                )
                        find<Direction>(bit::minimal_element(active_pawns));
        }

        template<class Direction>
        void find(BitIndex from_sq) const
        {
                auto const dest_sq = Next<Board, Direction>()(from_sq);
                moves_.push_back(Move::template create<Color>(from_sq ^ dest_sq, promotion_sq<Color, Board>(dest_sq)));
        }

        /* replace with
         *
         * active_pawns &= Prev<Board, Direction>()(propagate_.path());
         * std::transform(begin(active_pawns), end(active_pawns), std::back_inserter(moves_), [](Square const& from_sq) -> Move {
         *      auto const dest_sq = Next<Board, Direction>()(from_sq);
         *      return Move::template create<Color>(from_sq ^ dest_sq, promotion_sq<Color, Board>(dest_sq));
         * });
         *
         */
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
