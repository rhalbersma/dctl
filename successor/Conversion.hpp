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
        static void generate(const Position<Rules, Board>& p, Stack& moves)
        {
                DoJumps::generate(p, moves);
                if (moves.empty())
                        DoPromotions::generate(p, moves);
        }

        template<template<typename, typename> class Position>
        static int count(const Position<Rules, Board>& p)
        {
                auto num_moves = DoJumps::count(p);
                if (!num_moves)
                        num_moves += DoPromotions::count(p);
                return num_moves;
        }

        template<template<typename, typename> class Position>
        static bool detect(const Position<Rules, Board>& p)
        {
                return (
                        DoJumps::detect(p) ||
                        DoPromotions::detect(p)
                );
        }
};

}       // namespace successor
}       // namespace dctl
