#pragma once
#include "Driver_fwd.hpp"
#include "Selection.hpp"
#include "../bit/Bit.hpp"
#include "../board/Compass.hpp"
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

        typedef angle::Compass<Color, Board> Compass;

public:
        template<typename Position>
        static void generate(Position const& p, Stack& moves)
        {
                generate(p.pawns(Color), not_occupied(p), moves);
        }

        template<typename Position>
        static int count(Position const& p)
        {
                return count(p.pawns(Color), not_occupied(p));
        }

        template<typename Position>
        static bool detect(Position const& p)
        {
                return detect(p.pawns(Color), not_occupied(p));
        }

        static void generate(BitBoard active_pawns, BitBoard not_occupied, Stack& moves)
        {
                generate<typename Compass::left_up >(active_pawns, not_occupied, moves);
                generate<typename Compass::right_up>(active_pawns, not_occupied, moves);
        }

        static int count(BitBoard active_pawns, BitBoard not_occupied)
        {
                return (
                        count<typename Compass::left_up >(active_pawns, not_occupied) +
                        count<typename Compass::right_up>(active_pawns, not_occupied)
                );
        }

        static bool detect(BitBoard active_pawns, BitBoard not_occupied)
        {
                return (
                        detect<typename Compass::left_up >(active_pawns, not_occupied) ||
                        detect<typename Compass::right_up>(active_pawns, not_occupied)
                );
        }

private:
        template<typename Direction>
        static void generate(BitBoard active_pawns, BitBoard not_occupied, Stack& moves)
        {
                BitIndex from_sq, dest_sq;
                for (
                        active_pawns &= Pull<Board, Direction>()(not_occupied);
                        active_pawns;
                        bit::clear_first(active_pawns)
                ) {
                        from_sq = bit::get_first(active_pawns);
                        dest_sq = Board::next<Direction>(from_sq);
                        moves.push_back(
                                Move::create<Color>(
                                        from_sq ^ dest_sq,
                                        promotion_sq<Color, Board>(dest_sq)
                                )
                        );
                }
        }

        template<typename Direction>
        static int count(BitBoard active_pawns, BitBoard not_occupied)
        {
                return bit::count(
                        Sink<Board, Direction, rules::range::distance_1>()(active_pawns, not_occupied)
                );
        }

        template<typename Direction>
        static bool detect(BitBoard active_pawns, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Sink<Board, Direction, rules::range::distance_1>()(active_pawns, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
