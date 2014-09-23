#pragma once
#include <utility>                      // pair

namespace dctl {
namespace grid {
namespace ulo {

template<class Grid>
struct Coordinates
:
        std::pair<int, int>
{
        using std::pair<int, int>::pair;
};

}       // namespace ulo
}       // namespace grid
}       // namespace dctl
