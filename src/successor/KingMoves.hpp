#pragma once
#include <boost/mpl/bool_fwd.hpp>       // false_, true_
#include "Driver_fwd.hpp"
#include "Selection.hpp"
#include "../bit/Bit.hpp"
#include "../board/Direction.hpp"
#include "../board/Shift.hpp"
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../rules/Enum.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for king moves
template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::king, select::Moves, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef angle::Direction<Color, Board> Direction;

public:
        template<template<typename, typename> class Position>
        static void generate(Position<Rules, Board> const& p, Stack& moves)
        {
                serialize(unrestricted_kings(p, Color), not_occupied(p), moves);
        }

        template<template<typename, typename> class Position>
        static int count(Position<Rules, Board> const& p)
        {
                return count(unrestricted_kings(p, Color), not_occupied(p));
        }

        template<template<typename, typename> class Position>
        static bool detect(Position<Rules, Board> const& p)
        {
                return detect(unrestricted_kings(p, Color), not_occupied(p));
        }

private:
        static void serialize(BitBoard active_kings, BitBoard not_occupied, Stack& moves)
        {
                // tag dispatching on restrictions on consecutive moves with the same king
                serialize_dispatch(
                        active_kings, not_occupied, moves,
                        typename Rules::is_restricted_same_king_moves()
                );
        }

        // partial specialization for unrestricted consecutive moves with the same king
        static void serialize_dispatch(
                BitBoard active_kings, BitBoard not_occupied, Stack& moves, 
                boost::mpl::false_
        )
        {
                // loop cannot be empty because all active kings detected during
                // Dispatcher<Selection, Rules, Board>::select are unrestricted to move
                BOOST_ASSERT(!bit::is_zero(active_kings));
                do {
                        generate(bit::get_first(active_kings), not_occupied, moves);
                        bit::clear_first(active_kings);
                } while (active_kings);
        }

        // partial specialization for restricted consecutive moves with the same king
        static void serialize_dispatch(
                BitBoard active_kings, BitBoard not_occupied, Stack& moves, 
                boost::mpl::true_
        )
        {
                // loop could be empty if the single active king detected during
                // Dispatcher<Selection, Rules, Board>::select is restricted to move
                while (active_kings) {
                        generate(bit::get_first(active_kings), not_occupied, moves);
                        bit::clear_first(active_kings);
                }
        }

        static void generate(BitIndex from_sq, BitBoard not_occupied, Stack& moves)
        {
                generate<typename Direction::left_down >(from_sq, not_occupied, moves);
                generate<typename Direction::right_down>(from_sq, not_occupied, moves);
                generate<typename Direction::left_up   >(from_sq, not_occupied, moves);
                generate<typename Direction::right_up  >(from_sq, not_occupied, moves);
        }

        static int count(BitBoard active_kings, BitBoard not_occupied)
        {
                return (
                        count<typename Direction::left_down >(active_kings, not_occupied) +
                        count<typename Direction::right_down>(active_kings, not_occupied) +
                        count<typename Direction::left_up   >(active_kings, not_occupied) +
                        count<typename Direction::right_up  >(active_kings, not_occupied)
                );
        }

        static bool detect(BitBoard active_kings, BitBoard not_occupied)
        {
                return (
                        detect<typename Direction::left_down >(active_kings, not_occupied) ||
                        detect<typename Direction::right_down>(active_kings, not_occupied) ||
                        detect<typename Direction::left_up   >(active_kings, not_occupied) ||
                        detect<typename Direction::right_up  >(active_kings, not_occupied)
                );
        }

        template<typename Index>
        static void generate(BitIndex from_sq, BitBoard not_occupied, Stack& moves)
        {
                // tag dispatching on king range
                return generate_dispatch<Index>(
                        from_sq, not_occupied, moves,
                        typename Rules::king_range()
                );
        }

        // partial specialization for short ranged kings
        template<typename Index>
        static void generate_dispatch(
                BitIndex from_sq, BitBoard not_occupied, Stack& moves, 
                rules::range::distance_1
        )
        {
                if (auto const dest_sq = Board::next<Index>(from_sq) & not_occupied)
                        moves.push_back(Move::create<Color>(from_sq ^ dest_sq));
        }

        // partial specialization for long ranged kings
        template<typename Index>
        static void generate_dispatch(
                BitIndex from_sq, BitBoard not_occupied, Stack& moves, 
                rules::range::distance_N
        )
        {
                for (
                        auto dest_sq = Board::next<Index>(from_sq);
                        bit::is_element(dest_sq, not_occupied);
                        Board::advance<Index>(dest_sq)
                )
                        moves.push_back(Move::create<Color>(from_sq ^ dest_sq));
        }

        template<typename Index>
        static int count(BitBoard active_kings, BitBoard not_occupied)
        {
                return bit::count(
                        Sink<Board, Index, typename Rules::king_range>()(active_kings, not_occupied)
                );
        }

        template<typename Index>
        static bool detect(BitBoard active_kings, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Sink<Board, Index, rules::range::distance_1>()(active_kings, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
