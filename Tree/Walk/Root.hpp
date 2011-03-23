#include "../Generate/Successors.h"
#include "../Move/Stack.h"
#include "../Move/String.h"
#include "../Node/Layout.h"
#include "../Node/Position.h"
#include "../Node/String.h"
#include "../../Utilities/StopWatch.h"
#include <cassert>
#include <iostream>

namespace Tree {
namespace Walk {

template<typename Rules, typename Board>
NodeCount Root::perft(const Node::Position<Board>& p, size_t nominal_depth)
{
        NodeCount leafs = 0;
        StopWatch timer;

        timer.start();
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
NodeCount Root::divide(const Node::Position<Board>& p, size_t nominal_depth)
{
        NodeCount leafs = 0;
        NodeCount move_leafs;                
        StopWatch timer;

        timer.start();
        Move::Stack move_stack;
        Generate::Successors<Rules, Board>::generate(p, move_stack);

        announce(p, nominal_depth, move_stack.size());
        Node::Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                statistics_.reset();
                print_move(Move::String::write<Rules>()(p, move_stack[i]), i);

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
NodeCount Root::driver(const Node::Position<Board>& p, size_t ply, size_t depth)
{
        return (depth == 0)? leaf<Rules>(p, ply, depth) : fast<Rules>(p, ply, depth);
}

template<typename Rules, typename Board>
NodeCount Root::leaf(const Node::Position<Board>& p, size_t ply, size_t depth)
{        
        statistics_.update(ply);

        if (depth == 0)
                return 1;

        Move::Stack move_stack;
        Generate::Successors<Rules, Board>::generate(p, move_stack);
        NodeCount leafs = 0;        
        Node::Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
                leafs += leaf<Rules>(q, ply + 1, depth - 1);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::bulk(const Node::Position<Board>& p, size_t ply, size_t depth)
{
        statistics_.update(ply);

        Move::Stack move_stack;
        Generate::Successors<Rules, Board>::generate(p, move_stack);
        if (depth == 1)
                return move_stack.size();
        
        NodeCount leafs = 0;
        Node::Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
                leafs += bulk<Rules>(q, ply + 1, depth - 1);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::count(const Node::Position<Board>& p, size_t ply, size_t depth)
{
        statistics_.update(ply);

        if (depth == 1)
                return Generate::Successors<Rules, Board>::count(p);

        Move::Stack move_stack;
        Generate::Successors<Rules, Board>::generate(p, move_stack);
        NodeCount leafs = 0;
        Node::Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
                leafs += count<Rules>(q, ply + 1, depth - 1);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::hash(const Node::Position<Board>& p, size_t ply, size_t depth)
{
        statistics_.update(ply);

        const Entry* TT_entry = TT.find(p);
        if (TT_entry && TT_entry->is_sufficient(depth))
                return TT_entry->leafs();

        if (depth == 0)
                return 1;

        Move::Stack move_stack;
        Generate::Successors<Rules, Board>::generate(p, move_stack);
        NodeCount leafs = 0;
        Node::Position<Board> q;
        for (size_t i = 0; i < move_stack.size(); ++i) {
                q.template copy_make<Rules>(p, move_stack[i]);
                leafs += hash<Rules>(q, ply + 1, depth - 1);
        }

        TT.insert(p, Entry(leafs, depth));
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::fast(const Node::Position<Board>& p, size_t ply, size_t depth)
{
        statistics_.update(ply);

        const Entry* TT_entry = TT.find(p);
        if (TT_entry && TT_entry->is_sufficient(depth))
                return TT_entry->leafs();

        NodeCount leafs;
        if (depth == 1)
                leafs = Generate::Successors<Rules, Board>::count(p);
        else {
                Move::Stack move_stack;
                Generate::Successors<Rules, Board>::generate(p, move_stack);
                leafs = 0;
                Node::Position<Board> q;
                for (size_t i = 0; i < move_stack.size(); ++i) {
                        q.template copy_make<Rules>(p, move_stack[i]);
                        leafs += fast<Rules>(q, ply + 1, depth - 1);
                }
        }

        TT.insert(p, Entry(leafs, depth));
        return leafs;
}

template<typename Board>
void Root::announce(const Node::Position<Board>& p, size_t nominal_depth)
{        
        std::cout << Node::Layout::write<Node::FEN_tag>()(p) << std::endl;
        std::cout << Node::String::write<Node::FEN_tag>()(p) << std::endl << std::endl;
        std::cout << "Searching to nominal depth=" << nominal_depth << std::endl;
        std::cout << std::endl;
}

template<typename Board>
void Root::announce(const Node::Position<Board>& p, size_t nominal_depth, size_t num_moves)
{
        announce(p, nominal_depth);
        std::cout << "Found " << num_moves << " move_stack, searching each to nominal depth=" << nominal_depth - 1 << std::endl;
        std::cout << std::endl;
}

}       // namesapce Walk
}       // namespace Tree
