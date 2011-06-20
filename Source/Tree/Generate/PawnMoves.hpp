#include <cassert>
#include "../Node/Position.h"
#include "../../Geometry/Board.h"
#include "../../Geometry/Direction.h"
#include "../../Geometry/PushPull.h"
#include "../../Utilities/Bit.h"

using namespace geometry::direction;

namespace tree {
namespace generate {

template<bool Color, typename Rules, typename Board>
void Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::generate(const node::Position<Board>& p, node::Stack* move_stack)
{
        generate_dirs(p.men(Color), p.not_occupied(), move_stack);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::generate_dirs(BitBoard active_men, BitBoard not_occupied, node::Stack* move_stack)
{
        generate_dir<Indices<Board, Color>::LEFT_UP >(active_men, not_occupied, move_stack);
        generate_dir<Indices<Board, Color>::RIGHT_UP>(active_men, not_occupied, move_stack);
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
void Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::generate_dir(BitBoard active_men, BitBoard not_occupied, node::Stack* move_stack)
{
        BitBoard from_sq, dest_sq;
        for (active_men &= Pull<Board, Index>()(not_occupied); active_men; bit::clear_lowest(active_men)) {
                from_sq = bit::get_lowest(active_men);
                dest_sq = Push<Board, Index>()(from_sq);
                node::push<Color>(from_sq ^ dest_sq, promotion(dest_sq), move_stack);
        }
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::generate_reverse(const node::Position<Board>& p, node::Stack* move_stack)
{
        generate_reverse_dirs(p.men(Color), p.not_occupied(), move_stack);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::generate_reverse_dirs(BitBoard active_men, BitBoard not_occupied, node::Stack* move_stack)
{
        generate_dir<Indices<Board, Color>::LEFT_DOWN >(active_men, not_occupied, move_stack);
        generate_dir<Indices<Board, Color>::RIGHT_DOWN>(active_men, not_occupied, move_stack);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::generate_promotions(const node::Position<Board>& p, node::Stack* move_stack)
{
        generate_dirs(promotors(p.men(Color)), p.not_occupied(), move_stack);
}

template<bool Color, typename Rules, typename Board>
BitBoard Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::promotion(BitBoard dest_sq)
{
        return dest_sq & Board::PROMOTION[Color][0];
}

template<bool Color, typename Rules, typename Board>
BitBoard Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::promotors(BitBoard active_men)
{
        return active_men & Board::PROMOTION[Color][1];
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::count(const node::Position<Board>& p)
{
        return count_dirs(p.men(Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::count_dirs(BitBoard active_men, BitBoard not_occupied)
{
        return (
                count_dir<Indices<Board, Color>::LEFT_UP >(active_men, not_occupied) +
                count_dir<Indices<Board, Color>::RIGHT_UP>(active_men, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
size_t Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::count_dir(BitBoard active_men, BitBoard not_occupied)
{
        return bit::count(active_men & Pull<Board, Index>()(not_occupied));
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::count_reverse(const node::Position<Board>& p)
{
        return count_reverse_dirs(p.men(Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::count_reverse_dirs(BitBoard active_men, BitBoard not_occupied)
{
        return (
                count_dir<Indices<Board, Color>::LEFT_DOWN >(active_men, not_occupied) +
                count_dir<Indices<Board, Color>::RIGHT_DOWN>(active_men, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board>
size_t Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::count_promotions(const node::Position<Board>& p)
{
        return count_dirs(promotors(p.men(Color)), p.not_occupied());
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::detect(const node::Position<Board>& p)
{
        return detect_dirs(p.men(Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::detect_dirs(BitBoard active_men, BitBoard not_occupied)
{
        return (
                detect_dir<Indices<Board, Color>::LEFT_UP >(active_men, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT_UP>(active_men, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<size_t Index>
bool Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::detect_dir(BitBoard active_men, BitBoard not_occupied)
{
        return !bit::is_zero(active_men & Pull<Board, Index>()(not_occupied));
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::detect_reverse(const node::Position<Board>& p)
{
        return detect_reverse_dirs(p.men(Color), p.not_occupied());
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::detect_reverse_dirs(BitBoard active_men, BitBoard not_occupied)
{
        return (
                detect_dir<Indices<Board, Color>::LEFT_DOWN >(active_men, not_occupied) ||
                detect_dir<Indices<Board, Color>::RIGHT_DOWN>(active_men, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, node::Pieces::PAWN, node::MOVES, Rules, Board>::detect_promotions(const node::Position<Board>& p)
{
        return detect_dirs(promotors(p.men(Color)), p.not_occupied());
}

}       // namespace generate
}       // namespace tree
