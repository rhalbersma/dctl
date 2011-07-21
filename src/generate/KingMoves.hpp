#include <cassert>
#include "../node/Position.h"
#include "../board/Board.h"
#include "../board/Direction.h"
#include "../utils/Bit.h"
#include "../utils/Shift.h"

namespace dtl {
namespace generate {

template<bool Color, typename Rules, typename Board>
void Driver<Color, Pieces::KING, MOVES, Rules, Board>::generate(const Position<Board>& p, Stack& move_stack)
{
        generate_serial(unrestricted_kings<Rules>(p, Color), p.not_occupied(), move_stack);
}

// tag dispatching based on restrictions on consecutive moves with the same king
template<bool Color, typename Rules, typename Board>
void Driver<Color, Pieces::KING, MOVES, Rules, Board>::generate_serial(BitBoard active_kings, BitBoard not_occupied, Stack& move_stack)
{
        generate_serial(active_kings, not_occupied, move_stack, Int2Type<rules::is_restricted_same_king_moves<Rules>::value>());
}

// partial specialization for restricted consecutive moves with the same king
template<bool Color, typename Rules, typename Board>
void Driver<Color, Pieces::KING, MOVES, Rules, Board>::generate_serial(BitBoard active_kings, BitBoard not_occupied, Stack& move_stack, Int2Type<true>)
{
        // loop could be empty if the single active king detected during Successors<Rules, Board>::select is restricted to move
        while (active_kings) {
                generate_dirs(bit::get_first(active_kings), not_occupied, move_stack);
                bit::clear_first(active_kings);
        }
}

// partial specialization for unrestricted consecutive moves with the same king
template<bool Color, typename Rules, typename Board>
void Driver<Color, Pieces::KING, MOVES, Rules, Board>::generate_serial(BitBoard active_kings, BitBoard not_occupied, Stack& move_stack, Int2Type<false>)
{
        // loop cannot be empty because all active kings detected during Successors<Rules, Board>::select() are unrestricted to move
        assert(!bit::is_zero(active_kings));
        do {
                generate_dirs(bit::get_first(active_kings), not_occupied, move_stack);
                bit::clear_first(active_kings);
        } while (active_kings);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, Pieces::KING, MOVES, Rules, Board>::generate_dirs(BitBoard from_sq, BitBoard not_occupied, Stack& move_stack)
{
        generate_dir<board::Direction<Color, Board>::LEFT_DOWN >(from_sq, not_occupied, move_stack);
        generate_dir<board::Direction<Color, Board>::RIGHT_DOWN>(from_sq, not_occupied, move_stack);
        generate_dir<board::Direction<Color, Board>::LEFT_UP   >(from_sq, not_occupied, move_stack);
        generate_dir<board::Direction<Color, Board>::RIGHT_UP  >(from_sq, not_occupied, move_stack);
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index>
void Driver<Color, Pieces::KING, MOVES, Rules, Board>::generate_dir(BitBoard from_sq, BitBoard not_occupied, Stack& move_stack)
{
        return generate_dir<Index>(from_sq, not_occupied, move_stack, Int2Type<rules::is_long_king_range<Rules>::value>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index>
void Driver<Color, Pieces::KING, MOVES, Rules, Board>::generate_dir(BitBoard from_sq, BitBoard not_occupied, Stack& move_stack, Int2Type<rules::RANGE_1>)
{
        if (BitBoard dest_sq = Push<Board, Index>()(from_sq) & not_occupied)
                push<Color>(from_sq ^ dest_sq, move_stack);
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index>
void Driver<Color, Pieces::KING, MOVES, Rules, Board>::generate_dir(BitBoard from_sq, BitBoard not_occupied, Stack& move_stack, Int2Type<rules::RANGE_N>)
{
        for (BitBoard dest_sq = Push<Board, Index>()(from_sq); dest_sq & not_occupied; PushAssign<Board, Index>()(dest_sq))
                push<Color>(from_sq ^ dest_sq, move_stack);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, Pieces::KING, MOVES, Rules, Board>::generate_reverse(const Position<Board>& p, Stack& move_stack)
{
        generate_serial(p.kings(Color), p.not_occupied(), move_stack);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, Pieces::KING, MOVES, Rules, Board>::generate_promotions(const Position<Board>&, Stack&) 
{ 
        return;
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, Pieces::KING, MOVES, Rules, Board>::count(const Position<Board>& p)
{
        return count_dirs(unrestricted_kings<Rules>(p, Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, Pieces::KING, MOVES, Rules, Board>::count_dirs(BitBoard active_kings, BitBoard not_occupied)
{
        return (
                count_dir<board::Direction<Color, Board>::LEFT_DOWN >(active_kings, not_occupied) +
                count_dir<board::Direction<Color, Board>::RIGHT_DOWN>(active_kings, not_occupied) +
                count_dir<board::Direction<Color, Board>::LEFT_UP   >(active_kings, not_occupied) +
                count_dir<board::Direction<Color, Board>::RIGHT_UP  >(active_kings, not_occupied)
        );
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index>
size_t Driver<Color, Pieces::KING, MOVES, Rules, Board>::count_dir(BitBoard active_kings, BitBoard not_occupied)
{
        return count_dir<Index>(active_kings, not_occupied, Int2Type<rules::is_long_king_range<Rules>::value>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index>
size_t Driver<Color, Pieces::KING, MOVES, Rules, Board>::count_dir(BitBoard active_kings, BitBoard not_occupied, Int2Type<rules::RANGE_1>)
{
        return bit::count(Push<Board, Index>()(active_kings) & not_occupied);
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index>
size_t Driver<Color, Pieces::KING, MOVES, Rules, Board>::count_dir(BitBoard active_kings, BitBoard not_occupied, Int2Type<rules::RANGE_N>)
{
        return bit::count(active_kings ^ FloodFill<Board, Index>()(active_kings, not_occupied));
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, Pieces::KING, MOVES, Rules, Board>::count_reverse(const Position<Board>& p)
{
        return count(p);
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, Pieces::KING, MOVES, Rules, Board>::count_promotions(const Position<Board>&) 
{ 
        return 0; 
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Pieces::KING, MOVES, Rules, Board>::detect(const Position<Board>& p)
{
        return detect_dirs(unrestricted_kings<Rules>(p, Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Pieces::KING, MOVES, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard not_occupied)
{
        return (
                detect_dir<board::Direction<Color, Board>::LEFT_DOWN >(active_kings, not_occupied) ||
                detect_dir<board::Direction<Color, Board>::RIGHT_DOWN>(active_kings, not_occupied) ||
                detect_dir<board::Direction<Color, Board>::LEFT_UP   >(active_kings, not_occupied) ||
                detect_dir<board::Direction<Color, Board>::RIGHT_UP  >(active_kings, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Driver<Color, Pieces::KING, MOVES, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard not_occupied)
{
        return !bit::is_zero(Push<Board, Index>()(active_kings) & not_occupied);
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Pieces::KING, MOVES, Rules, Board>::detect_reverse(const Position<Board>& p)
{
        return detect(p);
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Pieces::KING, MOVES, Rules, Board>::detect_promotions(const Position<Board>&) 
{ 
        return false; 
}

}       // namespace generate
}       // namespace dtl
