#include "Propagate.h"
#include "../Board/Board.h"
#include "../Board/Direction.h"
#include "../Position/Position.h"
#include "../Utilities/Bit.h"
#include "../Utilities/Shift.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        generate_serial(p.template unrestricted_kings<Rules>(Color), moves);
}

// tag dispatching for restrictions on consecutive moves with the same king
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate_serial(BitBoard active_kings, Propagate<Rules, Board>& moves)
{
        generate_serial(active_kings, moves, Int2Type<is_RestrictedSameKingMoves<Rules>::VALUE>());
}

// partial specialization for restricted consecutive moves with the same king
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate_serial(BitBoard active_kings, Propagate<Rules, Board>& moves, Int2Type<true>)
{
        while (active_kings) {
                generate_dirs(Bit::get_lowest(active_kings), moves);
                Bit::clear_lowest(active_kings);
        }
}

// partial specialization for unrestricted consecutive moves with the same king
template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate_serial(BitBoard active_kings, Propagate<Rules, Board>& moves, Int2Type<false>)
{
        assert(!Bit::is_zero(active_kings));
        do {
                generate_dirs(Bit::get_lowest(active_kings), moves);
                Bit::clear_lowest(active_kings);
        } while (active_kings);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate_dirs(BitBoard from_sq, Propagate<Rules, Board>& moves)
{
        generate_dir<DirIndex<Board, Color>::LEFT_DOWN >(from_sq, moves);
        generate_dir<DirIndex<Board, Color>::RIGHT_DOWN>(from_sq, moves);
        generate_dir<DirIndex<Board, Color>::LEFT_UP   >(from_sq, moves);
        generate_dir<DirIndex<Board, Color>::RIGHT_UP  >(from_sq, moves);
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate_dir(BitBoard from_sq, Propagate<Rules, Board>& moves)
{
        return generate_dir<Index>(from_sq, moves, Int2Type<is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate_dir(BitBoard from_sq, Propagate<Rules, Board>& moves, Int2Type<RANGE_1>)
{
        if (BitBoard dest_sq = Shift<DirTraits<Index>::IS_POSITIVE>()(from_sq, Board::DIR[Index]) & moves.path())
                moves.template add_king_move<Color>(from_sq ^ dest_sq);
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::generate_dir(BitBoard from_sq, Propagate<Rules, Board>& moves, Int2Type<RANGE_N>)
{
        for (BitBoard dest_sq = Shift<DirTraits<Index>::IS_POSITIVE>()(from_sq, Board::DIR[Index]); dest_sq & moves.path(); ShiftAssign<DirTraits<Index>::IS_POSITIVE>()(dest_sq, Board::DIR[Index]))
                moves.template add_king_move<Color>(from_sq ^ dest_sq);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::count(const Position<Board>& p)
{
        return count_dirs(p.template unrestricted_kings<Rules>(Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::count_dirs(BitBoard active_kings, BitBoard not_occupied)
{
        return (
                count_dir<DirIndex<Board, Color>::LEFT_DOWN >(active_kings, not_occupied) +
                count_dir<DirIndex<Board, Color>::RIGHT_DOWN>(active_kings, not_occupied) +
                count_dir<DirIndex<Board, Color>::LEFT_UP   >(active_kings, not_occupied) +
                count_dir<DirIndex<Board, Color>::RIGHT_UP  >(active_kings, not_occupied)
        );
}

// tag dispatching based on king range
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
size_t GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::count_dir(BitBoard active_kings, BitBoard not_occupied)
{
        return count_dir<Index>(active_kings, not_occupied, Int2Type<is_LongKingRange<Rules>::VALUE>());
}

// partial specialization for short ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
size_t GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::count_dir(BitBoard active_kings, BitBoard not_occupied, Int2Type<RANGE_1>)
{
        return Bit::count(Shift<DirTraits<Index>::IS_POSITIVE>()(active_kings, Board::DIR[Index]) & not_occupied);
}

// partial specialization for long ranged kings
template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
size_t GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::count_dir(BitBoard active_kings, BitBoard not_occupied, Int2Type<RANGE_N>)
{
        return Bit::count(active_kings ^ Bit::flood_fill<DirTraits<Index>::IS_POSITIVE>(active_kings, not_occupied, Board::DIR[Index]));
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::detect(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return detect_dirs(p.template unrestricted_kings<Rules>(Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::detect_dirs(BitBoard active_kings, BitBoard not_occupied)
{
        return (
                detect_dir<DirIndex<Board, Color>::LEFT_DOWN >(active_kings, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT_DOWN>(active_kings, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::LEFT_UP   >(active_kings, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT_UP  >(active_kings, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Color, Pieces::KING, Move::MOVES, Rules, Board>::detect_dir(BitBoard active_kings, BitBoard not_occupied)
{
        return !Bit::is_zero(Shift<DirTraits<Index>::IS_POSITIVE>()(active_kings, Board::DIR[Index]) & not_occupied);
}
