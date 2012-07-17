#pragma once
#include "../Driver_fwd.hpp"
#include "../Result.hpp"
#include "../Selection.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Shift.hpp"
#include "../../node/Material.hpp"
#include "../../node/Promotion.hpp"
#include "../../node/Stack.hpp"
#include "../../rules/Enum.hpp"
#include "../../utility/IntegerTypes.hpp"
#include "../../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for pawn moves generation
template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::pawn, select::Moves, generation, Rules, Board>
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
                select(p.pawns(Color), not_occupied(p), moves);
        }

        static void select(BitBoard active_pawns, BitBoard not_occupied, Stack& moves)
        {
                branch(active_pawns, not_occupied, moves);
        }

private:
        static void branch(BitBoard active_pawns, BitBoard not_occupied, Stack& moves)
        {
                serialize<typename Compass::left_up >(active_pawns, not_occupied, moves);
                serialize<typename Compass::right_up>(active_pawns, not_occupied, moves);
        }

        template<typename Direction>
        static void serialize(BitBoard active_pawns, BitBoard not_occupied, Stack& moves)
        {
                for (
                        active_pawns &= Pull<Board, Direction>()(not_occupied);
                        active_pawns;
                        bit::clear_first(active_pawns)
                )
                        find<Direction>(bit::get_first(active_pawns), moves);
        }

        template<typename Direction>
        static void find(BitIndex from_sq, Stack& moves)
        {
                auto const dest_sq = Board::next<Direction>(from_sq);
                moves.push_back(Move::create<Color>(from_sq ^ dest_sq, promotion_sq<Color, Board>(dest_sq)));
        }
};

}       // namespace successor
}       // namespace dctl
