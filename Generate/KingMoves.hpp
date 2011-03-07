#include "../Geometry/Board.h"
#include "../Geometry/Direction.h"
#include "../Geometry/PushPull.h"
#include "../Position/Position.h"
#include "../Utilities/Bit.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

using namespace Geometry::Direction;

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate(const Position<Board>& p, Move::List& move_list)
{
        generate_serial(p.template unrestricted_kings<Rules>(Color), p.not_occupied(), move_list);
}

// tag dispatching for restrictions on consecutive move_list with the same king
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate_serial(BitBoard active_kings, BitBoard not_occupied, Move::List& move_list)
{
        generate_serial(active_kings, not_occupied, move_list, Int2Type<Variants::is_RestrictedSameKingMoves<Rules>::VALUE>());
}

// partial specialization for restricted consecutive move_list with the same king
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate_serial(BitBoard active_kings, BitBoard not_occupied, Move::List& move_list, Int2Type<true>)
{
        // loop could be empty if the single active king detected during select_strategy() is restricted to move
        while (active_kings) {
                generate_dirs(Bit::get_lowest(active_kings), not_occupied, move_list);
                Bit::clear_lowest(active_kings);
        }
}

// partial specialization for unrestricted consecutive move_list with the same king
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate_serial(BitBoard active_kings, BitBoard not_occupied, Move::List& move_list, Int2Type<false>)
{
        // loop cannot be empty because all active kings detected during select_strategy() are unrestricted to move
        assert(!Bit::is_zero(active_kings));
        do {
                generate_dirs(Bit::get_lowest(active_kings), not_occupied, move_list);
                Bit::clear_lowest(active_kings);
        } while (active_kings);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate_dirs(BitBoard from_sq, BitBoard not_occupied, Move::List& move_list)
{
        generate_dir<Indices<Board, Color>::LEFT_DOWN >(from_sq, not_occupied, move_list);
        generate_dir<Indices<Board, Color>::RIGHT_DOWN>(from_sq, not_occupied, move_list);
        generate_dir<Indices<Board, Color>::LEFT_UP   >(from_sq, not_occupied, move_list);
        generate_dir<Indices<Board, Color>::RIGHT_UP  >(from_sq, not_occupied, move_list);
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate_dir(BitBoard from_sq, BitBoard not_occupied, Move::List& move_list)
{
        return generate_dir<Index>(from_sq, not_occupied, move_list, Int2Type<Variants::is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate_dir(BitBoard from_sq, BitBoard not_occupied, Move::List& move_list, Int2Type<Variants::RANGE_1>)
{
        if (BitBoard dest_sq = Push<Board, Index>()(from_sq) & not_occupied)
                move_list.push_back<Color>(from_sq ^ dest_sq);
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate_dir(BitBoard from_sq, BitBoard not_occupied, Move::List& move_list, Int2Type<Variants::RANGE_N>)
{
        for (BitBoard dest_sq = Push<Board, Index>()(from_sq); dest_sq & not_occupied; PushAssign<Board, Index>()(dest_sq))
                move_list.push_back<Color>(from_sq ^ dest_sq);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::count(const Position<Board>& p)
{
        return count_dirs(p.template unrestricted_kings<Rules>(Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::count_dirs(BitBoard active_kings, BitBoard not_occupied)
{
        return (
                count_dir<Indices<Board, Color>::LEFT_DOWN >(active_kings, not_occupied) +
                count_dir<Indices<Board, Color>::RIGHT_DOWN>(active_kings, not_occupied) +
                count_dir<Indices<Board, Color>::LEFT_UP   >(active_kings, not_occupied) +
                count_dir<Indices<Board, Color>::RIGHT_UP  >(active_kings, not_occupied)
        );
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
size_t Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::count_dir(BitBoard active_kings, BitBoard not_occupied)
{
        return count_dir<Index>(active_kings, not_occupied, Int2Type<Variants::is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
size_t Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::count_dir(BitBoard active_kings, BitBoard not_occupied, Int2Type<Variants::RANGE_1>)
{
        return Bit::count(Push<Board, Index>()(active_kings) & not_occupied);
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
size_t Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::count_dir(BitBoard active_kings, BitBoard not_occupied, Int2Type<Variants::RANGE_N>)
{
        return Bit::count(active_kings ^ FloodFill<Board, Index>()(active_kings, not_occupied));
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::detect(const Position<Board>& p)
{
        return detect_dirs(p.template unrestricted_kings<Rules>(Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard not_occupied)
{
        return (
                detect_dir<Indices<Board, Color>::LEFT_DOWN >(active_kings, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT_DOWN>(active_kings, not_occupied) ||
                detect_dir<Indices<Board, Color>::LEFT_UP   >(active_kings, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT_UP  >(active_kings, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::KING, Move::MOVES, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard not_occupied)
{
        return !Bit::is_zero(Push<Board, Index>()(active_kings) & not_occupied);
}
