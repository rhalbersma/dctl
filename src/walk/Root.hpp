#include <cassert>
#include <iostream>
#include "../generate/Successors.h"
#include "../node/Position.h"
#include "../node/Stack.h"
#include "../utils/Timer.h"

#include "../setup/Diagram.h"
#include "../setup/String.h"
#include "../notation/String.h"
#include "../protocol/pdn/PDN.h"

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
        Stack move_stack;
        generate::Successors<Rules, Board>::generate(p, move_stack);

        announce(p, depth, move_stack.size());
        Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                statistics_.reset();
                print_move(notation::write<Rules>()(p, move_stack[i]), i);

                q.template copy_make<Rules>(p, move_stack[i]);
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

        Stack move_stack;
        generate::Successors<Rules, Board>::generate(p, move_stack);
        NodeCount leafs = 0;        
        Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
                leafs += leaf<Rules>(q, ply + 1, depth - 1);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::bulk(const Position<Board>& p, int ply, int depth)
{
        statistics_.update(ply);

        Stack move_stack;
        generate::Successors<Rules, Board>::generate(p, move_stack);
        if (depth == 1)
                return move_stack.size();
        
        NodeCount leafs = 0;
        Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
                leafs += bulk<Rules>(q, ply + 1, depth - 1);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::count(const Position<Board>& p, int ply, int depth)
{
        statistics_.update(ply);

        if (depth == 1)
                return generate::Successors<Rules, Board>::count(p);

        Stack move_stack;
        generate::Successors<Rules, Board>::generate(p, move_stack);
        NodeCount leafs = 0;
        Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
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

        Stack move_stack;
        generate::Successors<Rules, Board>::generate(p, move_stack);
        NodeCount leafs = 0;
        Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
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
                leafs = generate::Successors<Rules, Board>::count(p);
        else {
                Stack move_stack;
                generate::Successors<Rules, Board>::generate(p, move_stack);
                leafs = 0;
                Position<Board> q;
                for (size_t i = 0; i < move_stack.size(); ++i) {
                        q.template copy_make<Rules>(p, move_stack[i]);
                        leafs += fast<Rules>(q, ply + 1, depth - 1);
                }
        }

        TT.insert(p, Transposition(leafs, depth));
        return leafs;
}

template<typename Board>
void Root::announce(const Position<Board>& p, int depth)
{        
        std::cout << setup::diagram<protocol::pdn::version>()(p) << std::endl;
        std::cout << setup::write<protocol::pdn::version>()(p) << std::endl << std::endl;
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
