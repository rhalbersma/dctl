#pragma once
#include <cstddef>      // std::size_t
#include <boost/config.hpp>
#include "Transposition.h"
#include "Score.h"
#include "../node/Stack.hpp"
#include "../hash/Map.h"
#include "../utility/Ply.h"
#include "../utility/IntegerTypes.h"
#include "../utility/Statistics.h"

namespace dctl {

namespace successor { template<typename, typename> class Successor; }
template<typename> class Position;
class Timer;

namespace search {

class Parameters;

class Root
{
public:
        // typdefs
        enum EntryType { ZW, PV };

        template<typename, typename B> static int analyze(const Position<B>&, int);

        static void resize_hash(std::size_t);
        static void clear_hash();
        static void interrupt();
        static bool is_interrupted();

private:
        template<typename, typename B> static int negamax(const Position<B>&, int, int, Parameters&);
        template<typename, typename B> static int alpha_beta(const Position<B>&, int, int, int, int, Parameters&);
        template<typename, typename B> static int iterative_deepening(const Position<B>&, int);        
        template<int, typename, typename B> static int pvs(const Position<B>&, int, int, int, int, Parameters&);
        template<int, typename, typename B> static int quiescence(const Position<B>&, int, int, int, int, Parameters&);

        template<typename B> static void announce(const Position<B>&, int);
        template<typename, typename B> static void report(int, int, const Timer&, const Position<B>&, const Sequence&);

        template<typename, typename B> static void insert_PV(const Position<B>&, const Sequence&, int);
        template<typename, typename B> static void print_PV(const Position<B>&, const Sequence&);

        static bool is_PV(int);

        // implementation
        BOOST_STATIC_CONSTANT(auto, ROOT_ID_INCREMENT = 2);

        // representation
        static Statistics statistics_;
        static bool interrupted_;

        // 8-byte hash entries: 32-bit hash signature, 4-byte {value, type, depth, move} content
        // 8-way buckets on 64-byte cache lines, (1 Gb = 2^27 entries) 
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef hash::Map<uint32_t, Transposition> TranspositionTable;
        static TranspositionTable TT;
};

}       // namespace search
}       // namespace dctl

// include template definitions inside header
#include "Root.hpp"
#include "Algorithms.hpp"
