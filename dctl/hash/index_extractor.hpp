#pragma once
#include <cstdint>
#include <functional>                   // function

namespace dctl {
namespace hash {

struct IndexExtractor
{
        typedef uint64_t value_type;

        template<typename Key>
        value_type operator()(Key const& key) const
        {
                return (key.hash_index());
        }
};

}       // namespace hash
}       // namespace dctl
