#pragma once
#include <functional>                   // function

namespace dctl {
namespace hash {

// primary template
template<typename Index, typename Position>
struct Find
:
        std::function<Index(Position)>
{
        Index operator()(Position const& p) const
        {
                return (p.hash_index());
        }
};

}       // namespace hash
}       // namespace dctl
