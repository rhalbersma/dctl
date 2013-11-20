#pragma once
#include <algorithm>                                    // transform
#include <iterator>
#include <dctl/successor/copy/impl/primary_fwd.hpp>     // copy (primary template)
#include <dctl/pieces/pawn.hpp>                         // pawn
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // select

#include <dctl/bit/bitboard.hpp>
#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/board/iterator.hpp>                      // Next, Prev
#include <dctl/node/promotion.hpp>
#include <dctl/ray/iterator.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

template<bool Color, class Position, class Sequence>
struct copy<Color, pieces::pawn, select::moves, Position, Sequence>
{
        // enforce reference semantics
        copy(copy const&) = delete;
        copy& operator=(copy const&) = delete;

private:
        using Board = typename Position::board_type;
        using Move = typename Sequence::value_type;
        using Compass = board::Compass<Board, Color>;
        using State = Propagate<select::moves, Position>;

        // representation

        State const& propagate_;
        Sequence& moves_;

public:
        // structors

        explicit copy(State const& p, Sequence& m)
        :
                propagate_{p},
                moves_{m}
        {}

        // function call operators

        template<class Set>
        void operator()(Set const& active_pawns) const
        {
                if (!active_pawns.empty())
                        branch(active_pawns);
        }

private:
        template<class Set>
        void branch(Set const& active_pawns) const
        {
                copy_if< Compass::left_up  >(active_pawns);
                copy_if< Compass::right_up >(active_pawns);
        }

        template<int Direction, class Set>
        void copy_if(Set const& active_pawns) const
        {
                transform<Direction>(active_pawns & Prev<Board, Direction>{}(propagate_.path()));
        }

        template<int Direction, class Set>
        void transform(Set movers) const
        {
                std::transform(begin(movers), end(movers), std::back_inserter(moves_), [](auto const& from_sq) {
                        auto const dest_sq = *++along_ray<Direction>(from_sq);
                        return Move::template create<Color>({from_sq, dest_sq}, promotion_sq<Color, Board>(dest_sq));
                });
        }

        template<int Direction>
        static ray::Iterator<Board, Direction> along_ray(int sq)
        {
                return ray::make_iterator<Board, Direction>(sq);
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
