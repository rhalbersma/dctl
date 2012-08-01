#pragma once
#include <vector>                       // vector

namespace dctl {
namespace search {

typedef std::vector<int> Variation;
typedef std::vector<int> Order;

inline Variation make_variation(int move, Variation const& continuation)
{
        Variation line(1, move);
        line.insert(std::end(line), std::begin(continuation), std::end(continuation));
        return line;
}

}       // namespace search
}       // namespace dctl
