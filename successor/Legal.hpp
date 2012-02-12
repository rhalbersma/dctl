#pragma once
#include "Selection.hpp"
#include "../node/Stack.hpp"
#include "../utility/NonConstructible.hpp"

namespace dctl {

template<typename> struct Position;

namespace successor {

// forward declaration of the primary template
template<bool, int, typename, typename, typename> struct Driver;

// partial specialization for legal successors
template<bool Color, int Material, typename Rules, typename Board> 
struct Driver<Color, Material, Legal, Rules, Board>
:
        private nonconstructible // enforce static semantics
{
public:
        static void generate(const Position<Board>& p, Stack& moves) 
        {
                DoJumps::generate(p, moves);
                if (moves.empty())
                        DoMoves::generate(p, moves);
        }

        static int count(const Position<Board>& p)
        {
                auto num_moves = DoJumps::count(p);
                if (!num_moves)
                        num_moves += DoMoves::count(p);
                return num_moves;
        }

        static bool detect(const Position<Board>& p) 
        { 
                return (
                        DoJumps::detect(p) || 
                        DoMoves::detect(p)
                );
        }

private:
        // typedefs
        typedef Driver<Color, Material, Jumps, Rules, Board> DoJumps;
        typedef Driver<Color, Material, Moves, Rules, Board> DoMoves;
};

}       // namespace successor
}       // namespace dctl
