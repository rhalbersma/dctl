#include "Propagate.h"
#include "../Board/Board.h"
#include "../Board/Direction.h"
#include "../Position/Position.h"
#include "../Utilities/Bit.h"
#include "../Utilities/Shift.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::generate(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        generate_dirs(p.men(Color), moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::generate_dirs(BitBoard active_men, Propagate<Rules, Board>& moves)
{
        generate_dir<DirIndex<Board, Color>::LEFT_UP >(active_men, moves);
        generate_dir<DirIndex<Board, Color>::RIGHT_UP>(active_men, moves);
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::generate_dir(BitBoard active_men, Propagate<Rules, Board>& moves)
{
        BitBoard from_sq, dest_sq;
        for (active_men &= moves.template man_movers<Index>(); active_men; Bit::clear_lowest(active_men)) {
                from_sq = Bit::get_lowest(active_men);
                dest_sq = Shift<DirTraits<Index>::IS_POSITIVE>()(from_sq, Board::DIR[Index]);
                moves.template add_man_move<Color>(from_sq, dest_sq);
        }
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::generate_promotions(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        generate_dirs(promotors(p), moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
BitBoard GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::promotors(const Position<Board>& p)
{
        return p.men(Color) & Board::PROMOTION[Color][1];
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::count(const Position<Board>& p)
{
        return count_dirs(p.men(Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::count_dirs(BitBoard active_men, BitBoard not_occupied)
{
        return (
                count_dir<DirIndex<Board, Color>::LEFT_UP >(active_men, not_occupied) +
                count_dir<DirIndex<Board, Color>::RIGHT_UP>(active_men, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
size_t GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::count_dir(BitBoard active_men, BitBoard not_occupied)
{
        return Bit::count(active_men & Shift<DirTraits<Index>::IS_NEGATIVE>()(not_occupied, Board::DIR[Index]));
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::detect(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return detect_dirs(p.men(Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::detect_dirs(BitBoard active_men, BitBoard not_occupied)
{
        return (
                detect_dir<DirIndex<Board, Color>::LEFT_UP >(active_men, not_occupied) ||
                detect_dir<DirIndex<Board, Color>::RIGHT_UP>(active_men, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::detect_dir(BitBoard active_men, BitBoard not_occupied)
{
        return !Bit::is_zero(active_men & Shift<DirTraits<Index>::IS_NEGATIVE>()(not_occupied, Board::DIR[Index]));
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool GenerateDriver<Pieces::PAWN, Move::MOVES, Color, Rules, Board>::detect_promotions(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return detect_dirs(promotors(p), p.not_occupied());
}
