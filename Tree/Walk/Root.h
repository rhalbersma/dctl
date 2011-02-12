#pragma once
#include "Node.h"
#include "../Statistics.h"
#include "../../Hash/DualMap.h"
#include "../../Position/Pieces.h"
#include "../../Utilities/IntegerTypes.h"

template<typename> class Position;
class StopWatch;

namespace Tree {
namespace Walk {

class Root
{
public:
        template<typename, typename B> static NodeCount perft(const Position<B>&, size_t);
        template<typename, typename B> static NodeCount divide(const Position<B>&, size_t);

private:
        template<typename B> static void announce(const Position<B>&, size_t);
        template<typename B> static void announce(const Position<B>&, size_t, size_t);

        static void report(size_t, NodeCount, const StopWatch&);
        static void summary(NodeCount);
        static void print_move(const std::string&, size_t);

        template<typename, typename B> static NodeCount driver(const Position<B>&, size_t, size_t);
        template<typename, typename B> static NodeCount leaf(const Position<B>&, size_t, size_t);
        template<typename, typename B> static NodeCount bulk(const Position<B>&, size_t, size_t);
        template<typename, typename B> static NodeCount count(const Position<B>&, size_t, size_t);
        template<typename, typename B> static NodeCount hash(const Position<B>&, size_t, size_t);
        template<typename, typename B> static NodeCount fast(const Position<B>&, size_t, size_t);
        
        // 32-byte hash entries: 24-byte piece lists signature, 8-byte (59-bit leafs, 5-bit depth) content
        // 2-way buckets on 64-byte cache lines, 2 * 2^23 buckets (= 1 Gb)
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef Hash::DualMap<Pieces, Node> TranspositionTable;

        static TranspositionTable TT;
        static Statistics statistics_;
};

}       // namespace Walk
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Root.hpp"
