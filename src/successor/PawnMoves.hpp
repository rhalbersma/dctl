#include <cassert>
#include "../node/Position.h"
#include "../board/Board.h"
#include "../board/Direction.h"
#include "../bit/Bit.h"
#include "../utils/Shift.h"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Moves, Rules, Board>::generate(
        const Position<Board>& p, Stack& moves
)
{
        generate_dirs(p.men(Color), not_occupied(p), moves);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Moves, Rules, Board>::generate_dirs(
        BitBoard active_men, BitBoard not_occupied, Stack& moves
)
{
        generate_dir<Direction::LEFT_UP >(active_men, not_occupied, moves);
        generate_dir<Direction::RIGHT_UP>(active_men, not_occupied, moves);
}

template<bool Color, typename Rules, typename Board> template<int Index>
void Driver<Color, Material::PAWN, Moves, Rules, Board>::generate_dir(
        BitBoard active_men, BitBoard not_occupied, Stack& moves
)
{
        BitBoard from_sq, dest_sq;
        for (active_men &= Pull<Board, Index>()(not_occupied); active_men; bit::clear_first(active_men)) {
                from_sq = bit::get_first(active_men);
                dest_sq = Push<Board, Index>()(from_sq);
                push<Color>(from_sq ^ dest_sq, promotion<Color, Board>(dest_sq), moves);
        }
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::PAWN, Moves, Rules, Board>::count(
        const Position<Board>& p
)
{
        return count_dirs(p.men(Color), not_occupied(p));
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::PAWN, Moves, Rules, Board>::count_dirs(
        BitBoard active_men, BitBoard not_occupied
)
{
        return (
                count_dir<Direction::LEFT_UP >(active_men, not_occupied) +
                count_dir<Direction::RIGHT_UP>(active_men, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<int Index>
int Driver<Color, Material::PAWN, Moves, Rules, Board>::count_dir(
        BitBoard active_men, BitBoard not_occupied
)
{
        return bit::count(active_men & Pull<Board, Index>()(not_occupied));
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Moves, Rules, Board>::detect(
        const Position<Board>& p
)
{
        return detect_dirs(p.men(Color), not_occupied(p));
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Moves, Rules, Board>::detect_dirs(
        BitBoard active_men, BitBoard not_occupied
)
{
        return (
                detect_dir<Direction::LEFT_UP >(active_men, not_occupied) ||
                detect_dir<Direction::RIGHT_UP>(active_men, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board> template<int Index>
bool Driver<Color, Material::PAWN, Moves, Rules, Board>::detect_dir(
        BitBoard active_men, BitBoard not_occupied
)
{
        return !bit::is_zero(active_men & Pull<Board, Index>()(not_occupied));
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Reverse, Rules, Board>::generate(
        const Position<Board>& p, Stack& moves
)
{
        generate_dirs(p.men(Color), not_occupied(p), moves);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Reverse, Rules, Board>::generate_dirs(
        BitBoard active_men, BitBoard not_occupied, Stack& moves
)
{
        Regular::generate_dir<Direction::LEFT_DOWN >(active_men, not_occupied, moves);
        Regular::generate_dir<Direction::RIGHT_DOWN>(active_men, not_occupied, moves);
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::PAWN, Reverse, Rules, Board>::count(
        const Position<Board>& p
)
{
        return count_dirs(p.men(Color), not_occupied(p));
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::PAWN, Reverse, Rules, Board>::count_dirs(
        BitBoard active_men, BitBoard not_occupied
)
{
        return (
                Regular::count_dir<Direction::LEFT_DOWN >(active_men, not_occupied) +
                Regular::count_dir<Direction::RIGHT_DOWN>(active_men, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Reverse, Rules, Board>::detect(
        const Position<Board>& p
)
{
        return detect_dirs(p.men(Color), not_occupied(p));
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Reverse, Rules, Board>::detect_dirs(
        BitBoard active_men, BitBoard not_occupied
)
{
        return (
                Regular::detect_dir<Direction::LEFT_DOWN >(active_men, not_occupied) ||
                Regular::detect_dir<Direction::RIGHT_DOWN>(active_men, not_occupied)
        );
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::PAWN, Promotion, Rules, Board>::generate(const Position<Board>& p, Stack& moves)
{
        Regular::generate_dirs(promotors<Color, Board>(p.men(Color)), not_occupied(p), moves);
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::PAWN, Promotion, Rules, Board>::count(const Position<Board>& p)
{
        return Regular::count_dirs(promotors<Color, Board>(p.men(Color)), not_occupied(p));
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::PAWN, Promotion, Rules, Board>::detect(const Position<Board>& p)
{
        return Regular::detect_dirs(promotors<Color, Board>(p.men(Color)), not_occupied(p));
}

template<bool Color, typename Board>
BitBoard promotion(BitBoard dest_sq)
{
        return dest_sq & Board::PROMOTION[Color][0];
}

template<bool Color, typename Board>
BitBoard promotors(BitBoard active_men)
{
        return active_men & Board::PROMOTION[Color][1];
}

}       // namespace successor
}       // namespace dctl
