#pragma once
#include <dctl/ray/iterator.hpp>        // Iterator

namespace dctl {
namespace ray {

template<class T>
struct direction;

template<class Board, int Direction>
struct direction<Iterator<Board, Direction>>
{
        enum { value = Direction };
};

}       // namespace ray
}       // namespace dctl
