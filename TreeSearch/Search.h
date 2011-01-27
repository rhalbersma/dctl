#pragma once
#include "Node.h"
#include "SearchStatistics.h"
#include "Value.h"
#include "../Hash/Map.h"
#include "../Position/Move.h"
#include "../Utilities/Ply.h"
#include "../Utilities/IntegerTypes.h"

template<typename> class Position;
class SearchParameters;
class StopWatch;

namespace TreeSearch {

class Search: private Value
{
public:
        // typdefs
        enum NodeType { ZW, PV };

        template<typename, typename B> static int root(const Position<B>&, size_t);

private:
        // 8-byte hash entries: 32-bit hash signature, 4-byte {value, type, depth, move} content
        // 8-way buckets on 64-byte cache lines, 2^24 buckets (= 1 Gb)
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef Hash::Map<uint32_t, Node> TranspositionTable;

        template<typename, typename B> static int negamax(const Position<B>&, size_t, size_t, SearchParameters&);
        template<typename, typename B> static int alpha_beta(const Position<B>&, size_t, size_t, int, int, SearchParameters&);

        template<size_t, typename, typename B> static int search(const Position<B>&, size_t, int, int, int, SearchParameters&);
        template<size_t, typename, typename B> static int quiescence(const Position<B>&, size_t, int, int, int, SearchParameters&);

        template<typename B> static void announce(const Position<B>&, size_t);
        static void report(size_t, int, int, int, const StopWatch&);

        template<typename, typename B> static void insert_PV(const SearchParameters&, const Position<B>&, int);
        template<typename, typename B> static void print_PV(const SearchParameters&, const Position<B>&, bool = false);

        static NodeCount nodes(void);
        static NodeCount sum_ply(void);
        static size_t max_ply(void);

        static void reset_statistics(void);
        static void update_statistics(size_t);
        static void identity_permutation(Move::Order&);

        static bool is_PV(size_t);

        // implementation
        static const size_t ROOT_ID_INCREMENT = 2;
        static const int R_NULL = 3;
        static const int R_NULL_VERIFY = 5;

        // representation
        static TranspositionTable TT;
        static SearchStatistics statistics_;
};

}       // namespace TreeSearch

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Search.hpp"
#include "Algorithms.hpp"