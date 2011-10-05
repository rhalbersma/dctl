#pragma once
#include "../capture/State.hpp"
#include "../node/Material.h"
#include "../node/Stack.hpp"
#include "../rules/Rules.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {

namespace capture { template<typename, typename> class State; }

namespace successor {

template<bool Color, typename Rules, typename Board> 
class Driver<Color, Material::BOTH, Jumps, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>& p, Stack& moves)
        {
                capture::State<Rules, Board> capture(p);
                generate_precede(p, capture, moves);
        }

        static int count(const Position<Board>& p)
        {
                Stack moves;
                generate(p, moves);
                return static_cast<int>(moves.size());
        }

        static bool detect(const Position<Board>& p)
        {
                return (
                        PawnJumps::detect(p) || 
                        KingJumps::detect(p)
                );
        }

private:
        // tag dispatching on absolute king capture precedence
        static void generate_precede(const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves)
        {
                generate_precede(
                        p, capture, moves, 
                        Int2Type<rules::is_absolute_king_precedence<Rules>::value>()
                );
        }

        // partial specialization for no absolute king capture precedence
        static void generate_precede(
                const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<false>
        )
        {
                KingJumps::generate(p, capture, moves);
                PawnJumps::generate(p, capture, moves);
        }

        // partial specialization for absolute king capture precedence
        static void generate_precede(
                const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<true>
        )
        {
                KingJumps::generate(p, capture, moves);
                if (moves.empty())
                        PawnJumps::generate(p, capture, moves);
        }

        // typedefs
        typedef Driver<Color, Material::KING, Jumps, Rules, Board> KingJumps;
        typedef Driver<Color, Material::PAWN, Jumps, Rules, Board> PawnJumps;
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "BothJumps.hpp"
