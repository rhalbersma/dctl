#include "Perft.h"
#include "../IO/MoveIO.h"
#include "../Utilities/StopWatch.h"
#include <iostream>

namespace TreeWalk {

template<typename Rules, typename Board>
NodeCount Divide::divide(const Position<Board>& p, size_t nominal_depth)
{
        NodeCount leafs = 0;
        NodeCount move_leafs;                
        StopWatch timer;

        timer.start();
        MoveList moves;;
        Generate<Rules, Board>::generate(p, moves);

        announce(p, nominal_depth, moves.size());
        Position<Board> q;
        for (size_t i = 0; i < moves.size(); ++i) {
                Perft::reset_statistics();
                print_move(write_move_string<Rules>()(p, moves[i]), i);

                q.template copy_make<Rules>(p, moves[i]);
                move_leafs = Perft::driver<Rules>(q, 0, nominal_depth - 1);
                leafs += move_leafs;
                
                timer.split();
                Perft::report(nominal_depth - 1, move_leafs, timer);
        }
        summary(leafs);

        return leafs;
}

template<typename Board>
void Divide::announce(const Position<Board>& p, size_t nominal_depth, size_t num_moves)
{
        Perft::announce(p, nominal_depth);
        std::cout << "Found " << num_moves << " moves, searching each to nominal depth=" << nominal_depth - 1 << std::endl;
        std::cout << std::endl;
}

}       // namespace TreeWalk
