#pragma once
#include "Driver_fwd.hpp"
#include "Primary.hpp"
#include "BothJumps.hpp"
#include "BothPromotion.hpp"
#include "KingJumps.hpp"
#include "PawnJumps.hpp"
#include "PawnPromotion.hpp"
#include "Selection.hpp"
#include "../node/Stack.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {

// partial specialization for legal successors
template<bool Color, int Material, typename Rules, typename Board>
struct Driver<Color, Material, select::Conversion, Rules, Board>
:
        // enforce static semantics
        private nonconstructible
{
private:
        // typedefs

        typedef Driver<Color, Material, select::Jumps     , Rules, Board> DoJumps;
        typedef Driver<Color, Material, select::Promotions, Rules, Board> DoPromotions;

public:
        template<template<typename, typename> class Position>
        static void generate(Position<Rules, Board> const& p, Stack& moves)
        {
                DoJumps::generate(p, moves);
                if (moves.empty())
                        DoPromotions::generate(p, moves);
        }

        template<template<typename, typename> class Position>
        static int count(Position<Rules, Board> const& p)
        {
                auto num_moves = DoJumps::count(p);
                if (!num_moves)
                        num_moves += DoPromotions::count(p);
                return num_moves;
        }

        template<template<typename, typename> class Position>
        static bool detect(Position<Rules, Board> const& p)
        {
                // speculate #jumps > #promotions so that the || is likely to short-circuit
                return (
                        DoJumps::detect(p) ||
                        DoPromotions::detect(p)
                );
        }
};

}       // namespace successor
}       // namespace dctl
