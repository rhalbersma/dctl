#pragma once
#include "Driver_fwd.hpp"
#include "KingReverse.hpp"
#include "PawnReverse.hpp"
#include "Selection.hpp"
#include "../node/Material.hpp"
#include "../node/Stack.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/NonConstructible.hpp"

namespace dctl {
namespace successor {

template<bool Color, typename Rules, typename Board>
struct Driver<Color, Material::both, select::Reverse, Rules, Board>
:
        private nonconstructible        // enforce static semantics
{
private:
        // typedefs
        typedef Driver<Color, Material::king, select::Reverse, Rules, Board> KingReverse;
        typedef Driver<Color, Material::pawn, select::Reverse, Rules, Board> PawnReverse;

public:
        template<template<typename, typename> class Position>
        static void generate(const Position<Rules, Board>& p, Stack& moves)
        {
                KingReverse::generate(p, moves);
                PawnReverse::generate(p, moves);
        }

        template<template<typename, typename> class Position>
        static int count(const Position<Rules, Board>& p)
        {
                return (
                        KingReverse::count(p) +
                        PawnReverse::count(p)
                );
        }

        template<template<typename, typename> class Position>
        static bool detect(const Position<Rules, Board>& p)
        {
                return (
                        PawnReverse::detect(p) ||
                        KingReverse::detect(p)
                );
        }
};

}       // namespace successor
}       // namespace dctl
