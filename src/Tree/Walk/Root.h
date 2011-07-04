#pragma once
#include "Entry.h"
#include "../Node/Pieces.h"
#include "../Statistics.h"
#include "../../Hash/DualMap.h"
#include "../../Utilities/IntegerTypes.h"

class Timer;

namespace tree {

namespace generate { template<typename, typename> class Successors; }
namespace node { template<typename> class Position; }

namespace walk {

class Root
{
public:
        template<typename, typename B> static NodeCount perft(const node::Position<B>&, int);
        template<typename, typename B> static NodeCount divide(const node::Position<B>&, int);

        static void resize_hash(size_t);
        static void clear_hash(void);

private:
        template<typename B> static void announce(const node::Position<B>&, int);
        template<typename B> static void announce(const node::Position<B>&, int, int);

        static void report(int, NodeCount, const Timer&);
        static void summary(void);
        static void summary(NodeCount);
        static void print_move(const std::string&, int);
 
        template<typename, typename B> static NodeCount driver(const node::Position<B>&, int, int);
        template<typename, typename B> static NodeCount leaf(const node::Position<B>&, int, int);
        template<typename, typename B> static NodeCount bulk(const node::Position<B>&, int, int);
        template<typename, typename B> static NodeCount count(const node::Position<B>&, int, int);
        template<typename, typename B> static NodeCount hash(const node::Position<B>&, int, int);
        template<typename, typename B> static NodeCount fast(const node::Position<B>&, int, int);
        
        // 32-byte hash entries: 24-byte piece lists signature, 8-byte (59-bit leafs, 5-bit depth) content
        // 2-way buckets on 64-byte cache lines, 2 * 2^23 buckets (= 1 Gb)
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef hash::DualMap<node::Pieces, Entry> TranspositionTable;

        static TranspositionTable TT;
        static Statistics statistics_;
};

}       // namespace walk
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Root.hpp"
