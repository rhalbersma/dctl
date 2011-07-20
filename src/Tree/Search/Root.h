#pragma once
#include "Entry.h"
#include "Score.h"
#include "../Node/Stack.h"
#include "../Statistics.h"
#include "../../Hash/Map.h"
#include "../../utils/Ply.h"
#include "../../utils/IntegerTypes.h"

class Timer;

namespace tree {

namespace generate { template<typename, typename> class Successors; }
namespace node { template<typename> class Position; }

namespace search {

class Parameters;

class Root
{
public:
        // typdefs
        enum EntryType { ZW, PV };

        template<typename, typename B> static int analyze(const node::Position<B>&, int);

        static void resize_hash(size_t);
        static void clear_hash();
        static void interrupt();
        static bool is_interrupted();

private:
        // 8-byte hash entries: 32-bit hash signature, 4-byte {value, type, depth, move} content
        // 8-way buckets on 64-byte cache lines, 2^24 buckets (= 1 Gb)
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef hash::Map<uint32_t, Entry> TranspositionTable;

        template<typename, typename B> static int negamax(const node::Position<B>&, int, int, Parameters&);
        template<typename, typename B> static int alpha_beta(const node::Position<B>&, int, int, int, int, Parameters&);
        template<typename, typename B> static int iterative_deepening(const node::Position<B>&, int);        
        template<int, typename, typename B> static int pvs(const node::Position<B>&, int, int, int, int, Parameters&);
        template<int, typename, typename B> static int quiescence(const node::Position<B>&, int, int, int, int, Parameters&);

        template<typename B> static void announce(const node::Position<B>&, int);
        static void report(int, int, const Timer&);

        template<typename, typename B> static void insert_PV(const node::Position<B>&, const node::Sequence&, int);
        template<typename, typename B> static void print_PV(const node::Position<B>&, const node::Sequence&);

        static void identity_permutation(node::Order&);
        static bool is_PV(int);

        // implementation
        static const int ROOT_ID_INCREMENT = 2;

        // representation
        static TranspositionTable TT;
        static Statistics statistics_;
        static bool interrupted_;
};

}       // namespace search
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Root.hpp"
#include "Algorithms.hpp"
