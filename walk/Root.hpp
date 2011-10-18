#pragma once
#include <cstddef>      // std::size_t
#include <iomanip>
#include <iostream>
#include <boost/assert.hpp>
#include "Transposition.hpp"
#include "../hash/DualMap.hpp"
#include "../node/Move.hpp"
#include "../node/Position.hpp"
#include "../node/Stack.hpp"
#include "../successor/Successor.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/Statistics.hpp"
#include "../utility/Timer.hpp"

#include "../setup/Diagram.hpp"
#include "../setup/String.hpp"
#include "../notation/String.hpp"

namespace dctl {
namespace walk {

template
<
        typename Rules,
        typename Board
>
class Root
{
public:               
        NodeCount perft(const Position<Board>& p, int depth)
        {
                NodeCount leafs = 0;
        
                Timer timer;
                announce(p, depth);
                for (auto i = 1; i <= depth; ++i) {                        
                        statistics_.reset();
                        leafs = driver(p, 0, i);
                        timer.split();
                        report(i, leafs, timer);
                }
                summary();
                return leafs;
        }
        
        NodeCount divide(const Position<Board>& p, int depth)
        {
                NodeCount leafs = 0;
                NodeCount move_leafs;                
        
                Timer timer;
                Stack moves;
                Successor<successor::Legal, Rules>::generate(p, moves);

                announce(p, depth, moves.size());
                for (auto i = 0; i < static_cast<int>(moves.size()); ++i) {
                        statistics_.reset();
                        print_move(notation::write<Rules>()(p, moves[i]), i);

                        auto q = p;
                        q.attach(p);
                        q.template make<Rules>(moves[i]);
                        move_leafs = driver(q, 0, depth - 1);
                        leafs += move_leafs;              
                
                        timer.split();
                        Root::report(depth - 1, move_leafs, timer);
                }
                summary(leafs);

                return leafs;
        }
        
        NodeCount test(const Position<Board>& p, int depth)
        {
                return driver(p, 0, depth);
        }

        void resize_hash(std::size_t s)
        {
                return TT.resize(s);
        }
        
        void clear_hash()
        {
                return TT.clear();
        }

private:
        void announce(const Position<Board>& p, int depth)
        {        
                std::cout << setup::diagram<pdn::protocol>()(p);
                std::cout << setup::write<pdn::protocol>()(p) << "\n";
                std::cout << "Searching to nominal depth=" << depth << "\n\n";
        }
        
        void announce(const Position<Board>& p, int depth, int num_moves)
        {
                announce(p, depth);
                std::cout << "Found " << num_moves << " moves, searching each to nominal depth=" << depth - 1 << "\n";
                std::cout << "\n";
        }

        void report(int depth, NodeCount leafs, const Timer& timer)
        {
                std::cout << "info";

                std::cout << " depth ";
                std::cout << std::setw( 2) << depth;

                std::cout << " leafs ";
                std::cout << std::setw(12) << std::right << leafs;

                std::cout << " nodes ";
                std::cout << std::setw(12) << std::right << statistics_.nodes();

                std::cout << " time ";
                std::cout << std::setw( 6) << timer.elapsed();

                const double nps = (1000 * statistics_.nodes()) / static_cast<double>(timer.lap());
                std::cout << " nps ";
                std::cout << std::dec << std::setiosflags(std::ios::fixed) << std::setprecision(0);
                std::cout << std::setw( 7) << nps;

                const double hashfull = 1000 * (static_cast<double>((TT.size() - TT.available())) / TT.size());
                std::cout << " hashfull ";
                std::cout << std::setw( 4) << std::right << hashfull;

                std::cout << "\n";
        }
        
        void summary()
        {
                std::cout << "\n";
        }
        
        void summary(NodeCount leafs)
        {
                std::cout << "Total leafs: " << leafs << "\n\n";
        }
        
        void print_move(const std::string& move, int i)
        {
                std::cout << std::setw(2) << (i + 1) << "." << move << " ";
        }
 
        NodeCount driver(const Position<Board>& p, int ply, int depth)
        {
                return (depth == 0)? leaf(p, ply, depth) : fast(p, ply, depth);
        }
        
        NodeCount leaf(const Position<Board>& p, int ply, int depth)
        {        
                statistics_.update(ply);

                if (depth == 0)
                        return 1;

                Stack moves;
                Successor<successor::Legal, Rules>::generate(p, moves);
                NodeCount leafs = 0;        
                for (auto m = moves.cbegin(); m != moves.cend(); ++m) {
                        auto q = p;
                        q.attach(p);
                        q.template make<Rules>(*m);
                        leafs += leaf(q, ply + 1, depth - 1);
                }
                return leafs;
        }
        
        NodeCount bulk(const Position<Board>& p, int ply, int depth)
        {
                statistics_.update(ply);

                Stack moves;
                Successor<successor::Legal, Rules>::generate(p, moves);
                if (depth == 1)
                        return moves.size();
        
                NodeCount leafs = 0;
                for (auto m = moves.cbegin(); m != moves.cend(); ++m) {
                        auto q = p;
                        q.attach(p);
                        q.template make<Rules>(*m);
                        leafs += bulk(q, ply + 1, depth - 1);
                }
                return leafs;
        }
        
        NodeCount count(const Position<Board>& p, int ply, int depth)
        {
                statistics_.update(ply);

                if (depth == 1)
                        return Successor<successor::Legal, Rules>::count(p);

                Stack moves;
                Successor<successor::Legal, Rules>::generate(p, moves);
                NodeCount leafs = 0;
                for (auto m = moves.cbegin(); m != moves.cend(); ++m) {
                        auto q = p;
                        q.attach(p);
                        q.template make<Rules>(*m);
                        leafs += count(q, ply + 1, depth - 1);
                }
                return leafs;
        }
        
        NodeCount hash(const Position<Board>& p, int ply, int depth)
        {
                statistics_.update(ply);

                auto TT_entry = TT.find(p);
                if (TT_entry && TT_entry->depth() == depth)
                        return TT_entry->leafs();

                if (depth == 0)
                        return 1;

                Stack moves;
                Successor<successor::Legal, Rules>::generate(p, moves);
                NodeCount leafs = 0;
                for (auto m = moves.cbegin(); m != moves.cend(); ++m) {
                        auto q = p;
                        q.attach(p);
                        q.template make<Rules>(*m);
                        leafs += hash(q, ply + 1, depth - 1);
                }

                TT.insert(p, Transposition(leafs, depth));
                return leafs;
        }
        
        NodeCount fast(const Position<Board>& p, int ply, int depth)
        {
                statistics_.update(ply);

                auto TT_entry = TT.find(p);
                if (TT_entry && TT_entry->depth() == depth)
                        return TT_entry->leafs();

                NodeCount leafs;
                if (depth == 1) {
                        leafs = Successor<successor::Legal, Rules>::count(p);
                } else {
                        Stack moves;
                        Successor<successor::Legal, Rules>::generate(p, moves);
                        leafs = 0;
                        for (auto m = moves.cbegin(); m != moves.cend(); ++m) {
                                auto q = p;
                                q.attach(p);
                                q.template make<Rules>(*m);
                                leafs += fast(q, ply + 1, depth - 1);
                        }
                }

                TT.insert(p, Transposition(leafs, depth));
                return leafs;
        }
        
        // 32-byte hash entries: 24-byte piece lists signature, 8-byte (59-bit leafs, 5-bit depth) content
        // 2-way buckets on 64-byte cache lines, 2 * 2^23 buckets (= 1 Gb)
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef hash::DualMap<Move, Transposition> TranspositionTable;
        TranspositionTable TT;

        // representation
        Statistics statistics_;
};

}       // namespace walk
}       // namespace dctl
