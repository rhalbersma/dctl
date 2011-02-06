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
        MoveList move_list;
        Generate<Rules, Board>::generate(p, move_list);

        announce(p, nominal_depth, move_list.size());
        Position<Board> q;
        for (size_t i = 0; i < move_list.size(); ++i) {
                Perft::reset_statistics();
                print_move(write_move_string<Rules>()(p, move_list[i]), i);

                q.template copy_make<Rules>(p, move_list[i]);
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
        std::cout << "Found " << num_moves << " move_list, searching each to nominal depth=" << nominal_depth - 1 << std::endl;
        std::cout << std::endl;
}

}       // namespace TreeWalk
