#pragma once
#include <cstddef>      // std::size_t
#include "Transposition.hpp"
#include "../hash/DualMap.hpp"
#include "../node/Move.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/Statistics.hpp"

namespace dctl {

namespace successor { template<typename, typename> class Successor; }
template<typename> struct Position;
class Timer;

namespace walk {

class Root
{
public:
        template<typename, typename B> static NodeCount perft(const Position<B>&, int);
        template<typename, typename B> static NodeCount divide(const Position<B>&, int);
        template<typename, typename B> static NodeCount test(const Position<B>&, int);

        static void resize_hash(std::size_t);
        static void clear_hash();

private:
        template<typename B> static void announce(const Position<B>&, int);
        template<typename B> static void announce(const Position<B>&, int, int);

        static void report(int, NodeCount, const Timer&);
        static void summary();
        static void summary(NodeCount);
        static void print_move(const std::string&, int);
 
        template<typename, typename B> static NodeCount driver(const Position<B>&, int, int);
        template<typename, typename B> static NodeCount leaf(const Position<B>&, int, int);
        template<typename, typename B> static NodeCount bulk(const Position<B>&, int, int);
        template<typename, typename B> static NodeCount count(const Position<B>&, int, int);
        template<typename, typename B> static NodeCount hash(const Position<B>&, int, int);
        template<typename, typename B> static NodeCount fast(const Position<B>&, int, int);
        
        // representation
        static Statistics statistics_;

        // 32-byte hash entries: 24-byte piece lists signature, 8-byte (59-bit leafs, 5-bit depth) content
        // 2-way buckets on 64-byte cache lines, 2 * 2^23 buckets (= 1 Gb)
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef hash::DualMap<Move, Transposition> TranspositionTable;
        static TranspositionTable TT;
};

}       // namespace walk
}       // namespace dctl

// include template definitions inside header
#include "Root.hpp"
