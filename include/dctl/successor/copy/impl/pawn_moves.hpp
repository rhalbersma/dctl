#pragma once
#include <boost/utility.hpp>                            // noncopyable
#include <dctl/successor/copy/impl/primary_fwd.hpp>     // copy (primary template)
#include <dctl/pieces/pawn.hpp>                         // pawn
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
        using Board = typename Position::board_type;
        using Move = typename Sequence::value_type;
        using Compass = board::Compass<Color, Board>;
        using State = Propagate<select::moves, Position>;

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
                copy_if<typename Compass::left_up >(active_pawns);
                copy_if<typename Compass::right_up>(active_pawns);
        }

        template<class Direction>
        void copy_if(BitBoard active_pawns) const
        {
                transform<Direction>(
                        bit::set_intersection(
                                active_pawns,
                                Prev<Board, Direction>()(propagate_.path())
                        )
                );
        }

        template<class Direction>
        void transform(BitBoard movers) const
        {
                for (; movers; bit::pop_front(movers)) {
                        auto const from_sq = bit::minimal_element(movers);
                        auto const dest_sq = Next<Board, Direction>()(from_sq);
                        moves_.push_back(Move::template create<Color>(from_sq ^ dest_sq, promotion_sq<Color, Board>(dest_sq)));
                }
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
