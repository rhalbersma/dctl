#pragma once
#include "../bit/Bit.hpp"
#include "../board/Direction.hpp"
#include "../node/Material.hpp"
#include "../node/Position.hpp"
#include "../node/Stack.hpp"
#include "../rules/Rules.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/NonConstructible.hpp"
#include "../utility/Shift.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {

namespace board { template<bool, typename> struct Direction; }
template<typename> struct Position;

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> class Driver;
struct Moves;

// partial specialization for king moves
template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::king, Moves, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>& p, Stack& moves)
        {
                generate_serial(unrestricted_kings<Rules>(p, Color), not_occupied(p), moves);
        }

        static int count(const Position<Board>& p)
        {
                return count_dirs(unrestricted_kings<Rules>(p, Color), not_occupied(p));
        }    

        static bool detect(const Position<Board>& p)
        {
                return detect_dirs(unrestricted_kings<Rules>(p, Color), not_occupied(p));
        }

private:
        // tag dispatching on restrictions on consecutive moves with the same king
        static void generate_serial(BitBoard active_kings, BitBoard not_occupied, Stack& moves)
        {
                generate_serial(
                        active_kings, not_occupied, moves, 
                        Int2Type<rules::is_restricted_same_king_moves<Rules>::value>()
                );
        }

        // partial specialization for unrestricted consecutive moves with the same king
        static void generate_serial(
                BitBoard active_kings, BitBoard not_occupied, Stack& moves, Int2Type<false>
        )
        {
                // loop cannot be empty because all active kings detected during 
                // Dispatcher<Selection, Rules, Board>::select are unrestricted to move
                BOOST_ASSERT(!bit::is_zero(active_kings));
                do {
                        generate_dirs(bit::get_first(active_kings), not_occupied, moves);
                        bit::clear_first(active_kings);
                } while (active_kings);
        }

        // partial specialization for restricted consecutive moves with the same king
        static void generate_serial(
                BitBoard active_kings, BitBoard not_occupied, Stack& moves, Int2Type<true>
        )
        {
                // loop could be empty if the single active king detected during 
                // Dispatcher<Selection, Rules, Board>::select is restricted to move
                while (active_kings) {
                        generate_dirs(bit::get_first(active_kings), not_occupied, moves);
                        bit::clear_first(active_kings);
                }
        }
        
        // typedefs
        typedef board::Direction<Color, Board> Direction;        

        static void generate_dirs(BitBoard from_sq, BitBoard not_occupied, Stack& moves)
        {
                generate_dir<Direction::left_down >(from_sq, not_occupied, moves);
                generate_dir<Direction::right_down>(from_sq, not_occupied, moves);
                generate_dir<Direction::left_up   >(from_sq, not_occupied, moves);
                generate_dir<Direction::right_up  >(from_sq, not_occupied, moves);
        }
        
        static int count_dirs(BitBoard active_kings, BitBoard not_occupied)
        {
                return (
                        count_dir<Direction::left_down >(active_kings, not_occupied) +
                        count_dir<Direction::right_down>(active_kings, not_occupied) +
                        count_dir<Direction::left_up   >(active_kings, not_occupied) +
                        count_dir<Direction::right_up  >(active_kings, not_occupied)
                );
        }
        
        static bool detect_dirs(BitBoard active_kings, BitBoard not_occupied)
        {
                return (
                        detect_dir<Direction::left_down >(active_kings, not_occupied) ||
                        detect_dir<Direction::right_down>(active_kings, not_occupied) ||
                        detect_dir<Direction::left_up   >(active_kings, not_occupied) ||
                        detect_dir<Direction::right_up  >(active_kings, not_occupied)
                );
        }

        // tag dispatching on king range
        template<int Index> 
        static void generate_dir(BitBoard from_sq, BitBoard not_occupied, Stack& moves)
        {
                return generate_dir<Index>(
                        from_sq, not_occupied, moves, 
                        Int2Type<rules::is_long_king_range<Rules>::value>()
                );
        }

        // partial specialization for short ranged kings
        template<int Index> 
        static void generate_dir(
                BitBoard from_sq, BitBoard not_occupied, Stack& moves, Int2Type<rules::range_1>
        )
        {
                if (auto dest_sq = Push<Board, Index>()(from_sq) & not_occupied)
                        push<Color>(from_sq ^ dest_sq, moves);
        }

        // partial specialization for long ranged kings
        template<int Index> 
        static void generate_dir(
                BitBoard from_sq, BitBoard not_occupied, Stack& moves, Int2Type<rules::range_N>
        )
        {
                for (
                        auto dest_sq = Push<Board, Index>()(from_sq); 
                        dest_sq & not_occupied; 
                        PushAssign<Board, Index>()(dest_sq)
                )
                        push<Color>(from_sq ^ dest_sq, moves);
        }

        template<int Index> 
        static int count_dir(BitBoard active_kings, BitBoard not_occupied)
        {
                return bit::count(
                        Destinations<
                                Moves, Board, Index, 
                                rules::is_long_king_range<Rules>::value
                        >()(active_kings, not_occupied)
                );
        }

        template<int Index> 
        static bool detect_dir(BitBoard active_kings, BitBoard not_occupied)
        {
                return !bit::is_zero(
                        Destinations<
                                Moves, Board, Index, 
                                rules::range_1
                        >()(active_kings, not_occupied)
                );
        }
};

}       // namespace successor
}       // namespace dctl
