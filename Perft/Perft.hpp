#include "../Generate/Propagate.h"
#include "../Generate/Generate.h"
#include "../Position/Position.h"
#include "../IO/PositionIO.h"
#include <ctime>
#include <iostream>

template<typename Rules, typename Board>
void Perft::root(Position<Board>& p, size_t nominal_depth)
{
        double start_time, time_used;
        NodeCount perft_count;

        announce(p, nominal_depth);
        for (size_t depth = 1; depth <= nominal_depth; ++depth) {
                start_time = clock();
                reset_statistics();
                perft_count = perft_bulk<Rules>(p, 0, depth);
                time_used = (clock() + 1 - start_time) / CLOCKS_PER_SEC;
                report(perft_count, depth, time_used, true);
        }
}

template<typename Rules, typename Board>
NodeCount Perft::perft(Position<Board>& p, size_t ply, size_t depth)
{
        update_statistics(ply);

        if (depth == 0)
                return 1;

        Propagate<Rules, Board> moves(p);
        Generate::generate(p, moves);
        NodeCount leafs = 0;
        for (size_t i = 0; i < moves.size(); ++i) {
                p.template make<Rules>(moves[i]);
                leafs += perft<Rules>(p, ply + 1, depth - 1);
                p.template undo<Rules>(moves[i]);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Perft::perft_bulk(Position<Board>& p, size_t ply, size_t depth)
{
        update_statistics(ply);

        if (depth == 1)
                return Generate::count<Rules>(p);

        Propagate<Rules, Board> moves(p);
        Generate::generate(p, moves);
        NodeCount leafs = 0;
        for (size_t i = 0; i < moves.size(); ++i) {
                p.template make<Rules>(moves[i]);
                leafs += perft_bulk<Rules>(p, ply + 1, depth - 1);
                p.template undo<Rules>(moves[i]);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Perft::perft_hash(Position<Board>& p, size_t ply, size_t depth)
{
        update_statistics(ply);

        const PerftNode* TT_entry = TT.find(p);
        if (TT_entry && TT_entry->is_depth_equal_to(depth))
                return TT_entry->leafs();

        if (depth == 0)
                return 1;

        Propagate<Rules, Board> moves(p);
        Generate::generate(p, moves);
        NodeCount leafs = 0;
        for (size_t i = 0; i < moves.size(); ++i) {
                p.template make<Rules>(moves[i]);
                leafs += perft_hash<Rules>(p, ply + 1, depth - 1);
                p.template undo<Rules>(moves[i]);
        }

        TT.insert(p, PerftNode(leafs, depth));
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Perft::perft_bulk_hash(Position<Board>& p, size_t ply, size_t depth)
{
        update_statistics(ply);

        const PerftNode* TT_entry = TT.find(p);
        if (TT_entry && TT_entry->is_depth_equal_to(depth))
                return TT_entry->leafs();

        NodeCount leafs;
        if (depth == 1)
                leafs = Generate::count<Rules>(p);
        else {
                Propagate<Rules, Board> moves(p);
                Generate::generate(p, moves);
                leafs = 0;
                for (size_t i = 0; i < moves.size(); ++i) {
                        p.template make<Rules>(moves[i]);
                        leafs += perft_bulk_hash<Rules>(p, ply + 1, depth - 1);
                        p.template undo<Rules>(moves[i]);
                }
        }

        TT.insert(p, PerftNode(leafs, depth));
        return leafs;
}

template<typename Board>
void Perft::announce(const Position<Board>& p, size_t nominal_depth)
{        
        std::cout << std::endl;
        std::cout << write_position_layout<FEN>()(p) << std::endl;
        std::cout << write_position_string<FEN>()(p) << std::endl << std::endl;
        std::cout << "Searching to nominal depth=" << nominal_depth << std::endl;
        std::cout << std::endl;
}
