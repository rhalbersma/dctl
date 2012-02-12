#pragma once
#include "Selection.hpp"
#include "../capture/State.hpp"
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/NonConstructible.hpp"

namespace dctl {

template<typename> struct Position;

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> struct Driver;

template<bool Color, typename Rules, typename Board> 
struct Driver<Color, Material::both, Jumps, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>& p, Stack& moves)
        {
                capture::State<Rules, Board> capture(p);
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
                return (
                        PawnJumps::detect(p) || 
                        KingJumps::detect(p)
                );
        }

private:
        static void generate(const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves)
        {
                // tag dispatching on absolute king capture precedence
                generate_dispatch(
                        p, capture, moves, 
                        Int2Type<rules::is_absolute_king_precedence<Rules>::value>()
                );
        }

        // partial specialization for no absolute king capture precedence
        static void generate_dispatch(
                const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<false>
        )
        {
                KingJumps::generate(p, capture, moves);
                PawnJumps::generate(p, capture, moves);
        }

        // partial specialization for absolute king capture precedence
        static void generate_dispatch(
                const Position<Board>& p, capture::State<Rules, Board>& capture, Stack& moves, Int2Type<true>
        )
        {
                KingJumps::generate(p, capture, moves);
                if (moves.empty())
                        PawnJumps::generate(p, capture, moves);
        }

        // typedefs
        typedef Driver<Color, Material::king, Jumps, Rules, Board> KingJumps;
        typedef Driver<Color, Material::pawn, Jumps, Rules, Board> PawnJumps;
};

}       // namespace successor
}       // namespace dctl
