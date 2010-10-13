#pragma once
#include "../Search/SearchNode.h"
#include <functional>

template<typename Entry, typename Key>
struct key_equal_to: public std::binary_function<Entry, Key, bool>
{
        bool operator()(const Entry&, const Key&) const;
};

template<typename Entry>
struct type_equal_to: public std::binary_function<Entry, SearchNode::Type, bool>
{
        bool operator()(const Entry&, SearchNode::Type) const;
};

template<typename Entry>
struct depth_compare: public std::binary_function<Entry, Entry, bool>
{
        bool operator()(const Entry&, const Entry&) const;
};

template<typename Entry>
struct leafs_compare: public std::binary_function<Entry, Entry, bool>
{
        bool operator()(const Entry&, const Entry&) const;
};

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "HashEntryPredicates.hpp"
