#pragma once
#include <utility>                      // pair

namespace dctl {
namespace grid {
namespace llo {

template<class Grid>
struct Coordinates
:
        std::pair<int, int>
{
        using std::pair<int, int>::pair;
};

}       // namespace llo
}       // namespace grid
}       // namespace dctl
