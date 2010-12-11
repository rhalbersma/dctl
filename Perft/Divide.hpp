#include "Perft.h"
#include "../IO/MoveIO.h"
#include "../Utilities/StopWatch.h"
#include <iostream>

template<typename Rules, typename Board>
NodeCount Divide::divide(const Position<Board>& pp, size_t nominal_depth)
{
        Position<Board> p(pp);
        NodeCount leafs = 0;
        NodeCount move_leafs;                
        StopWatch timer;

        timer.start();
        Propagate<Rules, Board> moves(p);
        Generate::generate(p, moves);

        announce(p, nominal_depth, moves.size());
        for (size_t i = 0; i < moves.size(); ++i) {
                Perft::reset_statistics();
                print_move(write_move_string<Rules>()(p, moves[i]), i);

                p.template make<Rules>(moves[i]);
                move_leafs = Perft::driver<Rules>(p, 0, nominal_depth - 1);
                leafs += move_leafs;
                p.template undo<Rules>(moves[i]);
                
                timer.split();
                Perft::report(nominal_depth - 1, move_leafs, timer);
        }
        summary(leafs);

        assert(p == pp);
        return leafs;
}

template<typename Board>
void Divide::announce(const Position<Board>& p, size_t nominal_depth, size_t num_moves)
{
        Perft::announce(p, nominal_depth);
        std::cout << "Found " << num_moves << " moves, searching each to nominal depth=" << nominal_depth - 1 << std::endl;
        std::cout << std::endl;
}
