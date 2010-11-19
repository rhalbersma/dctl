#pragma once
#include <functional>

template<typename Key, typename Index>
struct ZobristFind: public std::unary_function<Key, Index>
{
        Index operator()(const Key&) const;
};

template<typename Key, typename Index>
struct ZobristHash: public std::unary_function<Key, Index>
{
        Index operator()(const Key&) const;
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "ZobristHash.hpp"
