#pragma once
#include "Driver_fwd.hpp"
#include "KingJumps.hpp"
#include "PawnJumps.hpp"
#include "Selection.hpp"
#include "../capture/State.hpp"
#include "../node/Material.hpp"
#include "../node/Position_fwd.hpp"
#include "../node/Stack.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/NonConstructible.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::both, select::Jumps, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
private:
        // typedefs
        typedef Driver<Color, Material::king, select::Jumps, Rules, Board> KingJumps;
        typedef Driver<Color, Material::pawn, select::Jumps, Rules, Board> PawnJumps;
        typedef capture::State<Rules, Board> State;

public:
        static void generate(const Position<Board>& p, Stack& moves)
        {
                State capture(p);
                generate(p, capture, moves);
        }

        static int count(const Position<Board>& p)
        {
                Stack moves;
                generate(p, moves);
                return static_cast<int>(moves.size());
        }

        static bool detect(const Position<Board>& p)
        {
                // speculate #pawns > #kings so that the || is likely to short-circuit
                return (
                        PawnJumps::detect(p) ||
                        KingJumps::detect(p)
                );
        }

private:
        static void generate(const Position<Board>& p, State& capture, Stack& moves)
        {
                // tag dispatching on absolute king capture precedence
                generate_dispatch(
                        p, capture, moves,
                        Int2Type<rules::is_absolute_king_precedence<Rules>::value>()
                );
        }

        // partial specialization for no absolute king capture precedence
        static void generate_dispatch(
                const Position<Board>& p, State& capture, Stack& moves, Int2Type<false>
        )
        {
                KingJumps::generate(p, capture, moves);
                PawnJumps::generate(p, capture, moves);
        }

        // partial specialization for absolute king capture precedence
        static void generate_dispatch(
                const Position<Board>& p, State& capture, Stack& moves, Int2Type<true>
        )
        {
                KingJumps::generate(p, capture, moves);
                if (moves.empty())
                        PawnJumps::generate(p, capture, moves);
        }
};

}       // namespace successor
}       // namespace dctl
