#pragma once
#include "Entry.h"
#include "Value.h"
#include "../Move/Types.h"
#include "../Statistics.h"
#include "../../Hash/Map.h"
#include "../../Utilities/Ply.h"
#include "../../Utilities/IntegerTypes.h"

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

        template<typename, typename B> static int analyze(const node::Position<B>&, size_t);

private:
        // 8-byte hash entries: 32-bit hash signature, 4-byte {value, type, depth, move} content
        // 8-way buckets on 64-byte cache lines, 2^24 buckets (= 1 Gb)
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef hash::Map<uint32_t, Entry> TranspositionTable;

        template<typename, typename B> static int negamax(const node::Position<B>&, size_t, size_t, Parameters&);
        template<typename, typename B> static int alpha_beta(const node::Position<B>&, size_t, size_t, int, int, Parameters&);
        template<typename, typename B> static int iterative_deepening(const node::Position<B>&, size_t);        
        template<size_t, typename, typename B> static int search(const node::Position<B>&, size_t, int, int, int, Parameters&);
        template<size_t, typename, typename B> static int quiescence(const node::Position<B>&, size_t, int, int, int, Parameters&);

        template<typename B> static void announce(const node::Position<B>&, size_t);
        static void report(size_t, int, const Timer&);

        template<typename, typename B> static void insert_PV(const node::Position<B>&, const move::Sequence&, int);
        template<typename, typename B> static void print_PV(const node::Position<B>&, const move::Sequence&);

        static void identity_permutation(move::Order&);
        static bool is_PV(size_t);

        // implementation
        static const size_t ROOT_ID_INCREMENT = 2;

        // representation
        static TranspositionTable TT;
        static Statistics statistics_;
};

}       // namespace search
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Root.hpp"
#include "Algorithms.hpp"
