#pragma once
#include <functional>                   // function

namespace dctl {
namespace hash {

// primary template
template<typename Index, typename Key>
struct Find;

// partial specialization for retrieval of pre-computed indices of positions
template<typename Index, template<typename, typename> class Position, typename Rules, typename Board>
struct Find< Index, Position<Rules, Board> >
:
        std::function<Index(Position<Rules, Board>)>
{
        Index operator()(Position<Rules, Board> const& p) const
        {
                return p.hash_index();
        }
};

}       // namespace hash
}       // namespace dctl
