#pragma once
#include "Driver_fwd.hpp"
#include "KingJumps.hpp"
#include "PawnJumps.hpp"
#include "Selection.hpp"
#include "../capture/State.hpp"
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../rules/Rules.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::both, select::Jumps, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<Color, Material::king, select::Jumps, Rules, Board> KingJumps;
        typedef Driver<Color, Material::pawn, select::Jumps, Rules, Board> PawnJumps;
        typedef capture::State<Rules, Board> State;

public:
        template<template<typename, typename> class Position>
        static void generate(const Position<Rules, Board>& p, Stack& moves)
        {
                State capture(p, moves);
                generate(p, capture);
        }

        template<template<typename, typename> class Position>
        static int count(const Position<Rules, Board>& p)
        {
                Stack moves;
                generate(p, moves);
                return static_cast<int>(moves.size());
        }

        template<template<typename, typename> class Position>
        static bool detect(const Position<Rules, Board>& p)
        {
                // speculate #pawns > #kings so that the || is likely to short-circuit
                return (
                        PawnJumps::detect(p) ||
                        KingJumps::detect(p)
                );
        }

private:
        template<template<typename, typename> class Position>
        static void generate(const Position<Rules, Board>& p, State& capture)
        {
                // tag dispatching on absolute king capture precedence
                generate_dispatch(
                        p, capture,
                        Int2Type<rules::is_absolute_king_precedence<Rules>::value>()
                );
        }

        // partial specialization for no absolute king capture precedence
        template<template<typename, typename> class Position>
        static void generate_dispatch(
                const Position<Rules, Board>& p, State& capture, Int2Type<false>
        )
        {
                KingJumps::generate(p, capture);
                PawnJumps::generate(p, capture);
        }

        // partial specialization for absolute king capture precedence
        template<template<typename, typename> class Position>
        static void generate_dispatch(
                const Position<Rules, Board>& p, State& capture, Int2Type<true>
        )
        {
                KingJumps::generate(p, capture);
                if (capture.empty())
                        PawnJumps::generate(p, capture);
        }
};

}       // namespace successor
}       // namespace dctl
