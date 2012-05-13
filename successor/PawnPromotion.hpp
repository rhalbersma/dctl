#pragma once
#include "Driver_fwd.hpp"
#include "PawnMoves.hpp"
#include "Selection.hpp"
#include "../node/Material.hpp"
#include "../node/Promotion.hpp"
#include "../node/Stack.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for pawn promotions
template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::pawn, select::Promotions, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<Color, Material::pawn, select::Moves, Rules, Board> ActivePawnMoves;

public:
        template<template<typename, typename> class Position>
        static void generate(Position<Rules, Board> const& p, Stack& moves)
        {
                ActivePawnMoves::generate(promoting_pawns<Color>(p), not_occupied(p), moves);
        }

        template<template<typename, typename> class Position>
        static int count(Position<Rules, Board> const& p)
        {
                return ActivePawnMoves::count(promoting_pawns<Color>(p), not_occupied(p));
        }

        template<template<typename, typename> class Position>
        static bool detect(Position<Rules, Board> const& p)
        {
                return ActivePawnMoves::detect(promoting_pawns<Color>(p), not_occupied(p));
        }
};

}       // namespace successor
}       // namespace dctl
