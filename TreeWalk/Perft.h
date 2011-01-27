#pragma once
#include "Node.h"
#include "../Hash/DualMap.h"
#include "../Position/Pieces.h"
#include "../TreeSearch/SearchStatistics.h"
#include "../Utilities/IntegerTypes.h"

template<typename> class Position;
class StopWatch;

namespace TreeWalk {

class Perft
{
public:
        template<typename, typename B> static NodeCount perft(const Position<B>&, size_t);
        template<typename, typename B> static NodeCount driver(const Position<B>&, size_t, size_t);

        template<typename B> static void announce(const Position<B>&, size_t);

        static void report(size_t, NodeCount, const StopWatch&);

        static void reset_statistics(void);
        static void update_statistics(size_t);

private:
        template<typename, typename B> static NodeCount perft_leaf(const Position<B>&, size_t, size_t);
        template<typename, typename B> static NodeCount perft_bulk(const Position<B>&, size_t, size_t);
        template<typename, typename B> static NodeCount perft_size(const Position<B>&, size_t, size_t);
        template<typename, typename B> static NodeCount perft_hash(const Position<B>&, size_t, size_t);
        template<typename, typename B> static NodeCount perft_fast(const Position<B>&, size_t, size_t);
        
        static NodeCount nodes(void);
        static NodeCount sum_ply(void);
        static size_t max_ply(void);

        // 32-byte hash entries: 24-byte piece lists signature, 8-byte (59-bit leafs, 5-bit depth) content
        // 2-way buckets on 64-byte cache lines, 2 * 2^23 buckets (= 1 Gb)
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef Hash::DualMap<Pieces, Node> TranspositionTable;

        static TranspositionTable TT;
        static SearchStatistics statistics_;
};

}       // namespace TreeWalk

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Perft.hpp"