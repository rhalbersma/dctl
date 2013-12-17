#pragma once
#include <iterator>
#include <dctl/successor/generate/impl/primary_fwd.hpp> // generate (primary template)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/pieces/king.hpp>           // king

#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/board/iterator.hpp>                      // Increment, Next
#include <dctl/node/unary_projections.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/ray/iterator.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for king moves generation
template<bool Color, class Position, class Sequence>
struct generate<Color, pieces::king, select::moves, Position, Sequence>
{
        // enforce reference semantics
        generate(generate const&) = delete;
        generate& operator=(generate const&) = delete;

private:
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
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

        template<class Set>
        void operator()(Set const& active_kings) const
        {
                serialize(active_kings);
        }

private:
        template<class Set>
        void serialize(Set const& active_kings) const
        {
                for (auto from_sq : active_kings)
                        branch(from_sq);
        }

        void branch(int from_sq) const
        {
                find(along_ray< Compass::left_down  >(from_sq));
                find(along_ray< Compass::right_down >(from_sq));
                find(along_ray< Compass::left_up    >(from_sq));
                find(along_ray< Compass::right_up   >(from_sq));
        }

        template<class Iterator>
        void find(Iterator from) const
        {
                // tag dispatching on king range
                find_dispatch(from, rules::range::move<Rules>{});
        }

        // overload for short ranged kings
        template<class Iterator>
        void find_dispatch(Iterator from, rules::range::distance_1) const
        {
                auto const dest = std::next(from);
                if (propagate_.path(*dest))
                        moves_.emplace_back(*from, *dest);
        }

        // overload for long ranged kings
        template<class Iterator>
        void find_dispatch(Iterator from, rules::range::distance_N) const
        {
                for (auto dest = std::next(from); propagate_.path(*dest); ++dest)
                        moves_.emplace_back(*from, *dest);
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
