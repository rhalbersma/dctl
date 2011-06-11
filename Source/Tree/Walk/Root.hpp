#include <cassert>
#include <iostream>
#include "../Generate/Successors.h"
#include "../Move/Stack.h"
#include "../Move/String.h"
#include "../Node/Layout.h"
#include "../Node/Position.h"
#include "../Node/String.h"
#include "../../Utilities/Timer.h"

namespace tree {
namespace walk {

template<typename Rules, typename Board>
NodeCount Root::perft(const node::Position<Board>& p, size_t nominal_depth)
{
        NodeCount leafs = 0;
        
        Timer timer;
        announce(p, nominal_depth);
        for (size_t depth = 1; depth <= nominal_depth; ++depth) {                        
                statistics_.reset();
                leafs = driver<Rules>(p, 0, depth);
                timer.split();
                report(depth, leafs, timer);
        }
        summary();
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::divide(const node::Position<Board>& p, size_t nominal_depth)
{
        NodeCount leafs = 0;
        NodeCount move_leafs;                
        
        Timer timer;
        move::Stack move_stack;
        generate::Successors<Rules, Board>::generate(p, move_stack);

        announce(p, nominal_depth, move_stack.size());
        node::Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                statistics_.reset();
                print_move(move::string::write<Rules>()(p, move_stack[i]), i);

                q.template copy_make<Rules>(p, move_stack[i]);
                move_leafs = driver<Rules>(q, 0, nominal_depth - 1);
                leafs += move_leafs;
                
                timer.split();
                Root::report(nominal_depth - 1, move_leafs, timer);
        }
        summary(leafs);

        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::driver(const node::Position<Board>& p, size_t ply, size_t depth)
{
        return (depth == 0)? leaf<Rules>(p, ply, depth) : fast<Rules>(p, ply, depth);
}

template<typename Rules, typename Board>
NodeCount Root::leaf(const node::Position<Board>& p, size_t ply, size_t depth)
{        
        statistics_.update(ply);

        if (depth == 0)
                return 1;

        move::Stack move_stack;
        generate::Successors<Rules, Board>::generate(p, move_stack);
        NodeCount leafs = 0;        
        node::Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
                leafs += leaf<Rules>(q, ply + 1, depth - 1);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::bulk(const node::Position<Board>& p, size_t ply, size_t depth)
{
        statistics_.update(ply);

        move::Stack move_stack;
        generate::Successors<Rules, Board>::generate(p, move_stack);
        if (depth == 1)
                return move_stack.size();
        
        NodeCount leafs = 0;
        node::Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
                leafs += bulk<Rules>(q, ply + 1, depth - 1);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::count(const node::Position<Board>& p, size_t ply, size_t depth)
{
        statistics_.update(ply);

        if (depth == 1)
                return generate::Successors<Rules, Board>::count(p);

        move::Stack move_stack;
        generate::Successors<Rules, Board>::generate(p, move_stack);
        NodeCount leafs = 0;
        node::Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
                leafs += count<Rules>(q, ply + 1, depth - 1);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::hash(const node::Position<Board>& p, size_t ply, size_t depth)
{
        statistics_.update(ply);

        const Entry* TT_entry = TT.find(p);
        if (TT_entry && TT_entry->equal_to(depth))
                return TT_entry->leafs();

        if (depth == 0)
                return 1;

        move::Stack move_stack;
        generate::Successors<Rules, Board>::generate(p, move_stack);
        NodeCount leafs = 0;
        node::Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
                leafs += hash<Rules>(q, ply + 1, depth - 1);
        }

        TT.insert(p, Entry(leafs, depth));
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::fast(const node::Position<Board>& p, size_t ply, size_t depth)
{
        statistics_.update(ply);

        const Entry* TT_entry = TT.find(p);
        if (TT_entry && TT_entry->equal_to(depth))
                return TT_entry->leafs();

        NodeCount leafs;
        if (depth == 1)
                leafs = generate::Successors<Rules, Board>::count(p);
        else {
                move::Stack move_stack;
                generate::Successors<Rules, Board>::generate(p, move_stack);
                leafs = 0;
                node::Position<Board> q;
                for (size_t i = 0; i < move_stack.size(); ++i) {
                        q.template copy_make<Rules>(p, move_stack[i]);
                        leafs += fast<Rules>(q, ply + 1, depth - 1);
                }
        }

        TT.insert(p, Entry(leafs, depth));
        return leafs;
}

template<typename Board>
void Root::announce(const node::Position<Board>& p, size_t nominal_depth)
{        
        std::cout << node::layout::write<node::FEN_tag>()(p) << std::endl;
        std::cout << node::string::write<node::FEN_tag>()(p) << std::endl << std::endl;
        std::cout << "Searching to nominal depth=" << nominal_depth << std::endl;
        std::cout << std::endl;
}

template<typename Board>
void Root::announce(const node::Position<Board>& p, size_t nominal_depth, size_t num_moves)
{
        announce(p, nominal_depth);
        std::cout << "Found " << num_moves << " move_stack, searching each to nominal depth=" << nominal_depth - 1 << std::endl;
        std::cout << std::endl;
}

}       // namesapce Walk
}       // namespace tree
