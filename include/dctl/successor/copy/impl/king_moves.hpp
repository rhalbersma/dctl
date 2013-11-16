#pragma once
#include <iterator>
#include <dctl/successor/copy/impl/primary_fwd.hpp>     // copy (primary template)
#include <dctl/successor/propagate/moves.hpp>           // Propagate (moves specialization)
#include <dctl/successor/select/moves.hpp>              // moves
#include <dctl/pieces/king.hpp>           // king

#include <dctl/bit/bit.hpp>
#include <dctl/board/compass.hpp>                       // Compass
#include <dctl/board/iterator.hpp>                      // Increment, Next
#include <dctl/node/unary_projections.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/bit/bitboard.hpp>                        // BitIndex
#include <dctl/ray/iterator.hpp>

namespace dctl {
namespace successor {
namespace detail {
namespace impl {

// partial specialization for king moves generation
template<bool Color, class Position, class Sequence>
struct copy<Color, pieces::king, select::moves, Position, Sequence>
{
        // enforce reference semantics
        copy(copy const&) = delete;
        copy& operator=(copy const&) = delete;

private:
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
        using Move = typename Sequence::value_type;
        using Compass = board::Compass<Board, Color>;
        using State = Propagate<select::moves, Position>;

        using BitSet = bit::bit_set<int, uint64_t, 1>;

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
        void operator()(Set const& active_kings) const
        {
                serialize(active_kings);
        }

private:
        template<class Set>
        void serialize(Set const& active_kings) const
        {
                for (auto from_sq: active_kings)
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
        void find(Iterator it) const
        {
                // tag dispatching on king range
                find_dispatch(it, rules::range::move<Rules>{});
        }

        // overload for short ranged kings
        template<class Iterator>
        void find_dispatch(Iterator it, rules::range::distance_1) const
        {
                auto const from_sq = BitBoard{1} << *it;
                auto const dest = std::next(it);
                auto const dest_sq = BitBoard{1} << *dest;
                if (BitSet(propagate_.path()).test(*dest))
                        moves_.push_back(Move::template create<Color>(from_sq ^ dest_sq));
        }

        // overload for long ranged kings
        template<class Iterator>
        void find_dispatch(Iterator it, rules::range::distance_N) const
        {
                auto const from_sq = BitBoard{1} << *it;
                for (
                        auto dest = std::next(it);
                        BitSet(propagate_.path()).test(*dest);
                        ++dest
                ) {
                        auto const dest_sq = BitBoard{1} << *dest;
                        moves_.push_back(Move::template create<Color>(from_sq ^ dest_sq));
                }
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
