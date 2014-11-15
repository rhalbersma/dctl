#pragma once
#include <cstddef>                              // size_t
#include <cstdint>                              // uint64_t
#include <dctl/position/mru_king/mru_king.hpp>  // MostRecentlyUsedKing
#include <dctl/random/uniform.hpp>              // cat_zero_runif

namespace dctl {
namespace random {

template<int M, int N>
struct MostRecentlyUsedKing
{
        static std::array<uint64_t, 1 + N> const index[];
        static std::array<uint64_t, 1 + M> const moves[];
};

template<int M, int N>
std::array<uint64_t, 1 + N> const
MostRecentlyUsedKing<M, N>::index[] =
{
        cat_zero_runif<N>(),
        cat_zero_runif<N>()
};

template<int M, int N>
std::array<uint64_t, 1 + M> const
MostRecentlyUsedKing<M, N>::moves[] =
{
        cat_zero_runif<M>(),
        cat_zero_runif<M>()
};

}       // namespace random

template<class Rules, class Board>
auto init_hash(MostRecentlyUsedKing<Rules, Board> const& mru_king, bool to_move)
{
        enum { M = MostRecentlyUsedKing<Rules, Board>::M };
        enum { N = MostRecentlyUsedKing<Rules, Board>::N };
        using Zobrist = random::MostRecentlyUsedKing<M, N>;

        return
                Zobrist::index[to_move][mru_king.index()] ^
                Zobrist::moves[to_move][mru_king.moves()]
        ;
}

}       // namespace dctl
