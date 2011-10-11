#pragma once
#include <functional>                   // std::unary_function
#include "../node/Position.hpp"

namespace dctl {
namespace hash {

// primary template
template<typename Key, typename Index>
struct Find;

// partial specialization for retrieval of pre-computed indices of positions
template<typename Board, typename Index>
struct Find<Position<Board>, Index>
: 
        public std::unary_function<Position<Board>, Index>
{
        Index operator()(const Position<Board>& p) const
        {
                return p.hash_index();
        }
};

}       // namespace hash
}       // namespace dctl
