#pragma once
#include "Driver_fwd.hpp"
#include "Selection.hpp"
#include "../bit/Bit.hpp"
#include "../board/Direction.hpp"
#include "../board/Shift.hpp"
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
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

        typedef board::Direction<Color, Board> Direction;

public:
        template<template<typename, typename> class Position>
        static void generate(const Position<Rules, Board>& p, Stack& moves)
        {
                serialize(unrestricted_kings<Rules>(p, Color), not_occupied(p), moves);
        }

        template<template<typename, typename> class Position>
        static int count(const Position<Rules, Board>& p)
        {
                return count(unrestricted_kings<Rules>(p, Color), not_occupied(p));
        }

        template<template<typename, typename> class Position>
        static bool detect(const Position<Rules, Board>& p)
        {
                return detect(unrestricted_kings<Rules>(p, Color), not_occupied(p));
        }

private:
        static void serialize(BitBoard active_kings, BitBoard not_occupied, Stack& moves)
        {
                // tag dispatching on restrictions on consecutive moves with the same king
                serialize_dispatch(
                        active_kings, not_occupied, moves,
                        Int2Type<rules::is_restricted_same_king_moves<Rules>::value>()
                );
        }

        // partial specialization for unrestricted consecutive moves with the same king
        static void serialize_dispatch(
                BitBoard active_kings, BitBoard not_occupied, Stack& moves, 
                Int2Type<false>
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
                Int2Type<true>
        )
        {
                // loop could be empty if the single active king detected during
                // Dispatcher<Selection, Rules, Board>::select is restricted to move
                while (active_kings) {
                        generate(bit::get_first(active_kings), not_occupied, moves);
                        bit::clear_first(active_kings);
                }
        }

        static void generate(BitBoard from_sq, BitBoard not_occupied, Stack& moves)
        {
                generate<Direction::left_down >(from_sq, not_occupied, moves);
                generate<Direction::right_down>(from_sq, not_occupied, moves);
                generate<Direction::left_up   >(from_sq, not_occupied, moves);
                generate<Direction::right_up  >(from_sq, not_occupied, moves);
        }

        static int count(BitBoard active_kings, BitBoard not_occupied)
        {
                return (
                        count<Direction::left_down >(active_kings, not_occupied) +
                        count<Direction::right_down>(active_kings, not_occupied) +
                        count<Direction::left_up   >(active_kings, not_occupied) +
                        count<Direction::right_up  >(active_kings, not_occupied)
                );
        }

        static bool detect(BitBoard active_kings, BitBoard not_occupied)
        {
                return (
                        detect<Direction::left_down >(active_kings, not_occupied) ||
                        detect<Direction::right_down>(active_kings, not_occupied) ||
                        detect<Direction::left_up   >(active_kings, not_occupied) ||
                        detect<Direction::right_up  >(active_kings, not_occupied)
                );
        }

        template<int Index>
        static void generate(BitBoard from_sq, BitBoard not_occupied, Stack& moves)
        {
                // tag dispatching on king range
                return generate_dispatch<Index>(
                        from_sq, not_occupied, moves,
                        Int2Type<rules::king_scan_range<Rules>::value>()
                );
        }

        // partial specialization for short ranged kings
        template<int Index>
        static void generate_dispatch(
                BitBoard from_sq, BitBoard not_occupied, Stack& moves, 
                Int2Type<rules::scan_1>
        )
        {
                if (const auto dest_sq = Push<Board, Index>()(from_sq) & not_occupied)
                        moves.push_back(Move::create<Color>(from_sq ^ dest_sq));
        }

        // partial specialization for long ranged kings
        template<int Index>
        static void generate_dispatch(
                BitBoard from_sq, BitBoard not_occupied, Stack& moves, 
                Int2Type<rules::scan_N>
        )
        {
                for (
                        auto dest_sq = Push<Board, Index>()(from_sq);
                        dest_sq & not_occupied;
                        PushAssign<Board, Index>()(dest_sq)
                )
                        moves.push_back(Move::create<Color>(from_sq ^ dest_sq));
        }

        template<int Index>
        static int count(BitBoard active_kings, BitBoard not_occupied)
        {
                return bit::count(
                        Sink<Board, Index, rules::king_scan_range<Rules>::value>()(active_kings, not_occupied)
                );
        }

        template<int Index>
        static bool detect(BitBoard active_kings, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Sink<Board, Index, rules::scan_1>()(active_kings, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
