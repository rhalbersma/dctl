#pragma once
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/copy/aux/primary_fwd.hpp>
#include <dctl/successor/propagate/moves.hpp>
#include <dctl/successor/select/moves.hpp>
#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>
#include <dctl/board/iterator.hpp>
#include <dctl/node/material.hpp>
#include <dctl/node/move.hpp>
#include <dctl/node/promotion.hpp>
#include <dctl/node/stack.hpp>
#include <dctl/node/unary_projections.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace successor {
namespace aux {

template<bool Color, typename Position>
struct copy<Color, Material::pawn, select::moves, Position>
:
        boost::noncopyable
{
private:
        // typedefs

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

        template<typename Direction>
        void serialize(BitBoard active_pawns) const
        {
                for (
                        active_pawns &= Prev<Board, Direction>()(propagate_.path());
                        active_pawns;
                        bit::first::clear(active_pawns)
                )
                        find<Direction>(bit::first::equal(active_pawns));
        }

        template<typename Direction>
        void find(BitIndex from_sq) const
        {
                auto const dest_sq = Next<Board, Direction>()(from_sq);
                moves_.push_back(Move::template create<Color>(from_sq ^ dest_sq, promotion_sq<Color, Board>(dest_sq)));
        }
};

}       // namespace aux
}       // namespace successor
}       // namespace dctl
