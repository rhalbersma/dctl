#include <boost/assert.hpp>
#include "../capture/State.h"
#include "../rules/Rules.h"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::BOTH, Jumps, Rules, Board>::generate(const Position<Board>& p, Stack& moves)
{
        capture::State<Rules, Board> capture(p);
        generate(p, capture, moves);
}

template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::BOTH, Jumps, Rules, Board>::generate(const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves)
{
        generate_precede(p, capture, moves);
}

// tag dispatching on absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::BOTH, Jumps, Rules, Board>::generate_precede(const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves)
{
        generate_precede(p, capture, moves, Int2Type<rules::is_absolute_king_precedence<Rules>::value>());
}

// partial specialization for no absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::BOTH, Jumps, Rules, Board>::generate_precede(const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<false>)
{
        KingJumps::generate(p, capture, moves);
        PawnJumps::generate(p, capture, moves);
}

// partial specialization for absolute king capture precedence
template<bool Color, typename Rules, typename Board>
void Driver<Color, Material::BOTH, Jumps, Rules, Board>::generate_precede(const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<true>)
{
        KingJumps::generate(p, capture, moves);
        if (moves.empty())
                PawnJumps::generate(p, capture, moves);
}

template<bool Color, typename Rules, typename Board>
int Driver<Color, Material::BOTH, Jumps, Rules, Board>::count(const Position<Board>& p)
{
        Stack moves;
        generate(p, moves);
        return static_cast<int>(moves.size());
}

template<bool Color, typename Rules, typename Board>
bool Driver<Color, Material::BOTH, Jumps, Rules, Board>::detect(const Position<Board>& p)
{
        return (
                PawnJumps::detect(p) || 
                KingJumps::detect(p)
        );
}

}       // namespace successor
}       // namespace dctl
