#include <cassert>
#include "../node/Position.h"
#include "../board/Direction.h"

namespace dctl {
namespace successor {

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

}       // namespace successor
}       // namespace dctl
