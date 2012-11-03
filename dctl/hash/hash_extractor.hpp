#pragma once
#include <cstdint>
#include <functional>                   // function

namespace dctl {
namespace hash {

struct HashExtractor
{
        typedef uint64_t index_type;

        template<typename U>
        index_type operator()(U const& u) const
        {
                return (u.hash_index());
        }
};

}       // namespace hash
}       // namespace dctl
