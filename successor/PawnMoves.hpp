#pragma once
#include "../bit/Bit.hpp"
#include "../board/Direction.hpp"
#include "../node/Material.hpp"
#include "../node/Position.hpp"
#include "../node/Promotion.hpp"
#include "../node/Stack.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/NonConstructible.hpp"
#include "../utility/Shift.hpp"

namespace dctl {

namespace board { template<bool, typename> struct Direction; }

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> class Driver;
struct Moves;

// partial specialization for pawn moves
template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::pawn, Moves, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
public:
        // typedefs
        typedef board::Direction<Color, Board> Direction;

        static void generate(const Position<Board>& p, Stack& moves)
        {
                generate(p.men(Color), not_occupied(p), moves);
        }

        static int count(const Position<Board>& p)
        {
                return count(p.men(Color), not_occupied(p));
        }

        static bool detect(const Position<Board>& p)
        {
                return detect(p.men(Color), not_occupied(p));
        }

        static void generate(BitBoard active_men, BitBoard not_occupied, Stack& moves)
        {
                generate<Direction::left_up >(active_men, not_occupied, moves);
                generate<Direction::right_up>(active_men, not_occupied, moves);
        }

        static int count(BitBoard active_men, BitBoard not_occupied)
        {
                return (
                        count<Direction::left_up >(active_men, not_occupied) +
                        count<Direction::right_up>(active_men, not_occupied)
                );
        }

        static bool detect(BitBoard active_men, BitBoard not_occupied)
        {
                return (
                        detect<Direction::left_up >(active_men, not_occupied) ||
                        detect<Direction::right_up>(active_men, not_occupied)
                );
        }

private:
        // implementation
        template<int Index> 
        static void generate(BitBoard active_men, BitBoard not_occupied, Stack& moves)
        {
                BitBoard from_sq, dest_sq;
                for (
                        active_men &= Pull<Board, Index>()(not_occupied); 
                        active_men; 
                        bit::clear_first(active_men)
                ) {
                        from_sq = bit::get_first(active_men);
                        dest_sq = Push<Board, Index>()(from_sq);
                        push<Color>(
                                from_sq ^ dest_sq, 
                                promotion_sq<Color, Board>(dest_sq), 
                                moves
                        );
                }
        }

        template<int Index> 
        static int count(BitBoard active_men, BitBoard not_occupied)
        {
                return bit::count(
                        Destinations<Moves, Board, Index, rules::scan_1>()(active_men, not_occupied)
                );
        }

        template<int Index> 
        static bool detect(BitBoard active_men, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Destinations<Moves, Board, Index, rules::scan_1>()(active_men, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
