#pragma once
#include <utility>                      // pair

namespace dctl {
namespace grid {
namespace sco {

struct Coordinates
:
        std::pair<int, int>
{
        using std::pair<int, int>::pair;
};

}       // namespace sco
}       // namespace grid
}       // namespace dctl
