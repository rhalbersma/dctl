#pragma once
#include <dctl/board/angle.hpp>                               // left_up, right_up
#include <dctl/color.hpp>                               // Player
#include <dctl/piece.hpp>                               // pawn
#include <dctl/state/promotion.hpp>                  // is_promotion
#include <dctl/actions/generate/detail/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/actions/select/push.hpp>               // select

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/ray.hpp>                                 // make_iterator
#include <dctl/utility/type_traits.hpp>                         // board_type_t, set_type_t, value_type_t
#include <dctl/board/wave/iterator.hpp>                       // make_iterator
#include <iterator>                                     // prev

namespace dctl {
namespace actions {
namespace detail {

template<Color ToMove, bool Reverse, class State, class Sequence>
class Generate<ToMove, Piece::pawn, select::push, Reverse, State, Sequence>
{
        using board_type = board_type_t<State>;
        using   set_type =   set_type_t<State>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse>;
        State const& position;
        Sequence& moves;

public:
        Generate(State const& p, Sequence& m)
        :
                position{p},
                moves{m}
        {}

        auto operator()() const
        {
                generate(position.pieces(ToMove, Piece::pawn));
        }

        auto operator()(set_type const& active_pawns) const
        {
                generate(active_pawns);
        }

private:
        auto generate(set_type const& active_pawns) const
        {
                if (active_pawns.none())
                        return;

                generate_movers<left_up (orientation)>(active_pawns);
                generate_movers<right_up(orientation)>(active_pawns);
        }

        template<int Direction>
        auto generate_movers(set_type const& active_pawns) const
        {
                auto movers = active_pawns & set_type(*std::prev(along_wave<Direction>(position.not_occupied())));
                movers.consume_each([&](auto const& from_sq){
                        auto const dest_sq = *++along_ray<Direction>(from_sq);
                        moves.emplace_back(from_sq, dest_sq, ToMove, is_promotion(dest_sq) ? Piece::king : Piece::pawn);
                });
        }

        template<int Direction>
        static auto along_wave(set_type const& s)
        {
                return wave::make_iterator<board_type, Direction>(s);
        }

        template<int Direction>
        static auto along_ray(std::size_t sq)
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }

        static auto is_promotion(std::size_t sq)
        {
                return dctl::is_promotion<board_type, ToMove>(sq);
        }
};

}       // namespace detail
}       // namespace actions
}       // namespace dctl
