#include <cassert>
#include "KingJumps.h"
#include "PawnJumps.h"
#include "../capture/State.h"
#include "../rules/Rules.h"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::BOTH, Move::JUMPS, Rules, Board>::generate_regular(const Position<Board>& p, Stack& moves)
{
        capture::State<Rules, Board> capture(p);
        generate_regular(p, capture, moves);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::BOTH, Move::JUMPS, Rules, Board>::generate_regular(const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves)
{
        generate_precede(p, capture, moves);
}

// tag dispatching based on absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves)
{
        generate_precede(p, capture, moves, Int2Type<rules::is_absolute_king_precedence<Rules>::value>());
}

// partial specialization for no absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<false>)
{
        Driver<Color, Material::KING, Move::JUMPS, Rules, Board>::generate_regular(p, capture, moves);
        Driver<Color, Material::PAWN, Move::JUMPS, Rules, Board>::generate_regular(p, capture, moves);
}

// partial specialization for absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::BOTH, Move::JUMPS, Rules, Board>::generate_precede(const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<true>)
{
        Driver<Color, Material::KING, Move::JUMPS, Rules, Board>::generate_regular(p, capture, moves);
        if (moves.empty())
                Driver<Color, Material::PAWN, Move::JUMPS, Rules, Board>::generate_regular(p, capture, moves);
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::BOTH, Move::JUMPS, Rules, Board>::count_regular(const Position<Board>& p)
{
        Stack moves;
        generate_regular(p, moves);
        return static_cast<int>(moves.size());
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::BOTH, Move::JUMPS, Rules, Board>::detect_regular(const Position<Board>& p)
{
        return (
                Driver<Color, Material::PAWN, Move::JUMPS, Rules, Board>::detect_regular(p) || 
                Driver<Color, Material::KING, Move::JUMPS, Rules, Board>::detect_regular(p)
        );
}

}       // namespace successor
}       // namespace dctl
