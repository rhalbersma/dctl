#pragma once
#include "Driver_fwd.hpp"
#include "Selection.hpp"
#include "../bit/Bit.hpp"
#include "../board/Direction.hpp"
#include "../board/Shift.hpp"
#include "../node/Material.hpp"
#include "../node/Promotion.hpp"
#include "../node/Stack.hpp"
#include "../rules/Enum.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for pawn moves
template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::pawn, select::Moves, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef board::Direction<Color, Board> Direction;

public:
        template<template<typename, typename> class Position>
        static void generate(Position<Rules, Board> const& p, Stack& moves)
        {
                generate(p.pawns(Color), not_occupied(p), moves);
        }

        template<template<typename, typename> class Position>
        static int count(Position<Rules, Board> const& p)
        {
                return count(p.pawns(Color), not_occupied(p));
        }

        template<template<typename, typename> class Position>
        static bool detect(Position<Rules, Board> const& p)
        {
                return detect(p.pawns(Color), not_occupied(p));
        }

        static void generate(BitBoard active_pawns, BitBoard not_occupied, Stack& moves)
        {
                generate<typename Direction::left_up >(active_pawns, not_occupied, moves);
                generate<typename Direction::right_up>(active_pawns, not_occupied, moves);
        }

        static int count(BitBoard active_pawns, BitBoard not_occupied)
        {
                return (
                        count<typename Direction::left_up >(active_pawns, not_occupied) +
                        count<typename Direction::right_up>(active_pawns, not_occupied)
                );
        }

        static bool detect(BitBoard active_pawns, BitBoard not_occupied)
        {
                return (
                        detect<typename Direction::left_up >(active_pawns, not_occupied) ||
                        detect<typename Direction::right_up>(active_pawns, not_occupied)
                );
        }

private:
        template<typename Index>
        static void generate(BitBoard active_pawns, BitBoard not_occupied, Stack& moves)
        {
                BitBoard from_sq, dest_sq;
                for (
                        active_pawns &= Pull<Board, Index>()(not_occupied);
                        active_pawns;
                        bit::clear_first(active_pawns)
                ) {
                        from_sq = bit::get_first(active_pawns);
                        dest_sq = Push<Board, Index>()(from_sq);
                        moves.push_back(
                                Move::create<Color>(
                                        from_sq ^ dest_sq,
                                        promotion_sq<Color, Board>(dest_sq)
                                )
                        );
                }
        }

        template<typename Index>
        static int count(BitBoard active_pawns, BitBoard not_occupied)
        {
                return bit::count(
                        Sink<Board, Index, rules::range::distance_1>()(active_pawns, not_occupied)
                );
        }

        template<typename Index>
        static bool detect(BitBoard active_pawns, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Sink<Board, Index, rules::range::distance_1>()(active_pawns, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
