#pragma once
#include <dctl/utility/ply.hpp>         // PlyCount
#include <cstdint>
#include <random>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/config/limits.hpp>

namespace dctl {
namespace zobrist {
namespace detail {

template<class Index>
struct Random
{
        static uint64_t gen_random()
        {
                static std::mt19937 engine;
                static std::uniform_int_distribution<uint64_t> dist;
                return dist(engine);
        }

        // xor with random numbers matching a bitboard's set 1-bits
        template<template<class, class, int> class Set, class T, class Block, int Nb>
        static Index xor_rand(Set<T, Block, Nb> b, Index const* random)
        {
                auto hash = Index{};
                for (auto sq : b)
                        hash ^= random[sq];
                return hash;
        }

        // xor with random numbers conditional on a ply count
        static Index xor_rand(int i, Index const* random)
        {
                return random[i];
        }

        // xor with a random number conditional on a boolean
        static Index xor_rand(bool b, Index const* random)
        {
                return random[b];
        }

        static Index const PIECES[][BOOST_PP_LIMIT_REPEAT];
        static Index const KINGS[];
        static Index const SIDE[];
        static Index const RESTRICTED_KING[][BOOST_PP_LIMIT_REPEAT];
        static Index const RESTRICTED_MOVES[][BOOST_PP_LIMIT_REPEAT];
};

#ifndef DCTL_PP_RANDOM64
    #define DCTL_PP_RANDOM64(z, i, data) gen_random()
#endif

template<class Index>
Index const Random<Index>::PIECES[][BOOST_PP_LIMIT_REPEAT] =
{
        { BOOST_PP_ENUM(BOOST_PP_LIMIT_REPEAT, DCTL_PP_RANDOM64, ~) },
        { BOOST_PP_ENUM(BOOST_PP_LIMIT_REPEAT, DCTL_PP_RANDOM64, ~) }
};

template<class Index>
Index const Random<Index>::KINGS[] =
{
        BOOST_PP_ENUM(BOOST_PP_LIMIT_REPEAT, DCTL_PP_RANDOM64, ~)
};

template<class Index>
Index const Random<Index>::SIDE[] =
{
        0x0, BOOST_PP_ENUM(1, DCTL_PP_RANDOM64, ~)
};

template<class Index>
Index const Random<Index>::RESTRICTED_KING[][BOOST_PP_LIMIT_REPEAT] =
{
        { BOOST_PP_ENUM(BOOST_PP_LIMIT_REPEAT, DCTL_PP_RANDOM64, ~) },
        { BOOST_PP_ENUM(BOOST_PP_LIMIT_REPEAT, DCTL_PP_RANDOM64, ~) }
};

template<class Index>
Index const Random<Index>::RESTRICTED_MOVES[][BOOST_PP_LIMIT_REPEAT] =
{
        { BOOST_PP_ENUM(BOOST_PP_LIMIT_REPEAT, DCTL_PP_RANDOM64, ~) },
        { BOOST_PP_ENUM(BOOST_PP_LIMIT_REPEAT, DCTL_PP_RANDOM64, ~) }
};

#undef PP_RANDOM64

// explicit instantiation
template struct Random<uint64_t>;

}       // namespace detail
}       // namespace zobrist
}       // namespace dctl
