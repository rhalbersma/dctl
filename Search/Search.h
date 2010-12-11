#pragma once
#include "SearchNode.h"
#include "SearchStatistics.h"
#include "SearchValue.h"
#include "../Containers/Generic/HashMap.h"
#include "../Position/Move.h"
#include "../Utilities/Ply.h"
#include "../Utilities/IntegerTypes.h"

template<typename> class Position;
class SearchParameters;
class StopWatch;

class Search: private SearchValue
{
public:
        // typdefs
        enum NodeType { ZW, PV };

        template<typename, typename B> static int root(const Position<B>&, size_t);

private:
        // 32-bit hash signature, 4-byte {value, type, depth, move} entries, 2^27 entries (= 1 Gb)
        // depth-preferred replacement, incremental Zobrist hashing, 64-bit indices
        typedef HashMap<uint32_t, SearchNode, 27> TranspositionTable;

        template<typename, typename B> static int negamax(Position<B>&, size_t, size_t, SearchParameters&);
        template<typename, typename B> static int alpha_beta(Position<B>&, size_t, size_t, int, int, SearchParameters&);

        template<size_t, typename, typename B> static int search(Position<B>&, size_t, int, int, int, SearchParameters&);
        template<size_t, typename, typename B> static int quiescence(Position<B>&, size_t, int, int, int, SearchParameters&);

        template<typename B> static void announce(const Position<B>&, size_t);
        static void report(size_t, int, int, int, const StopWatch&);

        template<typename, typename B> static void insert_PV(const SearchParameters&, const Position<B>&, int);
        template<typename, typename B> static void print_PV(const SearchParameters&, const Position<B>&, bool = false);

        static uint64_t nodes(void);
        static uint64_t sum_ply(void);
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
        static SearchStatistics d_statistics;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Search.hpp"
#include "SearchAlgorithms.hpp"
