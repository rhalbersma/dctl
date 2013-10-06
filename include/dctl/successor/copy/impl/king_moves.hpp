#pragma once
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

        template<class Set>
        void operator()(Set const& active_kings) const
        {
                serialize(active_kings);
        }

private:
        template<class Set>
        void serialize(Set const& active_kings) const
        {
                for (auto sq: bit::bit_set<int, uint64_t, 1>(active_kings))
                        branch(BitBoard{1} << sq);
        }

        void branch(BitIndex from_sq) const
        {
                find< Compass::left_down  >(from_sq);
                find< Compass::right_down >(from_sq);
                find< Compass::left_up    >(from_sq);
                find< Compass::right_up   >(from_sq);
        }

        template<int Direction>
        void find(BitIndex from_sq) const
        {
                // tag dispatching on king range
                find_dispatch<Direction>(from_sq, rules::range::move<Rules>());
        }

        // overload for short ranged kings
        template<int Direction>
        void find_dispatch(BitIndex from_sq, rules::range::distance_1) const
        {
                if (auto const dest_sq = Next<Board, Direction>()(from_sq) & propagate_.path())
                        moves_.push_back(Move::template create<Color>(from_sq ^ dest_sq));
        }

        // overload for long ranged kings
        template<int Direction>
        void find_dispatch(BitIndex from_sq, rules::range::distance_N) const
        {
                for (
                        auto dest_sq = Next<Board, Direction>()(from_sq);
                        bit::is_element(dest_sq, propagate_.path());
                        Increment<Board, Direction>()(dest_sq)
                )
                        moves_.push_back(Move::template create<Color>(from_sq ^ dest_sq));
        }
};

}       // namespace impl
}       // namespace detail
}       // namespace successor
}       // namespace dctl
