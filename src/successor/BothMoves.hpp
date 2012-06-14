#pragma once
#include "Driver_fwd.hpp"
#include "KingMoves.hpp"
#include "PawnMoves.hpp"
#include "Selection.hpp"
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::both, select::Moves, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs
        typedef Driver<Color, Material::king, select::Moves, Rules, Board> KingMoves;
        typedef Driver<Color, Material::pawn, select::Moves, Rules, Board> PawnMoves;

public:
        template<template<typename, typename> class Position>
        static void generate(Position<Rules, Board> const& p, Stack& moves)
        {
                KingMoves::generate(p, moves);
                PawnMoves::generate(p, moves);
        }

        template<template<typename, typename> class Position>
        static int count(Position<Rules, Board> const& p)
        {
                return (
                        KingMoves::count(p) +
                        PawnMoves::count(p)
                );
        }

        template<template<typename, typename> class Position>
        static bool detect(Position<Rules, Board> const& p)
        {
                // speculate #pawns > #kings so that the || is likely to short-circuit
                return (
                        PawnMoves::detect(p) ||
                        KingMoves::detect(p)
                );
        }
};

}       // namespace successor
}       // namespace dctl
