#pragma once
#include "../Tree/Search/Entry.h"
#include <functional>

namespace Hash {

template<typename Entry, typename Key>
struct key_equal_to: public std::binary_function<Entry, Key, bool>
{
        bool operator()(const Entry&, const Key&) const;
};

template<typename Entry>
struct type_equal_to: public std::binary_function<Entry, Tree::Search::Entry::Type, bool>
{
        bool operator()(const Entry&, Tree::Search::Entry::Type) const;
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

}       // namespace Hash

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "EntryPredicates.hpp"
