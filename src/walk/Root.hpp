#include <cassert>
#include <iostream>
#include "../successor/Successor.h"
#include "../node/Position.h"
#include "../node/Stack.h"
#include "../utils/Timer.h"

#include "../setup/Diagram.h"
#include "../setup/String.h"
#include "../notation/String.h"

namespace dctl {
namespace walk {

template<typename Rules, typename Board>
NodeCount Root::perft(const Position<Board>& p, int depth)
{
        NodeCount leafs = 0;
        
        Timer timer;
        announce(p, depth);
        for (auto i = 1; i <= depth; ++i) {                        
                statistics_.reset();
                leafs = driver<Rules>(p, 0, i);
                timer.split();
                report(i, leafs, timer);
        }
        summary();
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::divide(const Position<Board>& p, int depth)
{
        NodeCount leafs = 0;
        NodeCount move_leafs;                
        
        Timer timer;
        Stack moves;
        Successor<Rules, Board, Legal>::generate(p, moves);

        announce(p, depth, moves.size());
        for (auto i = 0; i < static_cast<int>(moves.size()); ++i) {
                statistics_.reset();
                print_move(notation::write<Rules>()(p, moves[i]), i);

                auto q(p);
                q.attach(p);
                q.template make<Rules>(moves[i]);
                move_leafs = driver<Rules>(q, 0, depth - 1);
                leafs += move_leafs;              
                
                timer.split();
                Root::report(depth - 1, move_leafs, timer);
        }
        summary(leafs);

        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::driver(const Position<Board>& p, int ply, int depth)
{
        return (depth == 0)? leaf<Rules>(p, ply, depth) : fast<Rules>(p, ply, depth);
}

template<typename Rules, typename Board>
NodeCount Root::leaf(const Position<Board>& p, int ply, int depth)
{        
        statistics_.update(ply);

        if (depth == 0)
                return 1;

        Stack moves;
        Successor<Rules, Board, Legal>::generate(p, moves);
        NodeCount leafs = 0;        
        for (auto m = moves.begin(); m != moves.end(); ++m) {
                auto q(p);
                q.attach(p);
                q.template make<Rules>(*m);
                leafs += leaf<Rules>(q, ply + 1, depth - 1);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::bulk(const Position<Board>& p, int ply, int depth)
{
        statistics_.update(ply);

        Stack moves;
        Successor<Rules, Board, Legal>::generate(p, moves);
        if (depth == 1)
                return moves.size();
        
        NodeCount leafs = 0;
        for (auto m = moves.begin(); m != moves.end(); ++m) {
                auto q(p);
                q.attach(p);
                q.template make<Rules>(*m);
                leafs += bulk<Rules>(q, ply + 1, depth - 1);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::count(const Position<Board>& p, int ply, int depth)
{
        statistics_.update(ply);

        if (depth == 1)
                return Successor<Rules, Board, Legal>::count(p);

        Stack moves;
        Successor<Rules, Board, Legal>::generate(p, moves);
        NodeCount leafs = 0;
        for (auto m = moves.begin(); m != moves.end(); ++m) {
                auto q(p);
                q.attach(p);
                q.template make<Rules>(*m);
                leafs += count<Rules>(q, ply + 1, depth - 1);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::hash(const Position<Board>& p, int ply, int depth)
{
        statistics_.update(ply);

        auto TT_entry = TT.find(p);
        if (TT_entry && TT_entry->depth() == depth)
                return TT_entry->leafs();

        if (depth == 0)
                return 1;

        Stack moves;
        Successor<Rules, Board, Legal>::generate(p, moves);
        NodeCount leafs = 0;
        for (auto m = moves.begin(); m != moves.end(); ++m) {
                auto q(p);
                q.attach(p);
                q.template make<Rules>(*m);
                leafs += hash<Rules>(q, ply + 1, depth - 1);
        }

        TT.insert(p, Transposition(leafs, depth));
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::fast(const Position<Board>& p, int ply, int depth)
{
        statistics_.update(ply);

        auto TT_entry = TT.find(p);
        if (TT_entry && TT_entry->depth() == depth)
                return TT_entry->leafs();

        NodeCount leafs;
        if (depth == 1)
                leafs = Successor<Rules, Board, Legal>::count(p);
        else {
                Stack moves;
                Successor<Rules, Board, Legal>::generate(p, moves);
                leafs = 0;
                for (auto m = moves.begin(); m != moves.end(); ++m) {
                        auto q(p);
                        q.attach(p);
                        q.template make<Rules>(*m);
                        leafs += fast<Rules>(q, ply + 1, depth - 1);
                }
        }

        TT.insert(p, Transposition(leafs, depth));
        return leafs;
}

template<typename Board>
void Root::announce(const Position<Board>& p, int depth)
{        
        std::cout << setup::diagram<pdn::protocol>()(p) << std::endl;
        std::cout << setup::write<pdn::protocol>()(p) << std::endl << std::endl;
        std::cout << "Searching to nominal depth=" << depth << std::endl;
        std::cout << std::endl;
}

template<typename Board>
void Root::announce(const Position<Board>& p, int depth, int num_moves)
{
        announce(p, depth);
        std::cout << "Found " << num_moves << " moves, searching each to nominal depth=" << depth - 1 << std::endl;
        std::cout << std::endl;
}

}       // namespace walk
}       // namespace dctl
