#pragma once
#include <algorithm>
#include <iterator>
#include <dctl/successor/generate/impl/primary_fwd.hpp> // generate (primary template)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/pieces/king.hpp>                         // king

#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/position/unary_projections.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/ray/classical.hpp>
#include <dctl/ray/fill.hpp>
#include <dctl/ray/iterator.hpp>
#include <dctl/board/mask.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for king moves generation
template<bool Color, class Position, class Sequence>
struct generate<Color, pieces::king, select::moves, Position, Sequence>
{
public:
        // enforce reference semantics
        generate(generate const&) = delete;
        generate& operator=(generate const&) = delete;

private:
        using Rules = typename Position::rules_type;
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

        void operator()(Set const& active_kings) const
        {
                // tag dispatching on king range
                find_dispatch(active_kings, rules::range::move<Rules>{});
        }

private:
        // overload for short ranged kings
        void find_dispatch(Set const& active_kings, rules::range::distance_1) const
        {
                if (active_kings.empty())
                        return;

                transform_movers<Compass::left_down >(active_kings);
                transform_movers<Compass::right_down>(active_kings);
                transform_movers<Compass::left_up   >(active_kings);
                transform_movers<Compass::right_up  >(active_kings);
        }

        // overload for long ranged kings
        void find_dispatch(Set const& active_kings, rules::range::distance_N) const
        {
                for (auto const& from_sq : active_kings) {
                        transform_targets(along_ray<Compass::left_down >(from_sq));
                        transform_targets(along_ray<Compass::right_down>(from_sq));
                        transform_targets(along_ray<Compass::left_up   >(from_sq));
                        transform_targets(along_ray<Compass::right_up  >(from_sq));
                }
        }

        template<int Direction>
        void transform_movers(Set const& active_kings) const
        {
                auto const movers = active_kings & *std::prev(along_wave<Direction>(propagate_.path()));
                std::transform(begin(movers), end(movers), std::back_inserter(moves_), [](auto const& from_sq) {
                        return Move{from_sq, *++along_ray<Direction>(from_sq)};
                });
        }

        template<int Direction>
        void transform_targets(ray::Iterator<Board, Direction> from) const
        {
                auto const targets = ray::fill(from, propagate_.path());
                std::transform(begin(targets), end(targets), std::back_inserter(moves_), [=](auto const& dest_sq) {
                        return Move{*from, dest_sq};
                });
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
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
