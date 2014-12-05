#pragma once
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/pieces/pawn.hpp>                         // pawn
#include <dctl/successor/select/push.hpp>               // select

#include <dctl/angle/directions.hpp>    // left_up, right_up
#include <dctl/color.hpp>
#include <dctl/board/orientation.hpp>   // orientation
#include <dctl/position/promotion.hpp>
#include <dctl/ray.hpp>                 // make_iterator
#include <dctl/type_traits.hpp>
#include <dctl/wave/iterator.hpp>
#include <algorithm>                    // transform
#include <iterator>                     // prev, back_inserter

namespace dctl {
namespace successor {

template<Color ToMove, class Position, class Sequence>
class Generate<ToMove, pieces::pawn, select::push, Position, Sequence>
{
public:
        // enforce reference semantics
        Generate(Generate const&) = delete;
        Generate& operator=(Generate const&) = delete;

private:
        using Board = board_type_t<Position>;
        using Set = set_type_t<Position>;
        using Move = value_type_t<Sequence>;
        static constexpr auto orientation = orientation_v<Board, ToMove>;

        // representation

        Set const& propagate;
        Sequence& moves;

public:
        // constructors

        Generate(Set const& p, Sequence& m)
        :
                propagate{p},
                moves{m}
        {}

        // function call operators

        auto operator()(Set const& active_pawns) const
        {
                if (active_pawns.none())
                        return;

                transform_movers<left_up (orientation)>(active_pawns);
                transform_movers<right_up(orientation)>(active_pawns);
        }

private:
        template<int Direction>
        auto transform_movers(Set const& active_pawns) const
        {
                auto const movers = active_pawns & Set(*std::prev(along_wave<Direction>(propagate)));
                std::transform(begin(movers), end(movers), std::back_inserter(moves), [](auto const& from_sq) {
                        auto const dest_sq = *++along_ray<Direction>(from_sq);
                        return Move{from_sq, dest_sq, is_promotion(dest_sq), ToMove};
                });
        }

        template<int Direction>
        static auto along_wave(Set const& s)
        {
                return wave::make_iterator<Board, Direction>(s);
        }

        template<int Direction>
        static auto along_ray(std::size_t sq)
        {
                return ray::make_iterator<Board, Direction>(sq);
        }

        static auto is_promotion(std::size_t sq)
        {
                return dctl::is_promotion<ToMove, Board>(sq);
        }
};

}       // namespace successor
}       // namespace dctl
