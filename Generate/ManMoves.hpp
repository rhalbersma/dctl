#include "Propagate.h"
#include "../Board/Board.h"
#include "../Board/Direction.h"
#include "../Position/Position.h"
#include "../Utilities/Bit.h"
#include "../Utilities/InlineOptions.h"
#include "../Utilities/Shift.h"
#include <cassert>

template<typename Rules, typename Board>
void ManMoves::generate(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        if (p.to_move())
		generate<Side::WHITE>(p, moves);
        else
                generate<Side::BLACK>(p, moves);
}

template<bool Color, typename Rules, typename Board>
void ManMoves::generate(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        generate_dirs<Color>(p.men(Color), moves);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void ManMoves::generate_dirs(BitBoard active_men, Propagate<Rules, Board>& moves)
{
        generate_dir<Color, DirIndex<Color>::LEFT_UP>(active_men, moves);
        generate_dir<Color, DirIndex<Color>::RIGHT_UP>(active_men, moves);
}

template<bool Color, size_t Index, typename Rules, typename Board> FORCE_INLINE
void ManMoves::generate_dir(BitBoard active_men, Propagate<Rules, Board>& moves)
{
        BitBoard from_sq, dest_sq;
        for (active_men &= moves.template movers<Index>(); active_men; Bit::clear_lowest(active_men)) {
                from_sq = Bit::get_lowest(active_men);
                dest_sq = Shift<Direction<Index>::IS_POSITIVE>()(from_sq, Board::DIR[Index]);
                moves.template add_man_move<Color>(from_sq, dest_sq);
        }
}

template<typename Rules, typename Board>
void ManMoves::generate_promotions(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        if (p.to_move())
                generate_promotions<Side::WHITE>(p, moves);
        else
                generate_promotions<Side::BLACK>(p, moves);
}

template<bool Color, typename Rules, typename Board>
void ManMoves::generate_promotions(const Position<Board>& p, Propagate<Rules, Board>& moves)
{
        assert(p.to_move() == Color);
        generate_dirs<Color>(promotors<Color>(p), moves);
}

template<bool Color, typename Board> FORCE_INLINE
BitBoard ManMoves::promotors(const Position<Board>& p)
{
        return p.men(Color) & Board::PROMOTION[Color][1];
}

template<typename Board>
size_t ManMoves::count(const Position<Board>& p)
{
        if (p.to_move())
                return count<Side::WHITE>(p);
        else
                return count<Side::BLACK>(p);
}

template<bool Color, typename Board>
size_t ManMoves::count(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return count_dirs<Color, Board>(p.men(Color), p.not_occupied());
}

template<bool Color, typename Board> FORCE_INLINE
size_t ManMoves::count_dirs(BitBoard active_men, BitBoard not_occupied)
{
        return (
                count_dir<DirIndex<Color>::LEFT_UP, Board>(active_men, not_occupied) +
                count_dir<DirIndex<Color>::RIGHT_UP, Board>(active_men, not_occupied)
        );
}

template<size_t Index, typename Board> FORCE_INLINE
size_t ManMoves::count_dir(BitBoard active_men, BitBoard not_occupied)
{
        return Bit::count(active_men & Shift<Direction<Index>::IS_NEGATIVE>()(not_occupied, Board::DIR[Index]));
}

template<typename Board>
bool ManMoves::detect(const Position<Board>& p)
{
        if (p.to_move())
                return detect<Side::WHITE>(p);
        else
                return detect<Side::BLACK>(p);
}

template<bool Color, typename Board>
bool ManMoves::detect(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return detect_dirs<Color, Board>(p.men(Color), p.not_occupied());
}

template<bool Color, typename Board> FORCE_INLINE
bool ManMoves::detect_dirs(BitBoard active_men, BitBoard not_occupied)
{
        return (
                detect_dir<DirIndex<Color>::LEFT_UP, Board>(active_men, not_occupied) ||
                detect_dir<DirIndex<Color>::RIGHT_UP, Board>(active_men, not_occupied)
        );
}

template<size_t Index, typename Board> FORCE_INLINE
bool ManMoves::detect_dir(BitBoard active_men, BitBoard not_occupied)
{
        return !Bit::is_zero(active_men & Shift<Direction<Index>::IS_NEGATIVE>()(not_occupied, Board::DIR[Index]));
}

template<typename Board>
bool ManMoves::detect_promotions(const Position<Board>& p)
{
        if (p.to_move())
                return detect_promotions<Side::WHITE>(p);
        else
                return detect_promotions<Side::BLACK>(p);
}

template<bool Color, typename Board>
bool ManMoves::detect_promotions(const Position<Board>& p)
{
        assert(p.to_move() == Color);
        return detect_dirs<Color, Board>(promotors<Color>(p), p.not_occupied());
}
