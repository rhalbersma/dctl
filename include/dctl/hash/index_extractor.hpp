#pragma once
#include <cstdint>                      // uint64_t

namespace dctl {
namespace hash {

struct IndexExtractor
{
        typedef uint64_t result_type;

        template<class Key>
        result_type operator()(Key const& key) const
        {
                return key.hash_index();
        }
};

}       // namespace hash
}       // namespace dctl
