#pragma once
#include <functional>                   // unary_function

namespace dctl {
namespace hash {

// primary template
template<typename Key, typename Index>
struct Find;

// partial specialization for retrieval of pre-computed indices of positions
template<typename Rules, typename Board, template<typename, typename> class Position, typename Index>
struct Find<Position<Rules, Board>, Index>
:
        public std::unary_function<Position<Rules, Board>, Index>
{
        Index operator()(Position<Rules, Board> const& p) const
        {
                return p.hash_index();
        }
};

}       // namespace hash
}       // namespace dctl
