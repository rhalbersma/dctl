#pragma once
#include <functional>                   // std::binary_function

namespace dctl {
namespace hash {

template<typename Entry, typename Key>
struct key_equal_to
: 
        public std::binary_function<Entry, Key, bool>
{
        bool operator()(const Entry& entry, const Key& key) const
        {
                return entry.first == key;
        }
};

template<typename Entry>
struct depth_compare
: 
        public std::binary_function<Entry, Entry, bool>
{
        bool operator()(const Entry& left, const Entry& right) const
        {
                return left.second.depth() < right.second.depth();
        }
};

template<typename Entry>
struct leafs_compare
: 
        public std::binary_function<Entry, Entry, bool>
{
        bool operator()(const Entry& left, const Entry& right) const
        {
                return left.second.leafs() < right.second.leafs();
        }
};

}       // namespace hash
}       // namespace dctl
