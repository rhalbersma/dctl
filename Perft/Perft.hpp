#include "../Generate/Propagate.h"
#include "../Generate/Generate.h"
#include "../Position/Position.h"
#include "../IO/PositionIO.h"
#include "../Utilities/StopWatch.h"
#include <cassert>
#include <iostream>

template<typename Rules, typename Board>
NodeCount Perft::perft(const Position<Board>& pp, size_t nominal_depth)
{
        Position<Board> p(pp);
        NodeCount leafs = 0;
        StopWatch timer;

        timer.start();
        announce(p, nominal_depth);
        for (size_t depth = 1; depth <= nominal_depth; ++depth) {             
                reset_statistics();
                leafs = driver<Rules>(p, 0, depth);
                timer.split();
                report(depth, leafs, timer);
        }
                
        assert(p == pp);
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Perft::driver(Position<Board>& p, size_t ply, size_t depth)
{
        return perft_bulk<Rules>(p, ply, depth);
}

template<typename Rules, typename Board>
NodeCount Perft::perft_leaf(Position<Board>& p, size_t ply, size_t depth)
{
        update_statistics(ply);

        if (depth == 0)
                return 1;

        Propagate<Rules, Board> moves(p);
        Generate::generate(p, moves);
        NodeCount leafs = 0;
        for (size_t i = 0; i < moves.size(); ++i) {
                p.template make<Rules>(moves[i]);
                leafs += perft_leaf<Rules>(p, ply + 1, depth - 1);
                p.template undo<Rules>(moves[i]);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Perft::perft_bulk(Position<Board>& p, size_t ply, size_t depth)
{
        update_statistics(ply);

        Propagate<Rules, Board> moves(p);
        Generate::generate(p, moves);
        if (depth == 1)
                return moves.size();
        
        NodeCount leafs = 0;
        for (size_t i = 0; i < moves.size(); ++i) {
                p.template make<Rules>(moves[i]);
                leafs += perft_bulk<Rules>(p, ply + 1, depth - 1);
                p.template undo<Rules>(moves[i]);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Perft::perft_count(Position<Board>& p, size_t ply, size_t depth)
{
        update_statistics(ply);

        if (depth == 1)
                return Generate::count<Rules>(p);

        Propagate<Rules, Board> moves(p);
        Generate::generate(p, moves);
        NodeCount leafs = 0;
        for (size_t i = 0; i < moves.size(); ++i) {
                p.template make<Rules>(moves[i]);
                leafs += perft_count<Rules>(p, ply + 1, depth - 1);
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
NodeCount Perft::perft_fast(Position<Board>& p, size_t ply, size_t depth)
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
                        leafs += perft_fast<Rules>(p, ply + 1, depth - 1);
                        p.template undo<Rules>(moves[i]);
                }
        }

        TT.insert(p, PerftNode(leafs, depth));
        return leafs;
}

template<typename Board>
void Perft::announce(const Position<Board>& p, size_t nominal_depth)
{        
        std::cout << write_position_layout<FEN>()(p) << std::endl;
        std::cout << write_position_string<FEN>()(p) << std::endl << std::endl;
        std::cout << "Searching to nominal depth=" << nominal_depth << std::endl;
        std::cout << std::endl;
}
