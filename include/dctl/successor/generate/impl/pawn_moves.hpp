#pragma once
#include <boost/range/adaptor/transformed.hpp>          // transformed
#include <boost/range/algorithm_ext/push_back.hpp>      // push_back
#include <dctl/successor/generate/impl/primary_fwd.hpp> // generate (primary template)
#include <dctl/pieces/pawn.hpp>                         // pawn
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // select

#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/position/promotion.hpp>
#include <dctl/ray.hpp>
#include <dctl/wave/iterator.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

template<bool Color, class Position, class Sequence>
struct generate<Color, pieces::pawn, select::moves, Position, Sequence>
{
public:
        // enforce reference semantics
        generate(generate const&) = delete;
        generate& operator=(generate const&) = delete;

private:
        using Board = typename Position::board_type;
        using Set = typename Board::set_type;
        using Move = typename Sequence::value_type;
        using Compass = board::Compass<Board, Color>;
        using State = Propagate<select::moves, Position>;

        // representation

        State const& propagate_;
        Sequence& moves_;

public:
        // structors

        explicit generate(State const& p, Sequence& m)
        :
                propagate_{p},
                moves_{m}
        {}

        // function call operators

        void operator()(Set const& active_pawns) const
        {
                if (active_pawns.empty())
                        return;

                transform_movers<Compass::left_up >(active_pawns);
                transform_movers<Compass::right_up>(active_pawns);
        }

private:
        template<int Direction>
        void transform_movers(Set const& active_pawns) const
        {
                auto const movers = active_pawns & *std::prev(along_wave<Direction>(propagate_.path()));
                boost::push_back(moves_, movers | boost::adaptors::transformed([](auto const& from_sq) {
                        auto const dest_sq = *++along_ray<Direction>(from_sq);
                        return Move{from_sq, dest_sq, Color, is_promotion(dest_sq)};
                }));
        }

        template<int Direction>
        static wave::Iterator<Board, Direction> along_wave(Set const& s)
        {
                return wave::make_iterator<Board, Direction>(s);
        }

        template<int Direction>
        static ray::Iterator<Board, Direction> along_ray(int sq)
        {
                return ray::make_iterator<Board, Direction>(sq);
        }

        static bool is_promotion(int sq)
        {
                return dctl::is_promotion<Color, Board>(sq);
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
