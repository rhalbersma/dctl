#include "../Geometry/Board.h"
#include "../Geometry/Direction.h"
#include "../Geometry/PushPull.h"
#include "../Position/Position.h"
#include "../Utilities/Bit.h"
#include "../Utilities/InlineOptions.h"
#include <cassert>

using namespace Geometry::Direction;

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::generate(const Position<Board>& p, Move::List& move_list)
{
        generate_dirs(p.men(Color), p.not_occupied(), move_list);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::generate_dirs(BitBoard active_men, BitBoard not_occupied, Move::List& move_list)
{
        generate_dir<Indices<Board, Color>::LEFT_UP >(active_men, not_occupied, move_list);
        generate_dir<Indices<Board, Color>::RIGHT_UP>(active_men, not_occupied, move_list);
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
void Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::generate_dir(BitBoard active_men, BitBoard not_occupied, Move::List& move_list)
{
        BitBoard from_sq, dest_sq;
        for (active_men &= Pull<Board, Index>()(not_occupied); active_men; Bit::clear_lowest(active_men)) {
                from_sq = Bit::get_lowest(active_men);
                dest_sq = Push<Board, Index>()(from_sq);
                move_list.push_back<Color>(from_sq ^ dest_sq, promotion(dest_sq));
        }
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
void Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::generate_promotions(const Position<Board>& p, Move::List& move_list)
{
        generate_dirs(promotors(p.men(Color)), p.not_occupied(), move_list);
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
BitBoard Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::promotion(BitBoard dest_sq)
{
        return dest_sq & Board::PROMOTION[Color][0];
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
BitBoard Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::promotors(BitBoard active_men)
{
        return active_men & Board::PROMOTION[Color][1];
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::count(const Position<Board>& p)
{
        return count_dirs(p.men(Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::count_dirs(BitBoard active_men, BitBoard not_occupied)
{
        return (
                count_dir<Indices<Board, Color>::LEFT_UP >(active_men, not_occupied) +
                count_dir<Indices<Board, Color>::RIGHT_UP>(active_men, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
size_t Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::count_dir(BitBoard active_men, BitBoard not_occupied)
{
        return Bit::count(active_men & Pull<Board, Index>()(not_occupied));
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
size_t Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::count_promotions(const Position<Board>& p)
{
        return count_dirs(promotors(p.men(Color)), p.not_occupied());
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::detect(const Position<Board>& p)
{
        return detect_dirs(p.men(Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::detect_dirs(BitBoard active_men, BitBoard not_occupied)
{
        return (
                detect_dir<Indices<Board, Color>::LEFT_UP >(active_men, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT_UP>(active_men, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index> FORCE_INLINE
bool Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::detect_dir(BitBoard active_men, BitBoard not_occupied)
{
        return !Bit::is_zero(active_men & Pull<Board, Index>()(not_occupied));
}

template<bool Color, typename Rules, typename Board> FORCE_INLINE
bool Template<Color, Pieces::PAWN, Move::MOVES, Rules, Board>::detect_promotions(const Position<Board>& p)
{
        return detect_dirs(promotors(p.men(Color)), p.not_occupied());
}
