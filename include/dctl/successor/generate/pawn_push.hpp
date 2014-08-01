#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/pieces/pawn.hpp>                         // pawn
#include <dctl/successor/propagate/push.hpp>            // Propagate (moves specialization)
#include <dctl/successor/select/push.hpp>               // select

#include <dctl/angle/directions.hpp>                    // left_up, right_up
#include <dctl/board/orientation.hpp>                   // orientation
#include <dctl/position/promotion.hpp>
#include <dctl/ray.hpp>                                 // make_iterator
#include <dctl/type_traits.hpp>
#include <dctl/wave/iterator.hpp>
#include <boost/range/adaptor/transformed.hpp>          // transformed
#include <boost/range/algorithm_ext/push_back.hpp>      // push_back
#include <iterator>                                     // prev

namespace dctl {
namespace successor {

template<bool Color, class Position, class Sequence>
class Generate<Color, pieces::pawn, select::push, Position, Sequence>
{
public:
        // enforce reference semantics
        Generate(Generate const&) = delete;
        Generate& operator=(Generate const&) = delete;

private:
        using Board = board_type_t<Position>;
        using Set = set_type_t<Position>;
        using Move = value_type_t<Sequence>;
        using State = Propagate<select::push, Position>;
        static constexpr auto orientation = orientation_v<Board, Color>;

        // representation

        State const& propagate_;
        Sequence& moves_;

public:
        // constructors

        Generate(State const& p, Sequence& m)
        :
                propagate_{p},
                moves_{m}
        {}

        // function call operators

        auto operator()(Set const& active_pawns) const
        {
                if (active_pawns.empty())
                        return;

                transform_movers<left_up (orientation)>(active_pawns);
                transform_movers<right_up(orientation)>(active_pawns);
        }

private:
        template<int Direction>
        auto transform_movers(Set const& active_pawns) const
        {
                auto const movers = active_pawns & *std::prev(along_wave<Direction>(propagate_.path()));
                boost::push_back(moves_, movers | boost::adaptors::transformed([](auto const& from_sq) {
                        auto const dest_sq = *++along_ray<Direction>(from_sq);
                        return Move{from_sq, dest_sq, is_promotion(dest_sq), Color};
                }));
        }

        template<int Direction>
        static auto along_wave(Set const& s)
        {
                return wave::make_iterator<Board, Direction>(s);
        }

        template<int Direction>
        static auto along_ray(int sq)
        {
                return ray::make_iterator<Board, Direction>(sq);
        }

        static auto is_promotion(int sq)
        {
                return dctl::is_promotion<Color, Board>(sq);
        }
};

}       // namespace successor
}       // namespace dctl
