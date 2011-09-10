#include <boost/assert.hpp>
#include "../node/Position.h"
#include "../board/Direction.h"
#include "../bit/Bit.h"
#include "../utils/Shift.h"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::KING, Moves, Rules, Board>::generate(
        const Position<Board>& p, Stack& moves
)
{
        generate_serial(unrestricted_kings<Rules>(p, Color), not_occupied(p), moves);
}

// tag dispatching based on restrictions on consecutive moves with the same king
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::KING, Moves, Rules, Board>::generate_serial(
        BitBoard active_kings, BitBoard not_occupied, Stack& moves
)
{
        generate_serial(active_kings, not_occupied, moves, Int2Type<rules::is_restricted_same_king_moves<Rules>::value>());
}

// partial specialization for restricted consecutive moves with the same king
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::KING, Moves, Rules, Board>::generate_serial(
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

// partial specialization for unrestricted consecutive moves with the same king
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::KING, Moves, Rules, Board>::generate_serial(
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

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::KING, Moves, Rules, Board>::generate_dirs(
        BitBoard from_sq, BitBoard not_occupied, Stack& moves
)
{
        generate_dir<Direction::left_down >(from_sq, not_occupied, moves);
        generate_dir<Direction::right_down>(from_sq, not_occupied, moves);
        generate_dir<Direction::left_up   >(from_sq, not_occupied, moves);
        generate_dir<Direction::right_up  >(from_sq, not_occupied, moves);
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<int Index>
void Driver<Color, Material::KING, Moves, Rules, Board>::generate_dir(
        BitBoard from_sq, BitBoard not_occupied, Stack& moves
)
{
        return generate_dir<Index>(from_sq, not_occupied, moves, Int2Type<rules::is_long_king_range<Rules>::value>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<int Index>
void Driver<Color, Material::KING, Moves, Rules, Board>::generate_dir(
        BitBoard from_sq, BitBoard not_occupied, Stack& moves, Int2Type<rules::RANGE_1>
)
{
        if (auto dest_sq = Push<Board, Index>()(from_sq) & not_occupied)
                push<Color>(from_sq ^ dest_sq, moves);
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<int Index>
void Driver<Color, Material::KING, Moves, Rules, Board>::generate_dir(
        BitBoard from_sq, BitBoard not_occupied, Stack& moves, Int2Type<rules::RANGE_N>
)
{
        for (auto dest_sq = Push<Board, Index>()(from_sq); dest_sq & not_occupied; PushAssign<Board, Index>()(dest_sq))
                push<Color>(from_sq ^ dest_sq, moves);
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::KING, Moves, Rules, Board>::count(
        const Position<Board>& p
)
{
        return count_dirs(unrestricted_kings<Rules>(p, Color), not_occupied(p));
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::KING, Moves, Rules, Board>::count_dirs(
        BitBoard active_kings, BitBoard not_occupied
)
{
        return (
                count_dir<Direction::left_down >(active_kings, not_occupied) +
                count_dir<Direction::right_down>(active_kings, not_occupied) +
                count_dir<Direction::left_up   >(active_kings, not_occupied) +
                count_dir<Direction::right_up  >(active_kings, not_occupied)
        );
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<int Index>
int Driver<Color, Material::KING, Moves, Rules, Board>::count_dir(
        BitBoard active_kings, BitBoard not_occupied
)
{
        return count_dir<Index>(active_kings, not_occupied, Int2Type<rules::is_long_king_range<Rules>::value>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<int Index>
int Driver<Color, Material::KING, Moves, Rules, Board>::count_dir(
        BitBoard active_kings, BitBoard not_occupied, Int2Type<rules::RANGE_1>
)
{
        return bit::count(Push<Board, Index>()(active_kings) & not_occupied);
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<int Index>
int Driver<Color, Material::KING, Moves, Rules, Board>::count_dir(
        BitBoard active_kings, BitBoard not_occupied, Int2Type<rules::RANGE_N>
)
{
        return bit::count(active_kings ^ FloodFill<Board, Index>()(active_kings, not_occupied));
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::KING, Moves, Rules, Board>::detect(
        const Position<Board>& p
)
{
        return detect_dirs(unrestricted_kings<Rules>(p, Color), not_occupied(p));
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::KING, Moves, Rules, Board>::detect_dirs(
        BitBoard active_kings, BitBoard not_occupied
)
{
        return (
                detect_dir<Direction::left_down >(active_kings, not_occupied) ||
                detect_dir<Direction::right_down>(active_kings, not_occupied) ||
                detect_dir<Direction::left_up   >(active_kings, not_occupied) ||
                detect_dir<Direction::right_up  >(active_kings, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::KING, Moves, Rules, Board>::detect_dir(
        BitBoard active_kings, BitBoard not_occupied
)
{
        return !bit::is_zero(Push<Board, Index>()(active_kings) & not_occupied);
}

}       // namespace successor
}       // namespace dctl
