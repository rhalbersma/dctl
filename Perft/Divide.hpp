#include "Perft.h"
#include "../IO/MoveIO.h"
#include <iostream>

template<typename Rules, typename Board>
void Divide::root(Position<Board>& p, size_t nominal_depth)
{
        double start_time, time_used;
        NodeCount divide_count = 0;
        NodeCount perft_count;

        Propagate<Rules, Board> moves(p);
        Generate::generate(p, moves);

        announce(p, nominal_depth, moves.size());
        for (size_t i = 0; i < moves.size(); ++i) {
                print_move(write_move_string<Rules>()(p, moves[i]), i);
                start_time = clock();

                p.template make<Rules>(moves[i]);
                Perft::reset_statistics();
                perft_count = Perft::perft<Rules>(p, 0, nominal_depth - 1);
                divide_count += perft_count;
                p.template undo<Rules>(moves[i]);

                time_used = (clock() + 1 - start_time) / CLOCKS_PER_SEC;
                Perft::report(perft_count, nominal_depth - 1, time_used);
        }
        summary(divide_count);
}

template<typename Board>
void Divide::announce(const Position<Board>& p, size_t nominal_depth, size_t num_moves)
{
        Perft::announce(p, nominal_depth);
        std::cout << "Found " << num_moves << " moves, searching each to nominal depth=" << nominal_depth - 1 << std::endl;
        std::cout << std::endl;
}
