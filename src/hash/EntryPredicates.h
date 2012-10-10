#pragma once
#include <functional>   // std::binary_function

namespace dctl {
namespace hash {

template<typename Entry, typename Key>
struct key_equal_to
: 
        public std::binary_function<Entry, Key, bool>
{
        bool operator()(const Entry&, const Key&) const;
};

template<typename Entry>
struct depth_compare
: 
        public std::binary_function<Entry, Entry, bool>
{
        bool operator()(const Entry&, const Entry&) const;
};

template<typename Entry>
struct leafs_compare
: 
        public std::binary_function<Entry, Entry, bool>
{
        bool operator()(const Entry&, const Entry&) const;
};

}       // namespace hash
}       // namespace dctl

// include template definitions inside header
#include "EntryPredicates.hpp"
