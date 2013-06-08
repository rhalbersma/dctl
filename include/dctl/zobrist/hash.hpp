#pragma once
#include <cstdint>                      // uint64_t
#include <dctl/zobrist/detail/hash.hpp> // hash

namespace dctl {
namespace zobrist {

template<class Index = uint64_t, class Key>
Index hash(Key const& key)
{
        return detail::hash<Index, Key>()(key);
}

}       // namespace zobrist
}       // namespace dctl
