#pragma once
#include "Generator_fwd.hpp"
#include "../Select.hpp"
#include "../../bit/Bit.hpp"
#include "../../board/Compass.hpp"
#include "../../board/Shift.hpp"
#include "../../node/Material.hpp"
#include "../../node/Promotion.hpp"
#include "../../node/Stack.hpp"
#include "../../rules/Enum.hpp"
#include "../../utility/IntegerTypes.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn moves generation
template<bool Color, typename Position>
struct generator<Color, Material::pawn, Moves, Position>
{
private:
        // typedefs

        typedef typename Position::board_type Board;
        typedef angle::Compass<Color, Board> Compass;

public:
        void operator()(Position const& p, Stack& moves)
        {
                if (auto const active_pawns = p.pawns(Color))
                        select(active_pawns, not_occupied(p), moves);
        }

private:
        void select(BitBoard active_pawns, BitBoard not_occupied, Stack& moves)
        {
                branch(active_pawns, not_occupied, moves);
        }

        void branch(BitBoard active_pawns, BitBoard not_occupied, Stack& moves)
        {
                serialize<typename Compass::left_up >(active_pawns, not_occupied, moves);
                serialize<typename Compass::right_up>(active_pawns, not_occupied, moves);
        }

        template<typename Direction>
        void serialize(BitBoard active_pawns, BitBoard not_occupied, Stack& moves)
        {
                for (
                        active_pawns &= Pull<Board, Direction>()(not_occupied);
                        active_pawns;
                        bit::clear_first(active_pawns)
                )
                        find<Direction>(bit::get_first(active_pawns), moves);
        }

        template<typename Direction>
        void find(BitIndex from_sq, Stack& moves)
        {
                auto const dest_sq = Board::next<Direction>(from_sq);
                moves.push_back(Move::create<Color>(from_sq ^ dest_sq, promotion_sq<Color, Board>(dest_sq)));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
