#include "../../Generate/Generate.h"
#include "../../Position/Move.h"
#include "../../Position/Position.h"
#include "../../IO/FEN.h"
#include "../../IO/MoveIO.h"
#include "../../Utilities/StopWatch.h"
#include <cassert>
#include <iostream>

namespace Tree {
namespace Walk {

template<typename Rules, typename Board>
NodeCount Root::perft(const Position<Board>& p, size_t nominal_depth)
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
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::divide(const Position<Board>& p, size_t nominal_depth)
{
        NodeCount leafs = 0;
        NodeCount move_leafs;                
        StopWatch timer;

        timer.start();
        Move::List move_list;
        Generate<Rules, Board>::generate(p, move_list);

        announce(p, nominal_depth, move_list.size());
        Position<Board> q;
        for (size_t i = 0; i < move_list.size(); ++i) {
                statistics_.reset();
                print_move(write_move_string<Rules>()(p, move_list[i]), i);

                q.template copy_make<Rules>(p, move_list[i]);
                move_leafs = driver<Rules>(q, 0, nominal_depth - 1);
                leafs += move_leafs;
                
                timer.split();
                Root::report(nominal_depth - 1, move_leafs, timer);
        }
        summary(leafs);

        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::driver(const Position<Board>& p, size_t ply, size_t depth)
{
        return (depth == 0)? leaf<Rules>(p, ply, depth) : fast<Rules>(p, ply, depth);
}

template<typename Rules, typename Board>
NodeCount Root::leaf(const Position<Board>& p, size_t ply, size_t depth)
{        
        statistics_.update(ply);

        if (depth == 0)
                return 1;

        Move::List move_list;
        Generate<Rules, Board>::generate(p, move_list);
        NodeCount leafs = 0;        
        Position<Board> q;
        for (size_t i = 0; i < move_list.size(); ++i) {
                q.template copy_make<Rules>(p, move_list[i]);
                leafs += leaf<Rules>(q, ply + 1, depth - 1);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::bulk(const Position<Board>& p, size_t ply, size_t depth)
{
        statistics_.update(ply);

        Move::List move_list;
        Generate<Rules, Board>::generate(p, move_list);
        if (depth == 1)
                return move_list.size();
        
        NodeCount leafs = 0;
        Position<Board> q;
        for (size_t i = 0; i < move_list.size(); ++i) {
                q.template copy_make<Rules>(p, move_list[i]);
                leafs += bulk<Rules>(q, ply + 1, depth - 1);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::count(const Position<Board>& p, size_t ply, size_t depth)
{
        statistics_.update(ply);

        if (depth == 1)
                return Generate<Rules, Board>::count(p);

        Move::List move_list;
        Generate<Rules, Board>::generate(p, move_list);
        NodeCount leafs = 0;
        Position<Board> q;
        for (size_t i = 0; i < move_list.size(); ++i) {
                q.template copy_make<Rules>(p, move_list[i]);
                leafs += count<Rules>(q, ply + 1, depth - 1);
        }
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::hash(const Position<Board>& p, size_t ply, size_t depth)
{
        statistics_.update(ply);

        const Node* TT_entry = TT.find(p);
        if (TT_entry && TT_entry->is_sufficient(depth))
                return TT_entry->leafs();

        if (depth == 0)
                return 1;

        Move::List move_list;
        Generate<Rules, Board>::generate(p, move_list);
        NodeCount leafs = 0;
        Position<Board> q;
        for (size_t i = 0; i < move_list.size(); ++i) {
                q.template copy_make<Rules>(p, move_list[i]);
                leafs += hash<Rules>(q, ply + 1, depth - 1);
        }

        TT.insert(p, Node(leafs, depth));
        return leafs;
}

template<typename Rules, typename Board>
NodeCount Root::fast(const Position<Board>& p, size_t ply, size_t depth)
{
        statistics_.update(ply);

        const Node* TT_entry = TT.find(p);
        if (TT_entry && TT_entry->is_sufficient(depth))
                return TT_entry->leafs();

        NodeCount leafs;
        if (depth == 1)
                leafs = Generate<Rules, Board>::count(p);
        else {
                Move::List move_list;
                Generate<Rules, Board>::generate(p, move_list);
                leafs = 0;
                Position<Board> q;
                for (size_t i = 0; i < move_list.size(); ++i) {
                        q.template copy_make<Rules>(p, move_list[i]);
                        leafs += fast<Rules>(q, ply + 1, depth - 1);
                }
        }

        TT.insert(p, Node(leafs, depth));
        return leafs;
}

template<typename Board>
void Root::announce(const Position<Board>& p, size_t nominal_depth)
{        
        std::cout << write_position_layout<FEN_tag>()(p) << std::endl;
        std::cout << write_position_string<FEN_tag>()(p) << std::endl << std::endl;
        std::cout << "Searching to nominal depth=" << nominal_depth << std::endl;
        std::cout << std::endl;
}

template<typename Board>
void Root::announce(const Position<Board>& p, size_t nominal_depth, size_t num_moves)
{
        announce(p, nominal_depth);
        std::cout << "Found " << num_moves << " move_list, searching each to nominal depth=" << nominal_depth - 1 << std::endl;
        std::cout << std::endl;
}

}       // namesapce Walk
}       // namespace Tree
